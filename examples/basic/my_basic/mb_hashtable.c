/*
** This source file is part of MY-BASIC
**
** For the latest info, see https://github.com/paladin-t/my_basic/
**
** Copyright (C) 2011 - 2024 Tony Wang
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of
** this software and associated documentation files (the "Software"), to deal in
** the Software without restriction, including without limitation the rights to
** use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
** the Software, and to permit persons to whom the Software is furnished to do so,
** subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
** FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
** IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef _MSC_VER
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif /* _CRT_SECURE_NO_WARNINGS */
#endif /* _MSC_VER */

#include "my_basic.h"
#include "mb_internal.h"

/** Dictionary operations */

unsigned _ht_hash_object(void* ht, void* d) {
	unsigned result = 0;
	_ht_node_t* self = (_ht_node_t*)ht;
	_object_t* o = (_object_t*)d;
	size_t i = 0;
	unsigned h = 0;
#ifdef MB_ENABLE_CLASS
	_object_t val;
#endif /* MB_ENABLE_CLASS */

	mb_assert(ht);

	h = o->type;
	switch(o->type) {
	case _DT_STRING:
		h = 5 * h + _ht_hash_string(ht, o->data.string);
		if(self->array_size == 1)
			result = 0;
		else
			result = h % self->array_size;

		break;
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		if(o->data.instance->hash) {
			mb_interpreter_t* s = o->data.instance->ref.s;
			_ls_node_t ast;
			_ls_node_t* tmp = &ast;
			mb_value_t va[1];
			mb_make_nil(va[0]);
			memset(&ast, 0, sizeof(_ls_node_t));
			if(_eval_routine(s, &tmp, va, 1, o->data.instance->hash, _has_routine_fun_arg, _pop_routine_fun_arg) == MB_FUNC_OK) {
				_MAKE_NIL(&val);
				_public_value_to_internal_object(&s->running_context->intermediate_value, &val);
				if(val.type != _DT_INT) {
					int ignored = MB_FUNC_OK;
					_handle_error_on_obj(s, SE_RN_INTEGER_EXPECTED, s->source_file, o, MB_FUNC_ERR, _exit, ignored);
				}

				o = &val;
			}
		}

		goto _default;
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		if(o->data.usertype_ref->hash) {
			if(self->array_size == 1) {
				result = 0;
			} else {
				h = 5 * h + o->data.usertype_ref->hash(o->data.usertype_ref->ref.s, o->data.usertype_ref->usertype);
				result = h % self->array_size;
			}

			break;
		}

		goto _default;
#endif /* MB_ENABLE_USERTYPE_REF */
	default:
#if defined MB_ENABLE_CLASS || defined MB_ENABLE_USERTYPE_REF
_default:
#endif /* MB_ENABLE_CLASS || MB_ENABLE_USERTYPE_REF */
		if(self->array_size == 1) {
			result = 0;
		} else {
			for(i = 0; i < sizeof(_raw_t); ++i)
				h = 5 * h + o->data.raw[i];
			result = h % self->array_size;
		}

		break;
	}

	goto _exit; /* Avoid an unreferenced label warning */

_exit:
	return result;
}

unsigned _ht_hash_string(void* ht, void* d) {
	unsigned result = 0;
	_ht_node_t* self = (_ht_node_t*)ht;
	char* s = (char*)d;
	unsigned h = 0;

	mb_assert(ht);

	if(self->array_size == 1) {
		result = 0;
	} else {
		for( ; *s; ++s)
			h = 5 * h + *s;
		result = h % self->array_size;
	}

	return result;
}

unsigned _ht_hash_intptr(void* ht, void* d) {
	unsigned result = 0;
	_ht_node_t* self = (_ht_node_t*)ht;

	mb_assert(ht);

	if(self->array_size == 1) {
		result = 0;
	} else {
		uintptr_t u = (uintptr_t)d;
		result = (unsigned)(u % self->array_size);
	}

	return result;
}

unsigned _ht_hash_ref(void* ht, void* d) {
	unsigned result = 0;
	_ht_node_t* self = (_ht_node_t*)ht;
	_ref_t* ref = (_ref_t*)d;

	mb_assert(ht);

	if(self->array_size == 1) {
		result = 0;
	} else {
		uintptr_t u = (uintptr_t)ref;
		result = (unsigned)(u % self->array_size);
	}

	return result;
}

int _ht_cmp_object(void* d1, void* d2) {
	_object_t* o1 = (_object_t*)d1;
	_object_t* o2 = (_object_t*)d2;
	int i = 0;
#ifdef MB_ENABLE_CLASS
	_routine_t* cmp = 0;
	_object_t val;
	bool_t fst = true;
#endif /* MB_ENABLE_CLASS */

	if(o1->type < o2->type)
		return -1;
	else if(o1->type > o2->type)
		return 1;

	switch(o1->type) {
	case _DT_STRING:
		return _ht_cmp_string(o1->data.string, o2->data.string);
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		if(o1->data.instance->compare) {
			cmp = o1->data.instance->compare;
			fst = true;
		} else if(o2->data.instance->compare) {
			cmp = o2->data.instance->compare;
			fst = false;
		}
		if(cmp) {
			mb_interpreter_t* s = o1->data.instance->ref.s;
			_ls_node_t ast;
			_ls_node_t* tmp = &ast;
			mb_value_t va[1];
			mb_make_nil(va[0]);
			_internal_object_to_public_value(fst ? o2 : o1, &va[0]);
			memset(&ast, 0, sizeof(_ls_node_t));
			if(_eval_routine(s, &tmp, va, 1, cmp, _has_routine_fun_arg, _pop_routine_fun_arg) == MB_FUNC_OK) {
				_MAKE_NIL(&val);
				_public_value_to_internal_object(&s->running_context->intermediate_value, &val);
				if(val.type != _DT_INT) {
					int ignored = MB_FUNC_OK;
					_handle_error_on_obj(s, SE_RN_INTEGER_EXPECTED, s->source_file, o1, MB_FUNC_ERR, _exit, ignored);
				}

				return (int)(fst ? val.data.integer : -val.data.integer);
			}
		}

		goto _default;
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		if(o1->data.usertype_ref->cmp)
			return o1->data.usertype_ref->cmp(o1->data.usertype_ref->ref.s, o1->data.usertype_ref->usertype, o2->data.usertype_ref->usertype);
		else if(o2->data.usertype_ref->cmp)
			return o2->data.usertype_ref->cmp(o1->data.usertype_ref->ref.s, o1->data.usertype_ref->usertype, o2->data.usertype_ref->usertype);

		goto _default;
#endif /* MB_ENABLE_USERTYPE_REF */
	default:
#if defined MB_ENABLE_CLASS || defined MB_ENABLE_USERTYPE_REF
_default:
#endif /* MB_ENABLE_CLASS || MB_ENABLE_USERTYPE_REF */
		if(mb_is_little_endian()) {
			for(i = (int)sizeof(_raw_t) - 1; i >= 0; --i) {
				if(o1->data.raw[i] < o2->data.raw[i])
					return -1;
				else if(o1->data.raw[i] > o2->data.raw[i])
					return 1;
			}
		} else {
			for(i = 0; i < (int)sizeof(_raw_t); ++i) {
				if(o1->data.raw[i] < o2->data.raw[i])
					return -1;
				else if(o1->data.raw[i] > o2->data.raw[i])
					return 1;
			}
		}

		break;
	}

	goto _exit; /* Avoid an unreferenced label warning */

_exit:
	return 0;
}

int _ht_cmp_string(void* d1, void* d2) {
	char* s1 = (char*)d1;
	char* s2 = (char*)d2;

	return strcmp(s1, s2);
}

int _ht_cmp_intptr(void* d1, void* d2) {
	intptr_t i1 = (intptr_t)d1;
	intptr_t i2 = (intptr_t)d2;

	if(i1 < i2)
		return -1;
	else if(i1 > i2)
		return 1;

	return 0;
}

int _ht_cmp_ref(void* d1, void* d2) {
	_ref_t* r1 = (_ref_t*)d1;
	_ref_t* r2 = (_ref_t*)d2;
	intptr_t i = (intptr_t)r1 - (intptr_t)r2;

	if(i < 0)
		return -1;
	else if(i > 0)
		return 1;

	return 0;
}

_ht_node_t* _ht_create(unsigned size, _ht_compare_t cmp, _ht_hash_t hs, _ls_operation_t freeextra) {
	const unsigned array_size = size ? size : _HT_ARRAY_SIZE_DEFAULT;
	_ht_node_t* result = 0;
	unsigned ul = 0;

	if(!cmp)
		cmp = _ht_cmp_intptr;
	if(!hs)
		hs = _ht_hash_intptr;

	result = (_ht_node_t*)mb_malloc(sizeof(_ht_node_t));
	result->free_extra = freeextra;
	result->compare = cmp;
	result->hash = hs;
	result->array_size = array_size;
	result->count = 0;
#if _LAZY_HASH_TABLE
	mb_unrefvar(ul);

	result->array = 0;
#else /* _LAZY_HASH_TABLE */
	result->array = (_ls_node_t**)mb_malloc(sizeof(_ls_node_t*) * result->array_size);
	for(ul = 0; ul < result->array_size; ++ul)
		result->array[ul] = _ls_create();
#endif /* _LAZY_HASH_TABLE */

	return result;
}

_ls_node_t* _ht_find(_ht_node_t* ht, void* key) {
	_ls_node_t* result = 0;
	_ls_node_t* bucket = 0;
	unsigned hash_code = 0;

	mb_assert(ht && key);

	hash_code = ht->hash(ht, key);
	if(ht->array && ht->array[hash_code]) {
		bucket = ht->array[hash_code];
		bucket = bucket->next;
	}
	while(bucket) {
		if(ht->compare(bucket->extra, key) == 0) {
			result = bucket;

			break;
		}
		bucket = bucket->next;
	}

	return result;
}

unsigned _ht_set_or_insert(_ht_node_t* ht, void* key, void* value) {
	unsigned result = 0;
	_ls_node_t* bucket = 0;
	unsigned hash_code = 0;
	unsigned ul = 0;

	mb_assert(ht && key);

	bucket = _ht_find(ht, key);
	if(bucket) { /* Update */
		bucket->data = value;
		++result;
	} else { /* Insert */
		hash_code = ht->hash(ht, key);
		if(!ht->array) {
			ht->array = (_ls_node_t**)mb_malloc(sizeof(_ls_node_t*) * ht->array_size);
			for(ul = 0; ul < ht->array_size; ++ul)
				ht->array[ul] = 0;
		}
		if(!ht->array[hash_code])
			ht->array[hash_code] = _ls_create();
		bucket = ht->array[hash_code];
		bucket = _ls_pushback(bucket, value);
		mb_assert(bucket);
		bucket->extra = key;
		++ht->count;
		++result;
	}

	return result;
}

unsigned _ht_remove(_ht_node_t* ht, void* key, _ls_compare_t cmp) {
	unsigned result = 0;
	unsigned hash_code = 0;
	_ls_node_t* bucket = 0;

	mb_assert(ht && key);

	if(!cmp)
		cmp = _ls_cmp_extra;

	bucket = _ht_find(ht, key);
	hash_code = ht->hash(ht, key);
	if(ht->array && ht->array[hash_code]) {
		bucket = ht->array[hash_code];
		result = _ls_try_remove(bucket, key, cmp, ht->free_extra);
		ht->count -= result;
	}

	return result;
}

unsigned _ht_foreach(_ht_node_t* ht, _ht_operation_t op) {
	unsigned result = 0;
	_ls_node_t* bucket = 0;
	unsigned ul = 0;

	if(ht->array) {
		for(ul = 0; ul < ht->array_size; ++ul) {
			bucket = ht->array[ul];
			if(bucket)
				result += _ls_foreach(bucket, op);
		}
	}

	return result;
}

unsigned _ht_count(_ht_node_t* ht) {
	mb_assert(ht);

	return ht->count;
}

void _ht_clear(_ht_node_t* ht) {
	unsigned ul = 0;

	mb_assert(ht);

	if(ht->array) {
		for(ul = 0; ul < ht->array_size; ++ul) {
			if(ht->array[ul])
				_ls_clear(ht->array[ul]);
		}
		ht->count = 0;
	}
}

void _ht_destroy(_ht_node_t* ht) {
	unsigned ul = 0;

	mb_assert(ht);

	if(ht->array) {
		if(ht->free_extra)
			_ht_foreach(ht, ht->free_extra);

		for(ul = 0; ul < ht->array_size; ++ul) {
			if(ht->array[ul])
				_ls_destroy(ht->array[ul]);
		}

		safe_free(ht->array);
	}
	safe_free(ht);
}

int _ht_remove_existing(void* data, void* extra, _ht_node_t* ht) {
	int result = _OP_RESULT_NORMAL;
	mb_unrefvar(data);

	if(_ht_find(ht, extra))
		_ht_remove(ht, extra, 0);

	return result;
}
