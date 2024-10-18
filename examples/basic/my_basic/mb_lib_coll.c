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

#include "mb_lib_coll.h"

#include "mb_lib_core.h"
#include "mb_lib_std.h"

/** Collection lib */

#ifdef MB_ENABLE_COLLECTION_LIB

MBCONST static const _func_t _coll_libs[] = {
	{ _COLL_ID_LIST, _coll_list },
	{ _COLL_ID_DICT, _coll_dict },
	{ _COLL_ID_PUSH, _coll_push },
	{ _COLL_ID_POP, _coll_pop },
	{ _COLL_ID_BACK, _coll_back },
	{ _COLL_ID_INSERT, _coll_insert },
	{ _COLL_ID_SORT, _coll_sort },
	{ _COLL_ID_EXISTS, _coll_exists },
	{ _COLL_ID_INDEX_OF, _coll_index_of },
	{ _COLL_ID_REMOVE, _coll_remove },
	{ _COLL_ID_CLEAR, _coll_clear },
	{ _COLL_ID_CLONE, _coll_clone },
	{ _COLL_ID_TO_ARRAY, _coll_to_array },
	{ _COLL_ID_ITERATOR, _coll_iterator },
	{ _COLL_ID_MOVE_NEXT, _coll_move_next }
};

_MB_DEFINE_LIBRARY(coll)

/* Execute ranged FOR-IN-NEXT-routine */
int _execute_ranged_for_loop(mb_interpreter_t* s, _ls_node_t** l, _var_t* var_loop) {
	int result = MB_FUNC_ERR;
	_ls_node_t* ast = 0;
	_running_context_t* running = 0;
	_var_t* pathed_var = 0;
	_object_t* old_val = 0;
	_ref_t* old_val_gc = 0;
	_ls_node_t* to_node = 0;
	_object_t range;
	_object_t* range_ptr = 0;
	_list_it_t* lit = 0;
	_dict_it_t* dit = 0;
	_list_it_t* tlit = 0;
	_dict_it_t* tdit = 0;
	mb_value_t ref_val;
	mb_value_t ref_it;
#if defined MB_ENABLE_USERTYPE_REF || defined MB_ENABLE_CLASS
	mb_meta_status_e os = MB_MS_NONE;
#endif /* MB_ENABLE_USERTYPE_REF || MB_ENABLE_CLASS */

	mb_assert(s && l && var_loop);

	running = s->running_context;
#ifdef MB_ENABLE_CLASS
	if(var_loop->pathing)
		pathed_var = _search_var_in_scope_chain(s, var_loop, 0);
	if(pathed_var) {
		_UNREF(pathed_var->data)
		_MAKE_NIL(pathed_var->data);
	}
#endif /* MB_ENABLE_CLASS */
	if(!pathed_var)
		pathed_var = var_loop;
	old_val = pathed_var->data;
	if(_is_ref(old_val)) {
		old_val_gc = (_ref_t*)old_val->data.pointer;
		if(!_gc_remove(old_val_gc, (void*)old_val_gc, &old_val_gc->s->gc))
			old_val_gc = 0;
	}
	range_ptr = &range;
	_MAKE_NIL(range_ptr);
	mb_make_nil(ref_val);
	mb_make_nil(ref_it);

	ast = *l;
	ast = ast->next;
	ast = ast->next;
	if(!ast) {
		_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

	/* Get collection */
	result = _calc_expression(s, &ast, &range_ptr);
	if(result != MB_FUNC_OK)
		goto _exit;

	/* Create iterator */
	switch(range_ptr->type) {
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		_internal_object_to_public_value(range_ptr, &ref_val);
		os = _try_overridden(s, (void**)&ast, &ref_val, _COLL_ID_ITERATOR, MB_MF_COLL);
		if((os & MB_MS_DONE) != MB_MS_NONE && (os & MB_MS_RETURNED) != MB_MS_NONE)
			_swap_public_value(&ref_it, &running->intermediate_value);

		break;
#endif /* MB_ENABLE_USERTYPE_REF */
	case _DT_LIST:
		tlit = lit = _create_list_it(range_ptr->data.list, true);

		break;
	case _DT_DICT:
		tdit = dit = _create_dict_it(range_ptr->data.dict, true);

		break;
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		_internal_object_to_public_value(range_ptr, &ref_val);
		os = _try_overridden(s, (void**)&ast, &ref_val, _COLL_ID_ITERATOR, MB_MF_COLL);
		if((os & MB_MS_DONE) != MB_MS_NONE && (os & MB_MS_RETURNED) != MB_MS_NONE)
			_swap_public_value(&ref_it, &running->intermediate_value);

		break;
#endif /* MB_ENABLE_CLASS */
	default:
		_handle_error_on_obj(s, SE_RN_ITERABLE_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);

		break;
	}
	to_node = ast;
	switch(range_ptr->type) { _REF_COLL(range_ptr) _REF_USERTYPE_REF(range_ptr) default: /* Do nothing */ break; }

_to:
	ast = to_node;

	/* Move next */
#ifdef MB_ENABLE_USERTYPE_REF
	if(ref_it.type != MB_DT_NIL) {
		mb_value_t moved_next;
		mb_value_t curr_val;
		_object_t curr_obj;

		mb_make_nil(moved_next);
		mb_make_nil(curr_val);
		_MAKE_NIL(&curr_obj);

		/* Move next */
		os = _try_overridden(s, (void**)&ast, &ref_it, _COLL_ID_MOVE_NEXT, MB_MF_COLL);
		if((os & MB_MS_DONE) != MB_MS_NONE && (os & MB_MS_RETURNED) != MB_MS_NONE)
			_swap_public_value(&moved_next, &running->intermediate_value);

		if(moved_next.type == MB_DT_INT && moved_next.value.integer) {
			/* Get current value */
			os = _try_overridden(s, (void**)&ast, &ref_it, _STD_ID_GET, MB_MF_FUNC);
			if((os & MB_MS_DONE) != MB_MS_NONE && (os & MB_MS_RETURNED) != MB_MS_NONE)
				_swap_public_value(&curr_val, &running->intermediate_value);

			/* Assign loop variable */
			_public_value_to_internal_object(&curr_val, &curr_obj);
			pathed_var->data = &curr_obj;
			/* Keep looping */
			result = _common_keep_looping(s, &ast, var_loop);
			_UNREF(&curr_obj)
			if(result == MB_LOOP_BREAK) {
				result = MB_FUNC_OK;

				goto _exit;
			} else if(result != MB_FUNC_OK || result == MB_SUB_RETURN) {
				goto _exit;
			}

			goto _to;
		} else {
			/* End looping */
			result = _common_end_looping(s, &ast);

			goto _exit;
		}
	}
#endif /* MB_ENABLE_USERTYPE_REF */
	if(lit) lit = _move_list_it_next(lit);
	else if(dit) dit = _move_dict_it_next(dit);
	if((lit && _invalid_list_it(lit)) || (dit && _invalid_dict_it(dit))) {
		_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	if(!lit && !dit) {
		/* End looping */
		result = _common_end_looping(s, &ast);

		goto _exit;
	} else {
		/* Assign loop variable */
		if(lit && !lit->list->range_begin && lit->curr.node && lit->curr.node->data) {
			pathed_var->data = (_object_t*)lit->curr.node->data;
		} else if(lit && lit->list->range_begin) {
			_dispose_object(pathed_var->data);
			pathed_var->data->type = _DT_INT;
			pathed_var->data->data.integer = lit->curr.ranging;
		} else if(dit && dit->curr_node && dit->curr_node->extra) {
			pathed_var->data = (_object_t*)dit->curr_node->extra;
		}
		/* Keep looping */
		result = _common_keep_looping(s, &ast, var_loop);
		if(result == MB_LOOP_BREAK) {
			result = MB_FUNC_OK;

			goto _exit;
		} else if(result != MB_FUNC_OK || result == MB_SUB_RETURN) {
			goto _exit;
		}

		goto _to;
	}

_exit:
#ifdef MB_ENABLE_USERTYPE_REF
	if(ref_it.type != MB_DT_NIL) {
		_object_t it_obj;
		_MAKE_NIL(&it_obj);

		_public_value_to_internal_object(&ref_it, &it_obj);
		_UNREF(&it_obj)
	}
#endif /* MB_ENABLE_USERTYPE_REF */
	if(tlit) _destroy_list_it(tlit);
	else if(tdit) _destroy_dict_it(tdit);
	switch(range_ptr->type) { _UNREF_COLL(range_ptr) _UNREF_USERTYPE_REF(range_ptr) default: /* Do nothing */ break; }

	*l = ast;

	pathed_var->data = old_val;
	if(old_val_gc)
		_gc_add(old_val_gc, (void*)old_val_gc, &old_val_gc->s->gc);

	return result;
}

/* LIST statement */
int _coll_list(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;
	_list_t* coll = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	coll = _create_list(s);

	if(mb_has_arg(s, l)) {
		_ls_node_t* ast = 0;
		_object_t* obj = 0;
		mb_make_nil(arg);
		_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _error);
		ast = (_ls_node_t*)*l;
		if(ast) obj = (_object_t*)ast->data;
		if(arg.type == MB_DT_INT && obj && _IS_FUNC(obj, _core_to)) {
			/* Push a range of integer */
			int_t begin = arg.value.integer;
			int_t end = 0;
			int_t step = 0;
			ast = ast->next;
			_mb_check_mark_exit(mb_pop_int(s, (void**)&ast, &end), result, _error);
			step = sgn(end - begin);
			coll->range_begin = (int_t*)mb_malloc(sizeof(int_t));
			*coll->range_begin = begin;
			coll->count = end - begin + step;
			if(!coll->count) coll->count = 1;
			*l = ast;
		} else {
			/* Push arguments */
			if(!_push_list(coll, &arg, 0)) {
				_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON2(l), MB_FUNC_ERR, _error, result);
			}
			while(mb_has_arg(s, l)) {
				mb_make_nil(arg);
				_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _error);
				if(!_push_list(coll, &arg, 0)) {
					_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON2(l), MB_FUNC_ERR, _error, result);
				}
			}
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _error);

	arg.type = MB_DT_LIST;
	arg.value.list = coll;
	_mb_check_mark_exit(mb_push_value(s, l, arg), result, _error);

	while(0) {
_error:
		mb_make_nil(arg);
		mb_push_value(s, l, arg);
		_destroy_list(coll);
	}

	return result;
}

/* DICT statement */
int _coll_dict(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;
	mb_value_t val;
	_dict_t* coll = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	coll = _create_dict(s);

	while(mb_has_arg(s, l)) {
		mb_make_nil(arg);
		mb_make_nil(val);
		_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _error);
		_mb_check_mark_exit(mb_pop_value(s, l, &val), result, _error);
		if(!_set_dict(coll, &arg, &val, 0, 0)) {
			_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON2(l), MB_FUNC_ERR, _error, result);
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _error);

	arg.type = MB_DT_DICT;
	arg.value.dict = coll;
	_mb_check_mark_exit(mb_push_value(s, l, arg), result, _error);

	while(0) {
_error:
		mb_make_nil(arg);
		mb_push_value(s, l, arg);
		_destroy_dict(coll);
	}

	return result;
}

/* PUSH statement */
int _coll_push(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	mb_value_t arg;
	_object_t olst;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_PUSH, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		if(coll.type != MB_DT_LIST) {
			_handle_error_on_obj(s, SE_RN_LIST_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
		_MAKE_NIL(&olst);
		_public_value_to_internal_object(&coll, &olst);

		while(mb_has_arg(s, l)) {
			mb_make_nil(arg);
			_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);
			if(!_push_list(olst.data.list, &arg, 0)) {
				_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, coll), result, _exit);
	}

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* POP statement */
int _coll_pop(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	mb_value_t val;
	_object_t olst;
	_object_t ocoll;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(val);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_value(s, l, &coll));
	os = _try_overridden(s, l, &coll, _COLL_ID_POP, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		if(coll.type != MB_DT_LIST) {
			_assign_public_value(s, &coll, 0, true);
			_handle_error_on_obj(s, SE_RN_LIST_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		_MAKE_NIL(&olst);
		_public_value_to_internal_object(&coll, &olst);
		if(_pop_list(olst.data.list, &val, s)) {
			mb_check(mb_push_value(s, l, val));
			_MAKE_NIL(&ocoll);
			_public_value_to_internal_object(&val, &ocoll);
			_UNREF(&ocoll)

			_assign_public_value(s, &coll, 0, true);
		} else {
			mb_check(mb_push_value(s, l, val));

			_assign_public_value(s, &coll, 0, true);

			_handle_error_on_obj(s, SE_RN_EMPTY_COLLECTION, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);
		}
	} else {
		if((os & MB_MS_RETURNED) == MB_MS_NONE) {
			mb_check(mb_push_value(s, l, val));
		}
	}

	mb_check(mb_attempt_close_bracket(s, l));

_exit:
	return result;
}

/* BACK statement */
int _coll_back(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	mb_value_t val;
	_object_t olst;
	_object_t* oval = 0;
	_ls_node_t* node = 0;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(val);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_value(s, l, &coll));
	os = _try_overridden(s, l, &coll, _COLL_ID_BACK, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		if(coll.type != MB_DT_LIST) {
			_assign_public_value(s, &coll, 0, true);
			_handle_error_on_obj(s, SE_RN_LIST_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		_MAKE_NIL(&olst);
		_public_value_to_internal_object(&coll, &olst);
		node = _ls_back(olst.data.list->list);
		oval = node ? (_object_t*)node->data : 0;
		if(oval) {
			_internal_object_to_public_value(oval, &val);

			mb_check(mb_push_value(s, l, val));

			_assign_public_value(s, &coll, 0, true);
		} else {
			mb_check(mb_push_value(s, l, val));

			_assign_public_value(s, &coll, 0, true);

			_handle_error_on_obj(s, SE_RN_EMPTY_COLLECTION, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);
		}
	} else {
		if((os & MB_MS_RETURNED) == MB_MS_NONE) {
			mb_check(mb_push_value(s, l, val));
		}
	}

	mb_check(mb_attempt_close_bracket(s, l));

_exit:
	return result;
}

/* INSERT statement */
int _coll_insert(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	int_t idx = 0;
	mb_value_t arg;
	_object_t olst;
	_object_t* oval = 0;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_INSERT, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_pop_int(s, l, &idx), result, _exit);
		_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

		if(coll.type != MB_DT_LIST) {
			_handle_error_on_obj(s, SE_RN_LIST_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
		_MAKE_NIL(&olst);
		_public_value_to_internal_object(&coll, &olst);

		if(!_insert_list(olst.data.list, idx, &arg, &oval)) {
			if(oval)
				_destroy_object(oval, 0);

			_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		_mb_check_mark_exit(mb_push_value(s, l, coll), result, _exit);
	} else {
		if((os & MB_MS_RETURNED) == MB_MS_NONE) {
			_mb_check_mark_exit(mb_push_value(s, l, coll), result, _exit);
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* SORT statement */
int _coll_sort(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	_object_t olst;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_SORT, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		if(coll.type != MB_DT_LIST) {
			_handle_error_on_obj(s, SE_RN_LIST_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
		_MAKE_NIL(&olst);
		_public_value_to_internal_object(&coll, &olst);

		_sort_list(olst.data.list);
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, coll), result, _exit);
	}

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* EXISTS statement */
int _coll_exists(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	mb_value_t arg;
	_object_t ocoll;
	mb_value_t ret;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(arg);
	mb_make_nil(ret);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_EXISTS, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

		_MAKE_NIL(&ocoll);
		switch(coll.type) {
		case MB_DT_LIST:
			_public_value_to_internal_object(&coll, &ocoll);
			mb_make_bool(ret, _find_list(ocoll.data.list, &arg, 0));

			break;
		case MB_DT_DICT:
			_public_value_to_internal_object(&coll, &ocoll);
			mb_make_bool(ret, _find_dict(ocoll.data.dict, &arg, 0));

			break;
		default:
			_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
		_mb_check_mark_exit(mb_push_value(s, l, ret), result, _exit);
	} else {
		if((os & MB_MS_RETURNED) == MB_MS_NONE) {
			_mb_check_mark_exit(mb_push_value(s, l, coll), result, _exit);
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* INDEX_OF statement */
int _coll_index_of(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	int idx = 0;
	mb_value_t coll;
	_object_t ocoll;
	mb_value_t val;
	mb_value_t ret;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(val);
	mb_make_nil(ret);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_INDEX_OF, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		ret.type = MB_DT_UNKNOWN;
		_mb_check_mark_exit(mb_pop_value(s, l, &val), result, _exit);
		_MAKE_NIL(&ocoll);
		switch(coll.type) {
		case MB_DT_LIST:
			_public_value_to_internal_object(&coll, &ocoll);
			if(_find_list(ocoll.data.list, &val, &idx)) {
				mb_make_int(ret, (int_t)idx);
			}

			break;
		default:
			_handle_error_on_obj(s, SE_RN_LIST_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, ret), result, _exit);
	}

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* REMOVE statement */
int _coll_remove(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	int_t idx = 0;
	mb_value_t key;
	_object_t ocoll;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(key);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_REMOVE, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_MAKE_NIL(&ocoll);
		switch(coll.type) {
		case MB_DT_LIST:
			_public_value_to_internal_object(&coll, &ocoll);
			while(mb_has_arg(s, l)) {
				_mb_check_mark_exit(mb_pop_int(s, l, &idx), result, _exit);

				if(!_remove_at_list(ocoll.data.list, idx)) {
					_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
				}
			}

			break;
		case MB_DT_DICT:
			_public_value_to_internal_object(&coll, &ocoll);
			while(mb_has_arg(s, l)) {
				_mb_check_mark_exit(mb_pop_value(s, l, &key), result, _exit);

				if(!_remove_dict(ocoll.data.dict, &key)) {
					_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
				}
			}

			break;
		default:
			_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, coll), result, _exit);
	}

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* CLEAR statement */
int _coll_clear(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	_object_t ocoll;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_CLEAR, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_MAKE_NIL(&ocoll);
		switch(coll.type) {
		case MB_DT_LIST:
			_public_value_to_internal_object(&coll, &ocoll);
			_clear_list(ocoll.data.list);

			break;
		case MB_DT_DICT:
			_public_value_to_internal_object(&coll, &ocoll);
			_clear_dict(ocoll.data.dict);

			break;
		default:
			_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, coll), result, _exit);
	}

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* CLONE statement */
int _coll_clone(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	_object_t ocoll;
	_object_t otgt;
	mb_value_t ret;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(ret);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_MAKE_NIL(&ocoll);
	_MAKE_NIL(&otgt);
	switch(coll.type) {
#ifdef MB_ENABLE_USERTYPE_REF
	case MB_DT_USERTYPE_REF:
		_public_value_to_internal_object(&coll, &ocoll);
		_clone_usertype_ref(ocoll.data.usertype_ref, &otgt);
		_internal_object_to_public_value(&otgt, &ret);

		break;
#endif /* MB_ENABLE_USERTYPE_REF */
	case MB_DT_LIST:
		_public_value_to_internal_object(&coll, &ocoll);
		_clone_object(s, &ocoll, &otgt, false, true);
		ret.type = MB_DT_LIST;
		ret.value.list = otgt.data.list;

		break;
	case MB_DT_DICT:
		_public_value_to_internal_object(&coll, &ocoll);
		_clone_object(s, &ocoll, &otgt, false, true);
		ret.type = MB_DT_DICT;
		ret.value.dict = otgt.data.dict;

		break;
	default:
		_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}

	_mb_check_mark_exit(mb_push_value(s, l, ret), result, _exit);

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* TO_ARRAY statement */
int _coll_to_array(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	_object_t ocoll;
	_array_t* array = 0;
	_array_helper_t helper;
	mb_value_t ret;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(ret);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_MAKE_NIL(&ocoll);
	switch(coll.type) {
	case MB_DT_LIST:
		_public_value_to_internal_object(&coll, &ocoll);
		array = _create_array(s, mb_strdup("", 1), _DT_REAL);
		array->count = ocoll.data.list->count;
		array->dimension_count = 1;
		array->dimensions[0] = ocoll.data.list->count;
		_init_array(array);
		helper.s = s;
		helper.array = array;
		helper.index = 0;
		_LS_FOREACH(ocoll.data.list->list, _do_nothing_on_object, _copy_list_to_array, &helper);
		ret.type = MB_DT_ARRAY;
		ret.value.array = array;

		break;
	default:
		_handle_error_on_obj(s, SE_RN_LIST_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}

	_mb_check_mark_exit(mb_push_value(s, l, ret), result, _exit);

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* ITERATOR statement */
int _coll_iterator(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t coll;
	_object_t ocoll;
	_list_it_t* lit = 0;
	_dict_it_t* dit = 0;
	mb_value_t ret;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(coll);
	mb_make_nil(ret);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &coll), result, _exit);
	os = _try_overridden(s, l, &coll, _COLL_ID_ITERATOR, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_MAKE_NIL(&ocoll);
		switch(coll.type) {
		case MB_DT_LIST:
			_public_value_to_internal_object(&coll, &ocoll);
			lit = _create_list_it(ocoll.data.list, false);
			ret.type = MB_DT_LIST_IT;
			ret.value.list_it = lit;

			break;
		case MB_DT_DICT:
			_public_value_to_internal_object(&coll, &ocoll);
			dit = _create_dict_it(ocoll.data.dict, false);
			ret.type = MB_DT_DICT_IT;
			ret.value.dict_it = dit;

			break;
		default:
			_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, ret), result, _exit);
	}

_exit:
	_assign_public_value(s, &coll, 0, true);

	return result;
}

/* MOVE_NEXT statement */
int _coll_move_next(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t it;
	_object_t oit;
	mb_value_t ret;
	mb_meta_status_e os = MB_MS_NONE;
	_ls_node_t* ast = 0;

	mb_assert(s && l);

	mb_make_nil(it);
	mb_make_nil(ret);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	ast = (_ls_node_t*)*l;
	if(ast && _IS_FUNC(ast->data, _coll_iterator)) {
		_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
	_mb_check_mark_exit(mb_pop_value(s, l, &it), result, _exit);
	os = _try_overridden(s, l, &it, _COLL_ID_MOVE_NEXT, MB_MF_COLL);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_MAKE_NIL(&oit);
		switch(it.type) {
		case MB_DT_LIST_IT:
			_public_value_to_internal_object(&it, &oit);
			oit.data.list_it = _move_list_it_next(oit.data.list_it);
			if(_invalid_list_it(oit.data.list_it)) {
				_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			} else if(oit.data.list_it) {
				mb_make_bool(ret, true);
			} else {
				mb_make_nil(ret);
			}

			break;
		case MB_DT_DICT_IT:
			_public_value_to_internal_object(&it, &oit);
			if(oit.data.dict_it && oit.data.dict_it->curr_node == _INVALID_DICT_IT && _invalid_dict_it(oit.data.dict_it)) {
				_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}
			oit.data.dict_it = _move_dict_it_next(oit.data.dict_it);
			if(_invalid_dict_it(oit.data.dict_it)) {
				_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			} else if(oit.data.dict_it) {
				mb_make_bool(ret, true);
			} else {
				mb_make_nil(ret);
			}

			break;
		default:
			_assign_public_value(s, &it, 0, true);
			_handle_error_on_obj(s, SE_RN_ITERABLE_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, ret), result, _exit);
	}

_exit:
	return result;
}

/* Create a list */
_list_t* _create_list(mb_interpreter_t* s) {
	_list_t* result = 0;

	mb_assert(s);

	result = (_list_t*)mb_malloc(sizeof(_list_t));
	memset(result, 0, sizeof(_list_t));
	result->list = _ls_create();
	_create_ref(&result->ref, _unref_list, _DT_LIST, s);

	return result;
}

/* Destroy a list */
void _destroy_list(_list_t* c) {
	mb_assert(c);

	if(c->range_begin) { safe_free(c->range_begin); }
	_ls_foreach(c->list, _destroy_object);
	_ls_destroy(c->list);
	_destroy_ref(&c->ref);
	safe_free(c);
}

/* Create a dictionary */
_dict_t* _create_dict(mb_interpreter_t* s) {
	_dict_t* result = 0;

	mb_assert(s);

	result = (_dict_t*)mb_malloc(sizeof(_dict_t));
	memset(result, 0, sizeof(_dict_t));
	result->dict = _ht_create(0, _ht_cmp_object, _ht_hash_object, _destroy_object_with_extra);
	_create_ref(&result->ref, _unref_dict, _DT_DICT, s);

	return result;
}

/* Destroy a dictionary */
void _destroy_dict(_dict_t* c) {
	mb_assert(c);

	_ht_foreach(c->dict, _destroy_object_with_extra);
	_ht_destroy(c->dict);
	_destroy_ref(&c->ref);
	safe_free(c);
}

/* Create an iterator of a list */
_list_it_t* _create_list_it(_list_t* coll, bool_t lock) {
	_list_it_t* result = 0;

	mb_assert(coll);

	result = (_list_it_t*)mb_malloc(sizeof(_list_it_t));
	memset(result, 0, sizeof(_list_it_t));
	result->list = coll;
	result->locking = lock;
	if(coll->range_begin)
		result->curr.ranging = *coll->range_begin - sgn(coll->count);
	else
		result->curr.node = coll->list;
	if(lock)
		_lock_ref_object(&coll->lock, &coll->ref, coll);
	_weak_ref(&coll->ref, coll, &result->weak_ref);

	return result;
}

/* Destroy an iterator of a list */
bool_t _destroy_list_it(_list_it_t* it) {
	bool_t result = true;

	mb_assert(it);

	if(_weak_unref(&it->weak_ref))
		_unlock_ref_object(&it->list->lock, &it->list->ref, it->list);
	safe_free(it);

	return result;
}

/* Move an iterator of a list to next step */
_list_it_t* _move_list_it_next(_list_it_t* it) {
	_list_it_t* result = 0;

	if(!it || !it->list || !it->list->list)
		goto _exit;

	if(it->list->lock < 0) { /* The iterator goes invalid if collection has been changed after obtaining iterator */
		result = it;

		goto _exit;
	}

	if(!it->curr.node && !it->list->range_begin)
		goto _exit;

	if(it->list->range_begin) {
		if(it->list->lock)
			it->curr.ranging += sgn(it->list->count);

		if(it->list->count > 0 && it->curr.ranging < *it->list->range_begin + it->list->count)
			result = it;
		else if(it->list->count < 0 && it->curr.ranging > *it->list->range_begin + it->list->count)
			result = it;
	} else {
		if(it->list->lock)
			it->curr.node = it->curr.node->next;

		if(it->curr.node)
			result = it;
	}

_exit:
	return result;
}

/* Create an iterator of a dictionary */
_dict_it_t* _create_dict_it(_dict_t* coll, bool_t lock) {
	_dict_it_t* result = 0;

	mb_assert(coll);

	result = (_dict_it_t*)mb_malloc(sizeof(_dict_it_t));
	memset(result, 0, sizeof(_dict_it_t));
	result->dict = coll;
	result->locking = lock;
	result->curr_bucket = 0;
	result->curr_node = _INVALID_DICT_IT;
	if(lock)
		_lock_ref_object(&coll->lock, &coll->ref, coll);
	_weak_ref(&coll->ref, coll, &result->weak_ref);

	return result;
}

/* Destroy an iterator of a dictionary */
bool_t _destroy_dict_it(_dict_it_t* it) {
	bool_t result = true;

	mb_assert(it);

	if(_weak_unref(&it->weak_ref))
		_unlock_ref_object(&it->dict->lock, &it->dict->ref, it->dict);
	safe_free(it);

	return result;
}

/* Move an iterator of a dictionary to next step */
_dict_it_t* _move_dict_it_next(_dict_it_t* it) {
	_dict_it_t* result = 0;

	if(!it || !it->dict || !it->dict->dict || !it->curr_node)
		goto _exit;

	if(!it->dict->lock)
		goto _exit;

	if(it->curr_node && it->curr_node != _INVALID_DICT_IT) {
		it->curr_node = it->curr_node->next;
		if(!it->curr_node)
			++it->curr_bucket;
	}
	if(!it->curr_node || it->curr_node == _INVALID_DICT_IT) {
		if(!it->dict->dict->array)
			goto _exit;
		for( ; it->curr_bucket < it->dict->dict->array_size; ++it->curr_bucket) {
			it->curr_node = it->dict->dict->array[it->curr_bucket];
			if(it->curr_node && it->curr_node->next) {
				it->curr_node = it->curr_node->next;

				break;
			}
		}
	}

	if(it->curr_node && it->curr_node->extra)
		result = it;

_exit:
	return result;
}

/* Unreference a list */
void _unref_list(_ref_t* ref, void* data) {
	mb_assert(ref);

	if(*ref->count == _NONE_REF)
		_destroy_list((_list_t*)data);
}

/* Unreference a dictionary */
void _unref_dict(_ref_t* ref, void* data) {
	mb_assert(ref);

	if(*ref->count == _NONE_REF)
		_destroy_dict((_dict_t*)data);
}

/* Push a value to a list */
bool_t _push_list(_list_t* coll, mb_value_t* val, _object_t* oarg) {
	mb_assert(coll && (val || oarg));

	if(_try_purge_it(coll->ref.s, val, oarg))
		return false;

	_fill_ranged(coll);

	if(val && !oarg)
		_create_internal_object_from_public_value(val, &oarg);
	_COLL_ROUTINE(oarg);
	_ls_pushback(coll->list, oarg);
	coll->count++;

	_write_on_ref_object(&coll->lock, &coll->ref, coll);
	_invalidate_list_cache(coll);

	return true;
}

/* Pop a value from a list */
bool_t _pop_list(_list_t* coll, mb_value_t* val, mb_interpreter_t* s) {
	_object_t* oval = 0;

	mb_assert(coll && val && s);

	_fill_ranged(coll);

	oval = (_object_t*)_ls_popback(coll->list);
	if(oval) {
		_internal_object_to_public_value(oval, val);
		_destroy_object_capsule_only(oval, 0);
		coll->count--;

		if(val->type == MB_DT_STRING)
			_mark_lazy_destroy_string(s, val->value.string);

		_write_on_ref_object(&coll->lock, &coll->ref, coll);
		_invalidate_list_cache(coll);

		return true;
	} else {
		mb_make_nil(*val);

		return false;
	}
}

/* Insert a value into a list */
bool_t _insert_list(_list_t* coll, int_t idx, mb_value_t* val, _object_t** oval) {
	_object_t* oarg = 0;

	mb_assert(coll && val);

	if(_try_purge_it(coll->ref.s, val, oval ? *oval : 0))
		return false;

	_fill_ranged(coll);

	_create_internal_object_from_public_value(val, &oarg);
	_COLL_ROUTINE(oarg);
	if(oval)
		*oval = oarg;

	if(_ls_insert_at(coll->list, (int)idx, oarg)) {
		coll->count++;
		_write_on_ref_object(&coll->lock, &coll->ref, coll);
		_invalidate_list_cache(coll);

		return true;
	}

	return false;
}

/* Set an element in a list with a specific index with a given value */
bool_t _set_list(_list_t* coll, int_t idx, mb_value_t* val, _object_t** oval) {
	_ls_node_t* result = 0;
	_object_t* oarg = 0;

	mb_assert(coll && (val || (oval && *oval)));

	if(_try_purge_it(coll->ref.s, val, oval ? *oval : 0))
		return false;

	_fill_ranged(coll);

	result = _node_at_list(coll, (int)idx);
	if(result) {
		if(result->data)
			_destroy_object(result->data, 0);
		if(val) {
			_create_internal_object_from_public_value(val, &oarg);
			_COLL_ROUTINE(oarg);
			if(oval)
				*oval = oarg;
		} else {
			oarg = *oval;
		}
		result->data = oarg;

		_write_on_ref_object(&coll->lock, &coll->ref, coll);
		_invalidate_list_cache(coll);
	}

	return !!(result && result->data);
}

/* Remove an element in a list with a specific index */
bool_t _remove_at_list(_list_t* coll, int_t idx) {
	bool_t result = false;
	_ls_node_t* node = 0;

	mb_assert(coll);

	_fill_ranged(coll);

	node = _node_at_list(coll, (int)idx);
	if(node) {
		if(node->data) {
			_ls_remove(coll->list, node, _destroy_object);
			coll->count--;

			_write_on_ref_object(&coll->lock, &coll->ref, coll);
			_invalidate_list_cache(coll);

			result = true;
		}
	}

	return result;
}

/* Get a node in a list with a specific index */
_ls_node_t* _node_at_list(_list_t* coll, int index) {
	_ls_node_t* result = 0;
	_ls_node_t* tmp = 0;
	int n = 0;

	mb_assert(coll);

	_fill_ranged(coll);

	if(index >= 0 && index < (int)coll->count) {
		/* Position: HEAD ... LEFT ... PIVOT ... RIGHT ... TAIL
			PIVOT is a cached node and,
			LEN(HEAD to LEFT) == LEN(LEFT to PIVOT) && LEN(PIVOT to RIGHT) == LEN(RIGHT to TAIL)
		 */
		int head = 0,
			left = coll->cached_index / 2,
			right = coll->cached_index + (coll->count - coll->cached_index) / 2,
			tail = coll->count - 1;
		if(coll->cached_node) {
			if(index >= head && index < left) { /* [HEAD, LEFT) */
				n = index;
				tmp = coll->list->next;
				while(tmp && n) {
					tmp = tmp->next;
					--n;
				}
				if(tmp) {
					result = tmp;
					coll->cached_node = tmp;
					coll->cached_index = index;
				}
			} else if(index >= left && index <= right) { /* [LEFT, RIGHT] */
				while(index != coll->cached_index) {
					if(index > coll->cached_index) {
						coll->cached_node = coll->cached_node->next;
						coll->cached_index++;
					} else if(index < coll->cached_index) {
						coll->cached_node = coll->cached_node->prev;
						coll->cached_index--;
					}
				}
				result = coll->cached_node;
			} else if(index > right && index <= tail) { /* (RIGHT, TAIL] */
				n = tail - index;
				tmp = coll->list->prev;
				while(tmp && n) {
					tmp = tmp->prev;
					--n;
				}
				if(tmp) {
					result = tmp;
					coll->cached_node = tmp;
					coll->cached_index = index;
				}
			} else {
				mb_assert(0 && "Impossible.");
			}
		} else {
			n = index;
			tmp = coll->list->next;
			while(tmp && n) {
				tmp = tmp->next;
				--n;
			}
			if(tmp) {
				result = tmp;
				coll->cached_node = tmp;
				coll->cached_index = index;
			}
		}
	}

	return result;
}

/* Get the value in a list with a specific index */
bool_t _at_list(_list_t* coll, int_t idx, mb_value_t* oval) {
	_ls_node_t* result = 0;

	mb_assert(coll && oval);

	_fill_ranged(coll);

	result = _node_at_list(coll, (int)idx);
	if(oval && result && result->data)
		_internal_object_to_public_value((_object_t*)result->data, oval);

	return !!(result && result->data);
}

/* Find a value in a list */
bool_t _find_list(_list_t* coll, mb_value_t* val, int* idx) {
	bool_t result = false;
	_object_t* oarg = 0;

	mb_assert(coll && val);

	_fill_ranged(coll);

	_create_internal_object_from_public_value(val, &oarg);
	result = !!_ls_find(coll->list, oarg, (_ls_compare_t)_ht_cmp_object, idx);
	_destroy_object(oarg, 0);

	return result;
}

/* Clear a list */
void _clear_list(_list_t* coll) {
	mb_assert(coll);

	if(coll->range_begin) { safe_free(coll->range_begin); }

	_ls_foreach(coll->list, _destroy_object);
	_ls_clear(coll->list);
	coll->count = 0;

	_write_on_ref_object(&coll->lock, &coll->ref, coll);
	_invalidate_list_cache(coll);
}

/* Sort a list */
void _sort_list(_list_t* coll) {
	mb_assert(coll);

	_ls_sort(&coll->list, (_ls_compare_t)_ht_cmp_object);

	_write_on_ref_object(&coll->lock, &coll->ref, coll);
	_invalidate_list_cache(coll);
}

/* Invalidate cached list index */
void _invalidate_list_cache(_list_t* coll) {
	mb_assert(coll);

	coll->cached_node = 0;
	coll->cached_index = 0;
}

/* Fill a ranged list with numbers */
void _fill_ranged(_list_t* coll) {
	_object_t* obj = 0;

	mb_assert(coll);

	if(coll->range_begin) {
		mb_value_t arg;
		int_t begin = *coll->range_begin;
		int_t end = *coll->range_begin + coll->count;
		int_t step = sgn(coll->count);

		do {
			mb_make_int(arg, begin);
			_create_internal_object_from_public_value(&arg, &obj);
			_ls_pushback(coll->list, obj);

			begin += step;
		} while(begin != end);
		safe_free(coll->range_begin);

		_write_on_ref_object(&coll->lock, &coll->ref, coll);
		_invalidate_list_cache(coll);
	}
}

/* Set an element to a dictionary with a key-value pair */
bool_t _set_dict(_dict_t* coll, mb_value_t* key, mb_value_t* val, _object_t* okey, _object_t* oval) {
	_ls_node_t* exists = 0;

	mb_assert(coll && (key || okey) && (val || oval));

	if(_try_purge_it(coll->ref.s, key, okey))
		return false;
	if(_try_purge_it(coll->ref.s, val, oval))
		return false;

	if(key && !okey)
		_create_internal_object_from_public_value(key, &okey);
	if(val && !oval)
		_create_internal_object_from_public_value(val, &oval);
	exists = _ht_find(coll->dict, okey);
	if(exists)
		_ht_remove(coll->dict, okey, _ls_cmp_extra_object);
	_COLL_ROUTINE(okey);
	_COLL_ROUTINE(oval);
	_ht_set_or_insert(coll->dict, okey, oval);

	_write_on_ref_object(&coll->lock, &coll->ref, coll);

	return true;
}

/* Remove an element to a dictionary with a specific key */
bool_t _remove_dict(_dict_t* coll, mb_value_t* key) {
	_ls_node_t* result = 0;
	_object_t* okey = 0;

	mb_assert(coll && key);

	_create_internal_object_from_public_value(key, &okey);

	if(_try_purge_it(coll->ref.s, key, okey)) {
		_destroy_object_capsule_only(okey, 0);

		return false;
	}

	result = _ht_find(coll->dict, okey);
	if(result && result->data) {
		_ht_remove(coll->dict, okey, _ls_cmp_extra_object);
		_destroy_object(okey, 0);

		_write_on_ref_object(&coll->lock, &coll->ref, coll);

		return true;
	}

	return false;
}

/* Find a key in a dictionary */
bool_t _find_dict(_dict_t* coll, mb_value_t* val, mb_value_t* oval) {
	_ls_node_t* result = 0;
	_object_t* oarg = 0;

	mb_assert(coll && val);

	if(val->type == MB_DT_LIST_IT || val->type == MB_DT_DICT_IT)
		return false;
	_create_internal_object_from_public_value(val, &oarg);
	result = _ht_find(coll->dict, oarg);
	_destroy_object(oarg, 0);
	if(oval) {
		if(result && result->data) {
			_internal_object_to_public_value((_object_t*)result->data, oval);
		} else {
			oval->type = MB_DT_UNKNOWN;
			oval->value.integer = 0;

			return true;
		}
	}

	return !!(result && result->data);
}

/* Clear a dictionary */
void _clear_dict(_dict_t* coll) {
	mb_assert(coll);

	_ht_foreach(coll->dict, _destroy_object_with_extra);
	_ht_clear(coll->dict);

	_write_on_ref_object(&coll->lock, &coll->ref, coll);
}

/* Determin whether a list iterator is invalid */
bool_t _invalid_list_it(_list_it_t* it) {
	if(!it)
		return false;

	return it && it->list && it->list->lock <= 0;
}

/* Determin whether a dictionary iterator is invalid */
bool_t _invalid_dict_it(_dict_it_t* it) {
	if(!it)
		return false;

	return it && it->dict && it->dict->lock <= 0;
}

/* Assign an iterator to another object */
bool_t _assign_with_it(_object_t* tgt, _object_t* src) {
	mb_assert(tgt && src);

	if(src->type != _DT_LIST_IT && src->type != _DT_DICT_IT)
		return false;

	switch(src->type) {
	case _DT_LIST_IT:
		if(src->data.list_it->locking) {
			tgt->data.list_it = _create_list_it(src->data.list_it->list, true);
		} else {
			tgt->data = src->data;
			tgt->data.list_it->locking = true;
			_lock_ref_object(&tgt->data.list_it->list->lock, &tgt->data.list_it->list->ref, tgt->data.list_it->list);
		}

		break;
	case _DT_DICT_IT:
		if(src->data.dict_it->locking) {
			tgt->data.dict_it = _create_dict_it(src->data.dict_it->dict, true);
		} else {
			tgt->data = src->data;
			tgt->data.dict_it->locking = true;
			_lock_ref_object(&tgt->data.dict_it->dict->lock, &tgt->data.dict_it->dict->ref, tgt->data.dict_it->dict);
		}

		break;
	default:
		mb_assert(0 && "Impossible.");

		break;
	}

	return true;
}

/* Try to purege an iterator */
bool_t _try_purge_it(mb_interpreter_t* s, mb_value_t* val, _object_t* obj) {
	bool_t result = false;
	_object_t tmp;

	mb_assert(s && (val || obj));

	_MAKE_NIL(&tmp);
#ifdef MB_ENABLE_COLLECTION_LIB
	if(val) {
		switch(val->type) {
		case MB_DT_LIST_IT: /* Fall through */
		case MB_DT_DICT_IT:
			_public_value_to_internal_object(val, &tmp);
			obj = &tmp;

			break;
		default: /* Do nothing */
			break;
		}
	}
	if(obj) {
		if(obj->type == _DT_LIST_IT) {
			result = true;
			if(obj->data.list_it->locking)
				return result;

			_destroy_list_it(obj->data.list_it); /* Process dangling value */
		} else if(obj->type == _DT_DICT_IT) {
			result = true;
			if(obj->data.dict_it->locking)
				return result;

			_destroy_dict_it(obj->data.dict_it); /* Process dangling value */
		} else {
			return result;
		}
	}
#endif /* MB_ENABLE_COLLECTION_LIB */

	return result;
}

/* Clone an object to a list */
int _clone_to_list(void* data, void* extra, _list_t* coll) {
	_object_t* obj = 0;
	_object_t* tgt = 0;
	mb_unrefvar(extra);

	mb_assert(data && coll);

	_fill_ranged(coll);

	tgt = _create_object();
	obj = (_object_t*)data;
	_clone_object(coll->ref.s, obj, tgt, false, false);
	_push_list(coll, 0, tgt);
	_REF(tgt)

	return 1;
}

/* Clone a key-value pair to a dictionary */
int _clone_to_dict(void* data, void* extra, _dict_t* coll) {
	_object_t* kobj = 0;
	_object_t* ktgt = 0;
	_object_t* vobj = 0;
	_object_t* vtgt = 0;

	mb_assert(data && extra && coll);

	ktgt = _create_object();
	kobj = (_object_t*)extra;
	_clone_object(coll->ref.s, kobj, ktgt, false, false);

	vtgt = _create_object();
	vobj = (_object_t*)data;
	_clone_object(coll->ref.s, vobj, vtgt, false, false);

	_set_dict(coll, 0, 0, ktgt, vtgt);
	_REF(ktgt)
	_REF(vtgt)

	return 1;
}

/* Copy an object from a list to an array */
int _copy_list_to_array(void* data, void* extra, _array_helper_t* h) {
	_object_t* obj = 0;
	mb_value_t val;
	_data_e type = _DT_NIL;
	mb_unrefvar(extra);

	mb_assert(data && h);

	obj = (_object_t*)data;
	mb_make_nil(val);
	_internal_object_to_public_value(obj, &val);
	type = obj->type;
	_set_array_elem(h->s, 0, h->array, h->index++, &val.value, &type);

	return 1;
}

/* Copy all keys of a dictionary to a value array */
int _copy_keys_to_value_array(void* data, void* extra, _keys_helper_t* h) {
	_object_t* obj = 0;
	mb_value_t val;
	mb_unrefvar(data);

	mb_assert(extra && h);

	if(h->index >= h->size)
		return 1;

	obj = (_object_t*)extra;
	mb_make_nil(val);
	_internal_object_to_public_value(obj, &val);
	h->keys[h->index++] = val;

	return 1;
}
#endif /* MB_ENABLE_COLLECTION_LIB */

/* Initialize a collection */
int mb_init_coll(struct mb_interpreter_t* s, void** l, mb_value_t* coll) {
	int result = MB_FUNC_OK;

	if(!s || !coll) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

#ifdef MB_ENABLE_COLLECTION_LIB
	switch(coll->type) {
	case MB_DT_LIST:
		coll->value.list = _create_list(s);

		break;
	case MB_DT_DICT:
		coll->value.dict = _create_dict(s);

		break;
	default:
		_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(coll);

	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_COLLECTION_LIB */

_exit:
	return result;
}

/* Get an element of a collection */
int mb_get_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t idx, mb_value_t* val) {
	int result = MB_FUNC_OK;
	_object_t ocoll;
	int_t i = 0;
	mb_value_t ret;

	mb_make_nil(ret);

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	_MAKE_NIL(&ocoll);
#ifdef MB_ENABLE_COLLECTION_LIB
	switch(coll.type) {
	case MB_DT_LIST:
		mb_int_val(idx, i);
		_public_value_to_internal_object(&coll, &ocoll);
		if(!_at_list(ocoll.data.list, i, &ret)) {
			_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		break;
	case MB_DT_DICT:
		_public_value_to_internal_object(&coll, &ocoll);
		if(!_find_dict(ocoll.data.dict, &idx, &ret)) {
			_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		break;
	default:
		_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(idx);
	mb_unrefvar(coll);
	mb_unrefvar(i);

	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_COLLECTION_LIB */

_exit:
	if(val) *val = ret;

	return result;
}

/* Set an element of a collection */
int mb_set_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t idx, mb_value_t val) {
	int result = MB_FUNC_OK;
	_object_t ocoll;
	int_t i = 0;
	_object_t* oval = 0;
	mb_value_t ret;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(ret);

	_MAKE_NIL(&ocoll);
#ifdef MB_ENABLE_COLLECTION_LIB
	switch(coll.type) {
	case MB_DT_LIST:
		mb_int_val(idx, i);
		_public_value_to_internal_object(&coll, &ocoll);
		while((int)ocoll.data.list->count <= i)
			_push_list(ocoll.data.list, &ret, 0);
		if(!_set_list(ocoll.data.list, i, &val, &oval)) {
			if(oval)
				_destroy_object(oval, 0);

			_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		break;
	case MB_DT_DICT:
		_public_value_to_internal_object(&coll, &ocoll);
		_set_dict(ocoll.data.dict, &idx, &val, 0, 0);

		break;
	default:
		_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(val);
	mb_unrefvar(idx);
	mb_unrefvar(coll);
	mb_unrefvar(oval);
	mb_unrefvar(i);

	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_COLLECTION_LIB */

_exit:
	return result;
}

/* Remove an element from a collection */
int mb_remove_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t idx) {
	int result = MB_FUNC_OK;
	_object_t ocoll;
	int_t i = 0;
	mb_value_t ret;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(ret);

	_MAKE_NIL(&ocoll);
#ifdef MB_ENABLE_COLLECTION_LIB
	switch(coll.type) {
	case MB_DT_LIST:
		mb_int_val(idx, i);
		_public_value_to_internal_object(&coll, &ocoll);
		if(!_remove_at_list(ocoll.data.list, i)) {
			_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		break;
	case MB_DT_DICT:
		_public_value_to_internal_object(&coll, &ocoll);
		if(!_remove_dict(ocoll.data.dict, &idx)) {
			_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		break;
	default:
		_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(coll);
	mb_unrefvar(idx);
	mb_unrefvar(i);

	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_COLLECTION_LIB */

_exit:
	return result;
}

/* Tell the element count of a collection */
int mb_count_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, int* c) {
	int result = MB_FUNC_OK;
	_object_t ocoll;
#ifdef MB_ENABLE_COLLECTION_LIB
	_list_t* lst = 0;
	_dict_t* dct = 0;
#endif /* MB_ENABLE_COLLECTION_LIB */
	int ret = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	_MAKE_NIL(&ocoll);
#ifdef MB_ENABLE_COLLECTION_LIB
	switch(coll.type) {
	case MB_DT_LIST:
		lst = (_list_t*)coll.value.list;
		ret = (int)lst->count;

		break;
	case MB_DT_DICT:
		dct = (_dict_t*)coll.value.dict;
		ret = (int)_ht_count(dct->dict);

		break;
	default:
		_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(coll);

	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_COLLECTION_LIB */

_exit:
	if(c) *c = ret;

	return result;
}

/* Get all keys of a collection */
int mb_keys_of_coll(struct mb_interpreter_t* s, void** l, mb_value_t coll, mb_value_t* keys, int c) {
	int result = MB_FUNC_OK;
	_object_t ocoll;
#ifdef MB_ENABLE_COLLECTION_LIB
	_list_t* lst = 0;
	_dict_t* dct = 0;
	int i = 0;
	_keys_helper_t helper;
#endif /* MB_ENABLE_COLLECTION_LIB */

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	_MAKE_NIL(&ocoll);
#ifdef MB_ENABLE_COLLECTION_LIB
	switch(coll.type) {
	case MB_DT_LIST:
		lst = (_list_t*)coll.value.list;
		for(i = 0; i < c && i < (int)lst->count; ++i) {
			mb_make_int(keys[i], i);
		}

		break;
	case MB_DT_DICT:
		dct = (_dict_t*)coll.value.dict;
		helper.keys = keys;
		helper.size = c;
		helper.index = 0;
		_HT_FOREACH(dct->dict, _do_nothing_on_object, _copy_keys_to_value_array, &helper);

		break;
	default:
		_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(coll);
	mb_unrefvar(keys);
	mb_unrefvar(c);

	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_COLLECTION_LIB */

_exit:
	return result;
}
