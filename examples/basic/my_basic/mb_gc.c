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

#include "mb_lib_coll.h"

/* Garbage collector operations */

/* Add a referenced object to GC table for later garbage detection */
void _gc_add(_ref_t* ref, void* data, _gc_t* gc) {
	_ht_node_t* table = 0;

	mb_assert(ref && data);

	if(!ref->count)
		return;

	if(gc && _ht_find(gc->collected_table, ref))
		_ht_remove(gc->collected_table, ref, 0);

	if(!gc->table)
		return;

	if(gc->collecting)
		table = gc->recursive_table;
	else
		table = gc->table;

	if(gc && gc->valid_table && _ht_find(gc->valid_table, ref))
		_ht_remove(table, ref, 0);
	else if(ref->count && *ref->count > _NONE_REF)
		_ht_set_or_insert(table, ref, data);
	else
		_ht_remove(table, ref, 0);
}

/* Remove a referenced object from GC */
unsigned _gc_remove(_ref_t* ref, void* data, _gc_t* gc) {
	_ht_node_t* table = 0;

	mb_assert(ref && data && gc);

	if(gc->collecting)
		table = gc->recursive_table;
	else
		table = gc->table;

	if(table)
		return _ht_remove(table, ref, 0);

	return 0;
}

/* Get reachable objects */
int _gc_add_reachable(void* data, void* extra, void* h) {
	int result = _OP_RESULT_NORMAL;
	_object_t* obj = 0;
	_var_t* var = 0;
	_ht_node_t* ht = 0;

	mb_assert(data && h);

	ht = (_ht_node_t*)h;
	obj = (_object_t*)data;
	if(_is_internal_object(obj))
		goto _exit;
	switch(obj->type) {
	case _DT_VAR:
		var = (_var_t*)obj->data.variable;
		_gc_add_reachable(var->data, extra, ht);

		break;
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		if(!_ht_find(ht, &obj->data.usertype_ref->ref))
			_ht_set_or_insert(ht, &obj->data.usertype_ref->ref, obj->data.usertype_ref);
#ifdef MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF
		if(obj->data.usertype_ref->alive_checker) {
			mb_value_t val;
			mb_make_nil(val);
			_internal_object_to_public_value(obj, &val);
			obj->data.usertype_ref->alive_checker(obj->data.usertype_ref->ref.s, h, val, _gc_alive_marker);
		}
#endif /* MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF */

		break;
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_ARRAY_REF
	case _DT_ARRAY:
		if(!_ht_find(ht, &obj->data.array->ref))
			_ht_set_or_insert(ht, &obj->data.array->ref, obj->data.array);

		break;
#endif /* MB_ENABLE_ARRAY_REF */
#ifdef MB_ENABLE_COLLECTION_LIB
	case _DT_LIST:
		if(!_ht_find(ht, &obj->data.list->ref)) {
			_ht_set_or_insert(ht, &obj->data.list->ref, obj->data.list);
			_LS_FOREACH(obj->data.list->list, _do_nothing_on_object, _gc_add_reachable, ht);
		}

		break;
	case _DT_DICT:
		if(!_ht_find(ht, &obj->data.dict->ref)) {
			_ht_set_or_insert(ht, &obj->data.dict->ref, obj->data.dict);
			_HT_FOREACH(obj->data.dict->dict, _do_nothing_on_object, _gc_add_reachable_both, ht);
		}

		break;
	case _DT_LIST_IT:
		if(!_ht_find(ht, &obj->data.list_it->list->ref)) {
			_ht_set_or_insert(ht, &obj->data.list_it->list->ref, obj->data.list_it->list);
			_LS_FOREACH(obj->data.list_it->list->list, _do_nothing_on_object, _gc_add_reachable, ht);
		}

		break;
	case _DT_DICT_IT:
		if(!_ht_find(ht, &obj->data.dict_it->dict->ref)) {
			_ht_set_or_insert(ht, &obj->data.dict_it->dict->ref, obj->data.dict_it->dict);
			_HT_FOREACH(obj->data.dict_it->dict->dict, _do_nothing_on_object, _gc_add_reachable_both, ht);
		}

		break;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		if(!_ht_find(ht, &obj->data.instance->ref)) {
			_ht_set_or_insert(ht, &obj->data.instance->ref, obj->data.instance);
			_traverse_class(obj->data.instance, _gc_add_reachable, _add_class_meta_reachable, _META_LIST_MAX_DEPTH, false, ht, 0);
		}

		break;
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_LAMBDA
	case _DT_ROUTINE:
		if(obj->data.routine->type == MB_RT_LAMBDA) {
			if(!_ht_find(ht, &obj->data.routine->func.lambda.ref)) {
				_running_context_ref_t* outs = obj->data.routine->func.lambda.outer_scope;
				_ht_set_or_insert(ht, &obj->data.routine->func.lambda.ref, obj->data.routine);
				_HT_FOREACH(obj->data.routine->func.lambda.scope->var_dict, _do_nothing_on_object, _gc_add_reachable, ht);
				while(outs) {
					_ht_set_or_insert(ht, &outs->ref, outs);
					_HT_FOREACH(outs->scope->var_dict, _do_nothing_on_object, _gc_add_reachable, ht);
					outs = outs->prev;
				}
			}
		}

		break;
#endif /* MB_ENABLE_LAMBDA */
	default: /* Do nothing */
		break;
	}

_exit:
	return result;
}

/* Get reachable objects from both key and value */
int _gc_add_reachable_both(void* data, void* extra, void* h) {
	int result = _OP_RESULT_NORMAL;

	mb_assert(data && extra && h);

	_gc_add_reachable(extra, 0, h);
	_gc_add_reachable(data, extra, h);

	return result;
}

#ifdef MB_ENABLE_FORK
/* Get reachable objects in a forked environment */
int _gc_get_reachable_in_forked(void* data, void* extra, _ht_node_t* valid) {
	int result = _OP_RESULT_NORMAL;
	mb_interpreter_t* s = 0;
	_running_context_t* root = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	s = (mb_interpreter_t*)data;
	root = _get_root_scope(s->running_context);
	_gc_get_reachable(s, valid, root);

	return result;
}
#endif /* MB_ENABLE_FORK */

/* Get all reachable referenced objects */
void _gc_get_reachable(mb_interpreter_t* s, _ht_node_t* ht, _running_context_t* end) {
	_running_context_t* running = 0;
	_ht_node_t* scope = 0;

	mb_assert(s && ht);

	running = s->running_context;
	while(running && running != end) {
		scope = running->var_dict;
		if(scope) {
			_HT_FOREACH(scope, _do_nothing_on_object, _gc_add_reachable, ht);
		}
		running = running->prev;
	}
}

/* Alive marker functor of a value */
void _gc_alive_marker(mb_interpreter_t* s, void* h, mb_value_t val) {
	_ht_node_t* ht = 0;
	_object_t obj;
	mb_unrefvar(s);

	ht = (_ht_node_t*)h;
	_MAKE_NIL(&obj);
	_public_value_to_internal_object(&val, &obj);
	_gc_add_reachable(&obj, 0, h);
}

/* Destroy only the capsule (wrapper) of an object, leave the data behind, and add it to GC if possible */
int _gc_destroy_garbage_in_list(void* data, void* extra, _gc_t* gc) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	obj = (_object_t*)data;
	_ADDGC(obj, gc, false)
	safe_free(obj);

	return result;
}

/* Destroy only the capsule (wrapper) of an object, leave the data behind, deal with extra as well, and add it to GC if possible */
int _gc_destroy_garbage_in_dict(void* data, void* extra, _gc_t* gc) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;

	mb_assert(data);

	obj = (_object_t*)data;
	_ADDGC(obj, gc, false)
	safe_free(obj);

	obj = (_object_t*)extra;
	_ADDGC(obj, gc, false)
	safe_free(obj);

	return result;
}

#ifdef MB_ENABLE_CLASS
/* Destroy only the capsule (wrapper) of an object, leave the data behind, deal with extra as well, and add it to GC if possible */
int _gc_destroy_garbage_in_class(void* data, void* extra, _gc_t* gc) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	obj = (_object_t*)data;
	if(_IS_VAR(obj)) {
		_gc_destroy_garbage_in_class(obj->data.variable->data, 0, gc);
		safe_free(obj->data.variable->name);
		safe_free(obj->data.variable);
	} else {
		_ADDGC(obj, gc, true)
	}
	safe_free(obj);

	return result;
}
#endif /* MB_ENABLE_CLASS */

#ifdef MB_ENABLE_LAMBDA
/* Destroy only the capsule (wrapper) of an object, leave the data behind, deal with extra as well, and add it to GC if possible */
int _gc_destroy_garbage_in_lambda(void* data, void* extra, _gc_t* gc) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	obj = (_object_t*)data;
	if(_IS_VAR(obj)) {
#ifdef MB_ENABLE_CLASS
		if(_is_string(obj) && obj->data.variable->pathing) {
			safe_free(obj->data.variable->data);
		} else {
			_gc_destroy_garbage_in_lambda(obj->data.variable->data, 0, gc);
		}
#else /* MB_ENABLE_CLASS */
		_gc_destroy_garbage_in_lambda(obj->data.variable->data, 0, gc);
#endif /* MB_ENABLE_CLASS */
		safe_free(obj->data.variable->name);
		safe_free(obj->data.variable);
	} else {
		_ADDGC(obj, gc, false)
	}
	safe_free(obj);

	return result;
}

/* Collect garbage of outer scopes */
void _gc_destroy_garbage_in_outer_scope(_running_context_ref_t* p, _gc_t* gc) {
	while(p) {
		_running_context_ref_t* scope = p;
		p = p->prev;
		_HT_FOREACH(scope->scope->var_dict, _do_nothing_on_object, _gc_destroy_garbage_in_lambda, gc);
		_ht_clear(scope->scope->var_dict);
	}
}
#endif /* MB_ENABLE_LAMBDA */

/* Collect a garbage */
int _gc_destroy_garbage(void* data, void* extra, _gc_t* gc) {
	int result = _OP_RESULT_NORMAL;
	_ref_t* ref = 0;
	bool_t proc = true;
#ifdef MB_ENABLE_COLLECTION_LIB
	_list_t* lst = 0;
	_dict_t* dct = 0;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_LAMBDA
	_routine_t* routine = 0;
	_running_context_ref_t* outs = 0;
#endif /* MB_ENABLE_LAMBDA */

	mb_assert(data && extra);

	ref = (_ref_t*)extra;
	if(_ht_find(gc->collected_table, ref)) {
		proc = true;

		goto _exit;
	}
	switch(ref->type) {
#ifdef MB_ENABLE_COLLECTION_LIB
	case _DT_LIST:
		lst = (_list_t*)data;
		_LS_FOREACH(lst->list, _do_nothing_on_object, _gc_destroy_garbage_in_list, gc);
		_ls_clear(lst->list);
		lst->count = 0;

		break;
	case _DT_DICT:
		dct = (_dict_t*)data;
		_HT_FOREACH(dct->dict, _do_nothing_on_object, _gc_destroy_garbage_in_dict, gc);
		_ht_clear(dct->dict);

		break;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_LAMBDA
	case _DT_ROUTINE:
		routine = (_routine_t*)data;
		if(routine->type == MB_RT_LAMBDA) {
			_HT_FOREACH(routine->func.lambda.scope->var_dict, _do_nothing_on_object, _gc_destroy_garbage_in_lambda, gc);
			_ht_clear(routine->func.lambda.scope->var_dict);
			outs = routine->func.lambda.outer_scope;
			if(outs) {
				if(!_ht_find(gc->collected_table, &outs->ref))
					_gc_add(&outs->ref, outs, gc);
				routine->func.lambda.outer_scope = 0;
			}
			while(outs) {
				if(_ht_find(gc->collected_table, &outs->ref))
					break;
				_HT_FOREACH(outs->scope->var_dict, _do_nothing_on_object, _remove_this_lambda_from_upvalue, routine);
				outs = outs->prev;
			}
		}

		break;
#endif /* MB_ENABLE_LAMBDA */
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF: /* Fall through */
#endif /* MB_ENABLE_USERTYPE_REF */
	case _DT_ARRAY: /* Do nothing */
		break;
	default:
		proc = false;

		break;
	}
	if(proc && ref->count)
		_unref(ref, data);

_exit:
	if(proc)
		result = _OP_RESULT_DEL_NODE;

	return result;
}

#ifdef MB_ENABLE_CLASS
/* Collect a class instance */
int _gc_destroy_garbage_class(void* data, void* extra, _gc_t* gc) {
	int result = _OP_RESULT_NORMAL;
	_ref_t* ref = 0;
	bool_t proc = true;
	_class_t* instance = 0;

	mb_assert(data && extra);

	ref = (_ref_t*)extra;
	if(_ht_find(gc->collected_table, ref)) {
		proc = true;

		goto _exit;
	}
	switch(ref->type) {
	case _DT_CLASS:
		instance = (_class_t*)data;
		_HT_FOREACH(instance->scope->var_dict, _do_nothing_on_object, _gc_destroy_garbage_in_class, gc);
		_ht_clear(instance->scope->var_dict);
		_ls_clear(instance->meta_list);
#ifdef MB_ENABLE_LAMBDA
		if(instance->scope->refered_lambdas) {
			_ls_destroy(instance->scope->refered_lambdas);
			instance->scope->refered_lambdas = 0;
		}
#endif /* MB_ENABLE_LAMBDA */

		break;
	default:
		proc = false;

		break;
	}
	if(proc && ref->count)
		_unref(ref, data);

_exit:
	if(proc)
		result = _OP_RESULT_DEL_NODE;

	return result;
}
#endif /* MB_ENABLE_CLASS */

#ifdef MB_ENABLE_LAMBDA
/* Collect an outer scope */
int _gc_destroy_garbage_outer_scope(void* data, void* extra, _gc_t* gc) {
	int result = _OP_RESULT_NORMAL;
	_ref_t* ref = 0;
	bool_t proc = true;
	_running_context_ref_t* outs = 0;

	mb_assert(data && extra);

	ref = (_ref_t*)extra;
	if(_ht_find(gc->collected_table, ref)) {
		proc = true;

		goto _exit;
	}
	switch(ref->type) {
	case _DT_OUTER_SCOPE:
		outs = (_running_context_ref_t*)data;
		if(!_ht_find(gc->collected_table, &outs->ref))
			_gc_destroy_garbage_in_outer_scope(outs, gc);

		break;
	default:
		proc = false;

		break;
	}
	if(proc && ref->count)
		_unref(ref, data);

_exit:
	if(proc)
		result = _OP_RESULT_DEL_NODE;

	return result;
}
#endif /* MB_ENABLE_LAMBDA */

/* Swap active garbage table and recursive table */
void _gc_swap_tables(mb_interpreter_t* s) {
	_ht_node_t* tmp = 0;

	mb_assert(s);

	tmp = s->gc.table;
	s->gc.table = s->gc.recursive_table;
	s->gc.recursive_table = tmp;
}

/* Try trigger garbage collection */
void _gc_try_trigger(mb_interpreter_t* s) {
	if(!_GCNOW(s))
		return;

	if(_ht_count(s->gc.table) >= MB_GC_GARBAGE_THRESHOLD)
		_gc_collect_garbage(s, 1);
}

/* Collect all garbage */
void _gc_collect_garbage(mb_interpreter_t* s, int depth) {
#ifdef MB_ENABLE_FORK
	mb_interpreter_t* src = 0;
#endif /* MB_ENABLE_FORK */
	_ht_node_t* valid = 0;
	_gc_t* gc = 0;

	mb_assert(s);

#ifdef MB_ENABLE_FORK
	while(mb_get_forked_from(s, &src) == MB_FUNC_OK)
		s = src;
#endif /* MB_ENABLE_FORK */

	gc = &s->gc;

	/* Check whether it's paused */
	if(gc->disabled)
		return;

	/* Prepare for GC */
	if(gc->collecting)
		return;
	gc->collecting++;
	_PREPAREGC(s, gc);

	/* Get reachable information */
	valid = _ht_create(0, _ht_cmp_ref, _ht_hash_ref, _do_nothing_on_object);
	if(depth != -1)
		gc->valid_table = valid;
	_gc_get_reachable(s, valid, 0);
#ifdef MB_ENABLE_FORK
	if(s->all_forked) {
		_LS_FOREACH(s->all_forked, _do_nothing_on_object, _gc_get_reachable_in_forked, valid);
	}
#endif /* MB_ENABLE_FORK */
	if(s->alive_check_handler)
		s->alive_check_handler(s, valid, _gc_alive_marker);

	/* Get unreachable information */
	_HT_FOREACH(valid, _do_nothing_on_object, _ht_remove_existing, gc->table);
	_HT_FOREACH(valid, _do_nothing_on_object, _ht_remove_existing, gc->recursive_table);

	/* Collect garbage */
	_PREVGC(s, gc);
	do {
#ifdef MB_ENABLE_CLASS
		_HT_FOREACH(gc->table, _do_nothing_on_object, _gc_destroy_garbage_class, &s->gc);
#endif /* MB_ENABLE_CLASS */
		_HT_FOREACH(gc->table, _do_nothing_on_object, _gc_destroy_garbage, &s->gc);
#ifdef MB_ENABLE_LAMBDA
		_HT_FOREACH(gc->table, _do_nothing_on_object, _gc_destroy_garbage_outer_scope, &s->gc);
#endif /* MB_ENABLE_LAMBDA */
		_ht_clear(gc->table);
		if(gc->collecting > 1)
			gc->collecting--;

		if(!depth || !_ht_count(gc->recursive_table))
			break;

		_gc_swap_tables(s);
		gc->collecting++;
	} while(1);
	_POSTGC(s, gc);

	/* Tidy */
	_ht_clear(gc->collected_table);
	gc->valid_table = 0;
	_ht_clear(valid);
	_ht_destroy(valid);
	gc->collecting--;
	mb_assert(!gc->collecting);
}
