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

#include "mb_lib_core.h"

#include "mb_lib_coll.h"
#include "mb_lib_coll.h"

/** Core lib */

MBCONST static const _func_t _core_libs[] = {
	{ _DUMMY_ASSIGN_CHAR, _core_dummy_assign },
	{ _CORE_ID_ADD, _core_add },
	{ _CORE_ID_MIN, _core_min },
	{ _CORE_ID_MUL, _core_mul },
	{ _CORE_ID_DIV, _core_div },
	{ _CORE_ID_MOD, _core_mod },
	{ _CORE_ID_POW, _core_pow },
	{ _CORE_ID_LBRA, _core_open_bracket },
	{ _CORE_ID_RBRA, _core_close_bracket },
	{ _CORE_ID_NEG, _core_neg },

	{ _CORE_ID_EQU, _core_equal },
	{ _CORE_ID_LT, _core_less },
	{ _CORE_ID_GT, _core_greater },
	{ _CORE_ID_LTEQ, _core_less_equal },
	{ _CORE_ID_GTEQ, _core_greater_equal },
	{ _CORE_ID_NEQ, _core_not_equal },

	{ _CORE_ID_AND, _core_and },
	{ _CORE_ID_OR, _core_or },
	{ _CORE_ID_NOT, _core_not },

	{ _CORE_ID_IS, _core_is },

	{ _CORE_ID_LET, _core_let },
	{ _CORE_ID_DIM, _core_dim },

	{ _CORE_ID_IF, _core_if },
	{ _CORE_ID_THEN, _core_then },
	{ _CORE_ID_ELSEIF, _core_elseif },
	{ _CORE_ID_ELSE, _core_else },
	{ _CORE_ID_ENDIF, _core_endif },

	{ _CORE_ID_FOR, _core_for },
#ifdef MB_ENABLE_COLLECTION_LIB
	{ _CORE_ID_IN, _core_in },
#endif /* MB_ENABLE_COLLECTION_LIB */
	{ _CORE_ID_TO, _core_to },
	{ _CORE_ID_STEP, _core_step },
	{ _CORE_ID_NEXT, _core_next },
	{ _CORE_ID_WHILE, _core_while },
	{ _CORE_ID_WEND, _core_wend },
	{ _CORE_ID_DO, _core_do },
	{ _CORE_ID_UNTIL, _core_until },

	{ _CORE_ID_EXIT, _core_exit },
	{ _CORE_ID_GOTO, _core_goto },
	{ _CORE_ID_GOSUB, _core_gosub },
	{ _CORE_ID_RETURN, _core_return },

	{ _CORE_ID_CALL, _core_call },
	{ _CORE_ID_DEF, _core_def },
	{ _CORE_ID_ENDDEF, _core_enddef },
	{ _VAR_ARGS_STR, _core_args },

#ifdef MB_ENABLE_CLASS
	{ _CORE_ID_CLASS, _core_class },
	{ _CORE_ID_ENDCLASS, _core_endclass },
	{ _CORE_ID_NEW, _core_new },
	{ _CORE_ID_VAR, _core_var },
	{ _CORE_ID_REFLECT, _core_reflect },
#endif /* MB_ENABLE_CLASS */

#ifdef MB_ENABLE_LAMBDA
	{ _CORE_ID_LAMBDA, _core_lambda },
#ifdef MB_LAMBDA_ALIAS
	{ MB_LAMBDA_ALIAS, _core_lambda },
#endif /* MB_LAMBDA_ALIAS */
#endif /* MB_ENABLE_LAMBDA */

#ifdef MB_ENABLE_ALLOC_STAT
	{ _CORE_ID_MEM, _core_mem },
#endif /* MB_ENABLE_ALLOC_STAT */

	{ _CORE_ID_TYPE, _core_type },
	{ _CORE_ID_IMPORT, _core_import },
	{ _CORE_ID_END, _core_end }
};

_MB_DEFINE_LIBRARY(core)

/* Operator #, dummy assignment */
int _core_dummy_assign(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* Operator + */
int _core_add(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_ONCALC(s, l, add, result, _exit);
	_instruct_obj_meta_obj(s, l, add, result, _exit);
	if(_is_string(((_tuple3_t*)*l)->e1) || _is_string(((_tuple3_t*)*l)->e2)) {
		if(_is_string(((_tuple3_t*)*l)->e1) && _is_string(((_tuple3_t*)*l)->e2)) {
			_instruct_connect_strings(l);
		} else {
			_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, TON(l), MB_FUNC_ERR, _exit, result);
		}
	} else {
		_instruct_num_op_num(+, l);
	}

_exit:
	return result;
}

/* Operator - (minus) */
int _core_min(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_ONCALC(s, l, sub, result, _exit);
	_instruct_obj_meta_obj(s, l, sub, result, _exit);
	_instruct_num_op_num(-, l);

	goto _exit; /* Avoid an unreferenced label warning */

_exit:
	return result;
}

/* Operator * */
int _core_mul(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_ONCALC(s, l, mul, result, _exit);
	_instruct_obj_meta_obj(s, l, mul, result, _exit);
	_instruct_num_op_num(*, l);

	goto _exit; /* Avoid an unreferenced label warning */

_exit:
	return result;
}

/* Operator / */
int _core_div(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_ONCALC(s, l, div, result, _exit);
	_instruct_obj_meta_obj(s, l, div, result, _exit);
	_proc_div_by_zero(s, l, real_t, _exit, result, SE_RN_DIVIDE_BY_ZERO);
	_instruct_num_op_num(/, l);

_exit:
	return result;
}

/* Operator MOD */
int _core_mod(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_proc_div_by_zero(s, l, int_t, _exit, result, SE_RN_DIVIDE_BY_ZERO);
	_instruct_int_op_int(%, l);

_exit:
	return result;
}

/* Operator ^ */
int _core_pow(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_instruct_fun_num_num(pow, l);

	return result;
}

/* Operator ( */
int _core_open_bracket(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* Operator ) */
int _core_close_bracket(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

#ifdef MB_ENABLE_LAMBDA
	if(s->last_routine && s->last_routine->type == MB_RT_LAMBDA) {
		result = _core_return(s, l);

		goto _exit;
	}
#endif /* MB_ENABLE_LAMBDA */

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* Operator - (negative) */
int _core_neg(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	mb_value_t arg;
	_running_context_t* running = 0;
	int* inep = 0;
	int calc_depth = 0;
#ifdef MB_ENABLE_USERTYPE_REF
	_object_t obj;
#endif /* MB_ENABLE_USERTYPE_REF */

	mb_assert(s && l);

	running = s->running_context;
	ast = (_ls_node_t*)*l;
	if(ast) ast = ast->next;

	if(!_ls_empty(s->in_neg_expr))
		inep = (int*)_ls_back(s->in_neg_expr)->data;

	if(inep)
		(*inep)++;

	calc_depth = running->calc_depth;

	mb_make_nil(arg);
	if(ast && _IS_FUNC((_object_t*)ast->data, _core_open_bracket)) {
		mb_check(mb_attempt_open_bracket(s, l));

		mb_check(mb_pop_value(s, l, &arg));

		mb_check(mb_attempt_close_bracket(s, l));
	} else {
		running->calc_depth = 1;

		mb_check(mb_attempt_func_begin(s, l));

		mb_check(mb_pop_value(s, l, &arg));

		mb_check(mb_attempt_func_end(s, l));
	}

	if(inep)
		(*inep)--;

	_ONNEG(s, l, arg, result, _exit);
	switch(arg.type) {
	case MB_DT_INT:
		arg.value.integer = -(arg.value.integer);

		break;
	case MB_DT_REAL:
		arg.value.float_point = -(arg.value.float_point);

		break;
#ifdef MB_ENABLE_USERTYPE_REF
	case MB_DT_USERTYPE_REF:
		_MAKE_NIL(&obj);
		_public_value_to_internal_object(&arg, &obj);
		if(obj.data.usertype_ref->calc_operators && obj.data.usertype_ref->calc_operators->neg) {
			mb_meta_operator_t neg = obj.data.usertype_ref->calc_operators->neg;
			mb_check(mb_ref_value(s, l, arg));
			mb_check(mb_unref_value(s, l, arg));
			mb_check(neg(s, l, &arg, 0, &arg));

			break;
		}
		/* Fall through */
#endif /* MB_ENABLE_USERTYPE_REF */
	default:
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, TON(l), MB_FUNC_WARNING, _exit, result);

		break;
	}

_exit:
	mb_check(mb_push_value(s, l, arg));

	running->calc_depth = calc_depth;

	return result;
}

/* Operator = (equal) */
int _core_equal(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_tuple3_t* tpr = 0;

	mb_assert(s && l);

	if(_is_string(((_tuple3_t*)*l)->e1) || _is_string(((_tuple3_t*)*l)->e2)) {
		if(_is_string(((_tuple3_t*)*l)->e1) && _is_string(((_tuple3_t*)*l)->e2)) {
			_instruct_compare_strings(==, l);
		} else if(_is_nil(((_tuple3_t*)*l)->e1) || _is_nil(((_tuple3_t*)*l)->e2)) {
			tpr = (_tuple3_t*)*l;
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = false;
		} else {
			_set_tuple3_result(l, 0);
			_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, TON(l), MB_FUNC_WARNING, _exit, result);
		}
	} else if(_is_number(((_tuple3_t*)*l)->e1) && _is_number(((_tuple3_t*)*l)->e2)) {
		_instruct_num_op_num(==, l);
		tpr = (_tuple3_t*)*l;
		if(((_object_t*)tpr->e3)->type != _DT_INT) {
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = ((_object_t*)tpr->e3)->data.float_point != 0.0f;
		}
	} else {
		_instruct_obj_op_obj(==, l);
	}

_exit:
	return result;
}

/* Operator < */
int _core_less(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_tuple3_t* tpr = 0;

	mb_assert(s && l);

	if(_is_string(((_tuple3_t*)*l)->e1) || _is_string(((_tuple3_t*)*l)->e2)) {
		if(_is_string(((_tuple3_t*)*l)->e1) && _is_string(((_tuple3_t*)*l)->e2)) {
			_instruct_compare_strings(<, l);
		} else {
			if(_is_string(((_tuple3_t*)*l)->e1)) {
				_set_tuple3_result(l, 0);
			} else {
				_set_tuple3_result(l, 1);
			}
			_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, TON(l), MB_FUNC_WARNING, _exit, result);
		}
	} else if(_is_number(((_tuple3_t*)*l)->e1) && _is_number(((_tuple3_t*)*l)->e2)) {
		_instruct_num_op_num(<, l);
		tpr = (_tuple3_t*)*l;
		if(((_object_t*)tpr->e3)->type != _DT_INT) {
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = ((_object_t*)tpr->e3)->data.float_point != 0.0f;
		}
	} else {
		_instruct_obj_op_obj(<, l);
	}

_exit:
	return result;
}

/* Operator > */
int _core_greater(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_tuple3_t* tpr = 0;

	mb_assert(s && l);

	if(_is_string(((_tuple3_t*)*l)->e1) || _is_string(((_tuple3_t*)*l)->e2)) {
		if(_is_string(((_tuple3_t*)*l)->e1) && _is_string(((_tuple3_t*)*l)->e2)) {
			_instruct_compare_strings(>, l);
		} else {
			if(_is_string(((_tuple3_t*)*l)->e1)) {
				_set_tuple3_result(l, 1);
			} else {
				_set_tuple3_result(l, 0);
			}
			_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, TON(l), MB_FUNC_WARNING, _exit, result);
		}
	} else if(_is_number(((_tuple3_t*)*l)->e1) && _is_number(((_tuple3_t*)*l)->e2)) {
		_instruct_num_op_num(>, l);
		tpr = (_tuple3_t*)*l;
		if(((_object_t*)tpr->e3)->type != _DT_INT) {
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = ((_object_t*)tpr->e3)->data.float_point != 0.0f;
		}
	} else {
		_instruct_obj_op_obj(>, l);
	}

_exit:
	return result;
}

/* Operator <= */
int _core_less_equal(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_tuple3_t* tpr = 0;

	mb_assert(s && l);

	if(_is_string(((_tuple3_t*)*l)->e1) || _is_string(((_tuple3_t*)*l)->e2)) {
		if(_is_string(((_tuple3_t*)*l)->e1) && _is_string(((_tuple3_t*)*l)->e2)) {
			_instruct_compare_strings(<=, l);
		} else {
			if(_is_string(((_tuple3_t*)*l)->e1)) {
				_set_tuple3_result(l, 0);
			} else {
				_set_tuple3_result(l, 1);
			}
			_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, TON(l), MB_FUNC_WARNING, _exit, result);
		}
	} else if(_is_number(((_tuple3_t*)*l)->e1) && _is_number(((_tuple3_t*)*l)->e2)) {
		_instruct_num_op_num(<=, l);
		tpr = (_tuple3_t*)*l;
		if(((_object_t*)tpr->e3)->type != _DT_INT) {
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = ((_object_t*)tpr->e3)->data.float_point != 0.0f;
		}
	} else {
		_instruct_obj_op_obj(<=, l);
	}

_exit:
	return result;
}

/* Operator >= */
int _core_greater_equal(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_tuple3_t* tpr = 0;

	mb_assert(s && l);

	if(_is_string(((_tuple3_t*)*l)->e1) || _is_string(((_tuple3_t*)*l)->e2)) {
		if(_is_string(((_tuple3_t*)*l)->e1) && _is_string(((_tuple3_t*)*l)->e2)) {
			_instruct_compare_strings(>=, l);
		} else {
			if(_is_string(((_tuple3_t*)*l)->e1)) {
				_set_tuple3_result(l, 1);
			} else {
				_set_tuple3_result(l, 0);
			}
			_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, TON(l), MB_FUNC_WARNING, _exit, result);
		}
	} else if(_is_number(((_tuple3_t*)*l)->e1) && _is_number(((_tuple3_t*)*l)->e2)) {
		_instruct_num_op_num(>=, l);
		tpr = (_tuple3_t*)*l;
		if(((_object_t*)tpr->e3)->type != _DT_INT) {
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = ((_object_t*)tpr->e3)->data.float_point != 0.0f;
		}
	} else {
		_instruct_obj_op_obj(>=, l);
	}

_exit:
	return result;
}

/* Operator <> */
int _core_not_equal(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_tuple3_t* tpr = 0;

	mb_assert(s && l);

	if(_is_string(((_tuple3_t*)*l)->e1) || _is_string(((_tuple3_t*)*l)->e2)) {
		if(_is_string(((_tuple3_t*)*l)->e1) && _is_string(((_tuple3_t*)*l)->e2)) {
			_instruct_compare_strings(!=, l);
		} else if(_is_nil(((_tuple3_t*)*l)->e1) || _is_nil(((_tuple3_t*)*l)->e2)) {
			tpr = (_tuple3_t*)*l;
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = true;
		} else {
			_set_tuple3_result(l, 1);
			_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, TON(l), MB_FUNC_WARNING, _exit, result);
		}
	} else if(_is_number(((_tuple3_t*)*l)->e1) && _is_number(((_tuple3_t*)*l)->e2)) {
		_instruct_num_op_num(!=, l);
		tpr = (_tuple3_t*)*l;
		if(((_object_t*)tpr->e3)->type != _DT_INT) {
			((_object_t*)tpr->e3)->type = _DT_INT;
			((_object_t*)tpr->e3)->data.integer = ((_object_t*)tpr->e3)->data.float_point != 0.0f;
		}
	} else {
		_instruct_obj_op_obj(!=, l);
	}

_exit:
	return result;
}

/* Operator AND */
int _core_and(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_instruct_bool_op_bool(&&, l);

	return result;
}

/* Operator OR */
int _core_or(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	_instruct_bool_op_bool(||, l);

	return result;
}

/* Operator NOT */
int _core_not(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	mb_value_t arg;
	mb_value_t ret;
	_running_context_t* running = 0;
	int calc_depth = 0;

	mb_assert(s && l);

	running = s->running_context;
	ast = (_ls_node_t*)*l;
	if(ast) ast = ast->next;

	calc_depth = running->calc_depth;

	mb_make_nil(arg);
	mb_make_nil(ret);
	if(ast && _IS_FUNC((_object_t*)ast->data, _core_open_bracket)) {
		mb_check(mb_attempt_open_bracket(s, l));

		mb_check(mb_pop_value(s, l, &arg));

		mb_check(mb_attempt_close_bracket(s, l));
	} else {
		running->calc_depth = 1;

		mb_check(mb_attempt_func_begin(s, l));

		mb_check(mb_pop_value(s, l, &arg));

		mb_check(mb_attempt_func_end(s, l));
	}
	_ONCOND(s, 0, &arg);

	switch(arg.type) {
	case MB_DT_NIL:
		mb_make_bool(ret, true);

		break;
	case MB_DT_INT:
		mb_make_bool(ret, !arg.value.integer);

		break;
	case MB_DT_REAL:
		mb_make_bool(ret, arg.value.float_point == (real_t)0);

		break;
	default:
		mb_make_bool(ret, false);

		break;
	}
	_assign_public_value(s, &arg, 0, true);
	mb_check(mb_push_int(s, l, ret.value.integer));

	running->calc_depth = calc_depth;

	return result;
}

/* Operator IS */
int _core_is(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_object_t* fst = 0;
	_object_t* scd = 0;
	_object_t* val = 0;
	bool_t is_a = 0;

	mb_assert(s && l);

	_instruct_obj_meta_obj(s, l, is, result, _exit);

	fst = (_object_t*)((_tuple3_t*)*l)->e1;
	scd = (_object_t*)((_tuple3_t*)*l)->e2;
	val = (_object_t*)((_tuple3_t*)*l)->e3;

	if(_IS_VAR(fst)) fst = fst->data.variable->data;
	if(_IS_VAR(scd)) scd = scd->data.variable->data;
	if(!fst || !scd) {
		_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, TON(l), MB_FUNC_ERR, _exit, result);
	}
	if(scd->type == _DT_TYPE) {
		val->type = _DT_INT;
		val->data.integer = (int_t)(!!(_internal_type_to_public_type(fst->type) & scd->data.type));
	} else {
#ifdef MB_ENABLE_CLASS
		if(!_IS_CLASS(fst) || !_IS_CLASS(scd)) {
			_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, TON(l), MB_FUNC_ERR, _exit, result);
		}
		_traverse_class(fst->data.instance, 0, _is_a_class, _META_LIST_MAX_DEPTH, true, scd->data.instance, &is_a);
		val->type = _DT_INT;
		val->data.integer = (int_t)is_a;
#else /* MB_ENABLE_CLASS */
		mb_unrefvar(is_a);

		_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, TON(l), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_CLASS */
	}

_exit:
	return result;
}

/* LET statement */
int _core_let(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_running_context_t* running = 0;
	_var_t* var = 0;
	_var_t* evar = 0;
	int refc = 1;
	_array_t* arr = 0;
	_object_t* arr_obj = 0;
	unsigned arr_idx = 0;
	bool_t literally = false;
	_object_t* val = 0;
#ifdef MB_ENABLE_COLLECTION_LIB
	int_t idx = 0;
	mb_value_t key;
	bool_t is_coll = false;
#endif /* MB_ENABLE_COLLECTION_LIB */

	mb_assert(s && l);

	running = s->running_context;

	ast = (_ls_node_t*)*l;
	obj = (_object_t*)ast->data;
	if(obj->type == _DT_FUNC)
		ast = ast->next;
	if(!ast || !ast->data) {
		_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	obj = (_object_t*)ast->data;
#ifdef MB_ENABLE_CLASS
	if(_IS_VAR(obj)) {
		_ls_node_t* fn = 0;
		if(_is_valid_class_accessor_following_routine(s, obj->data.variable, ast, &fn)) {
			if(fn)
				obj = (_object_t*)fn->data;
		}
	}
#endif /* MB_ENABLE_CLASS */
	if(obj->type == _DT_ARRAY) {
		arr_obj = obj;
		arr = _search_array_in_scope_chain(s, obj->data.array, &arr_obj);
		result = _get_array_index(s, &ast, 0, &arr_idx, &literally);
		if(result != MB_FUNC_OK)
			goto _exit;
	} else if(obj->type == _DT_VAR && obj->data.variable->data->type == _DT_ARRAY) {
		arr_obj = obj->data.variable->data;
		arr = _search_array_in_scope_chain(s, obj->data.variable->data->data.array, &arr_obj);
		result = _get_array_index(s, &ast, 0, &arr_idx, &literally);
		if(result != MB_FUNC_OK)
			goto _exit;
	} else if(obj->type == _DT_VAR) {
		if(_IS_ME(obj->data.variable)) {
			_handle_error_on_obj(s, SE_RN_CANNOT_ASSIGN_TO_RESERVED_WORD, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		} else {
			evar = obj->data.variable;
			var = _search_var_in_scope_chain(s, obj->data.variable, 0);
			if(var == evar) evar = 0;
#ifdef MB_ENABLE_CLASS
			if(evar && evar->pathing == _PATHING_UPVALUE) evar = 0;
#endif /* MB_ENABLE_CLASS */
			if(var == _OBJ_BOOL_TRUE->data.variable || var == _OBJ_BOOL_FALSE->data.variable) {
				_handle_error_on_obj(s, SE_RN_CANNOT_ASSIGN_TO_RESERVED_WORD, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
		}
	} else {
		_handle_error_on_obj(s, SE_RN_CANNOT_ASSIGN_TO_RESERVED_WORD, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

	ast = ast->next;
	if(!ast || !ast->data) {
		_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
#ifdef MB_ENABLE_COLLECTION_LIB
	if(var && _IS_COLL(var->data)) {
		obj = (_object_t*)ast->data;
		if(_IS_FUNC(obj, _core_open_bracket)) {
			mb_check(mb_attempt_open_bracket(s, l));

			switch(var->data->type) {
			case _DT_LIST:
				mb_check(mb_pop_int(s, l, &idx));

				break;
			case _DT_DICT:
				mb_make_nil(key);
				mb_check(mb_pop_value(s, l, &key));

				break;
			default: /* Do nothing */
				break;
			}

			mb_check(mb_attempt_close_bracket(s, l));

			ast = (_ls_node_t*)*l;
			is_coll = true;
		}
	}
#endif /* MB_ENABLE_COLLECTION_LIB */
	obj = (_object_t*)ast->data;
	if(!_IS_FUNC(obj, _core_equal)) { /* Is it an assign operator? */
		_handle_error_on_obj(s, SE_RN_ASSIGN_OPERATOR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

	ast = ast->next;
	val = _create_object();
	result = _calc_expression(s, &ast, &val);

	if(var) {
#ifdef MB_ENABLE_COLLECTION_LIB
		if(is_coll) {
			switch(var->data->type) {
			case _DT_LIST:
				if(!_set_list(var->data->data.list, idx, 0, &val)) {
					safe_free(val);
					_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
				}

				break;
			case _DT_DICT:
				if(!_set_dict(var->data->data.dict, &key, 0, 0, val)) {
					safe_free(val);
					_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
				}

				break;
			default: /* Do nothing */
				break;
			}

			goto _exit;
		}
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
_proc_extra_var:
#endif /* MB_ENABLE_CLASS */
		_dispose_object(var->data);
		var->data->type = val->type;
#ifdef MB_ENABLE_COLLECTION_LIB
		if(val->type == _DT_LIST_IT || val->type == _DT_DICT_IT)
			_assign_with_it(var->data, val);
		else
			var->data->data = val->data;
#else /* MB_ENABLE_COLLECTION_LIB */
		var->data->data = val->data;
#endif /* MB_ENABLE_COLLECTION_LIB */
		if(val->type == _DT_ROUTINE) {
#ifdef MB_ENABLE_LAMBDA
			if(val->data.routine->type == MB_RT_LAMBDA)
				var->data->is_ref = val->is_ref;
			else
				var->data->is_ref = true;
#else /* MB_ENABLE_LAMBDA */
			var->data->is_ref = true;
#endif /* MB_ENABLE_LAMBDA */
#ifndef MB_ENABLE_ARRAY_REF
		} else if(val->type == _DT_ARRAY) {
			var->data->is_ref = true;
#endif /* MB_ENABLE_ARRAY_REF */
		} else {
			var->data->is_ref = val->is_ref;
		}
#ifdef MB_ENABLE_CLASS
		if(evar && evar->pathing) {
			if(var->data->type == _DT_STRING) {
				var->data->data.string = mb_strdup(var->data->data.string, strlen(var->data->data.string) + 1);
				var->data->is_ref = false;
			}
			var = evar;
			evar = 0;
			refc++;

			goto _proc_extra_var;
		}
#endif /* MB_ENABLE_CLASS */
	} else if(arr && literally) {
		if(val->type != _DT_UNKNOWN) {
			if(arr->name) {
				_destroy_object(val, 0);
				_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}
#ifdef MB_ENABLE_ARRAY_REF
			_unref(&arr_obj->data.array->ref, arr_obj->data.array);
#endif /* MB_ENABLE_ARRAY_REF */
			arr_obj->type = val->type;
#ifdef MB_ENABLE_COLLECTION_LIB
			if(val->type == _DT_LIST_IT || val->type == _DT_DICT_IT)
				_assign_with_it(arr_obj, val);
			else
				arr_obj->data = val->data;
#else /* MB_ENABLE_COLLECTION_LIB */
			arr_obj->data = val->data;
#endif /* MB_ENABLE_COLLECTION_LIB */
			arr_obj->is_ref = val->is_ref;
		}
	} else if(arr) {
		mb_value_u _val;
		switch(val->type) {
#ifdef MB_SIMPLE_ARRAY
		case _DT_INT:
			if(arr->type == _DT_STRING) goto _default;
			_val.integer = val->data.integer;

			break;
		case _DT_REAL:
			if(arr->type == _DT_STRING) goto _default;
			_val.float_point = val->data.float_point;

			break;
		case _DT_STRING:
			if(arr->type != _DT_STRING) goto _default;
			_val.string = val->data.string;

			break;
_default:
#else /* MB_SIMPLE_ARRAY */
		case _DT_NIL: /* Fall through */
		case _DT_UNKNOWN: /* Fall through */
		case _DT_INT: /* Fall through */
		case _DT_REAL: /* Fall through */
		case _DT_STRING: /* Fall through */
		case _DT_TYPE: /* Fall through */
		case _DT_USERTYPE:
			_copy_bytes(_val.bytes, val->data.bytes);

			break;
#endif /* MB_SIMPLE_ARRAY */
		default:
			_dispose_object(val);
			safe_free(val);
			_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
		result = _set_array_elem(s, ast, arr, arr_idx, &_val, &val->type);
		if(result != MB_FUNC_OK)
			goto _exit;
		if(val->type == _DT_STRING && !val->is_ref) {
			safe_free(val->data.string);
		}
	}
	while(refc--) {
		_REF(val)
	}
	safe_free(val);

_exit:
	*l = ast;

	return result;
}

/* DIM statement */
int _core_dim(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* arr = 0;
	_object_t* len = 0;
	mb_value_u val;
	_array_t dummy;

	mb_assert(s && l);

	/* Array name */
	ast = (_ls_node_t*)*l;
	if(!ast->next || ((_object_t*)ast->next->data)->type != _DT_ARRAY) {
		_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, (ast && ast->next) ? ((_object_t*)ast->next->data) : 0, MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;
	arr = (_object_t*)ast->data;
	memset(&dummy, 0, sizeof(_array_t));
	dummy.type = arr->data.array->type;
	dummy.name = arr->data.array->name;
	/* ( */
	if(!ast->next || ((_object_t*)ast->next->data)->type != _DT_FUNC || ((_object_t*)ast->next->data)->data.func->pointer != _core_open_bracket) {
		_handle_error_on_obj(s, SE_RN_OPEN_BRACKET_EXPECTED, s->source_file, (ast && ast->next) ? ((_object_t*)ast->next->data) : 0, MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;
	/* Array subscript */
	if(!ast->next) {
		_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, (ast && ast->next) ? ((_object_t*)ast->next->data) : 0, MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;
	while(((_object_t*)ast->data)->type != _DT_FUNC || ((_object_t*)ast->data)->data.func->pointer != _core_close_bracket) {
		/* Get an integer value */
		len = (_object_t*)ast->data;
		if(!_try_get_value(len, &val, _DT_INT)) {
			_handle_error_on_obj(s, SE_RN_UNEXPECTED_TYPE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		if(val.integer <= 0) {
			_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		if(dummy.dimension_count >= MB_MAX_DIMENSION_COUNT) {
			_handle_error_on_obj(s, SE_RN_TOO_MANY_DIMENSIONS, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		dummy.dimensions[dummy.dimension_count++] = (int)val.integer;
		if(dummy.count)
			dummy.count *= (unsigned)val.integer;
		else
			dummy.count += (unsigned)val.integer;
		ast = ast->next;
		/* Comma? */
		if(_IS_SEP(ast->data, ','))
			ast = ast->next;
	}
	/* Create or modify raw data */
	_clear_array(arr->data.array);
#ifdef MB_ENABLE_ARRAY_REF
	dummy.ref = arr->data.array->ref;
#endif /* MB_ENABLE_ARRAY_REF */
	*(arr->data.array) = dummy;
	_init_array(arr->data.array);
	if(!arr->data.array->raw) {
		arr->data.array->dimension_count = 0;
		arr->data.array->dimensions[0] = 0;
		arr->data.array->count = 0;
		_handle_error_on_obj(s, SE_RN_OUT_OF_MEMORY, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

_exit:
	*l = ast;

	return result;
}

/* IF statement */
int _core_if(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* val = 0;
	_object_t* obj = 0;
	bool_t multi_line = false;
	bool_t skip = false;
	_running_context_t* running = 0;

	mb_assert(s && l);

	running = s->running_context;

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	val = _create_object();

_elseif:
	_MAKE_NIL(val);
	result = _calc_expression(s, &ast, &val);
	_ONCOND(s, val, 0);
	_REF(val)
	if(result != MB_FUNC_OK)
		goto _exit;
	if(!ast)
		goto _exit;

	obj = (_object_t*)ast->data;
	if(val->data.integer) {
		skip = true;

		if(!_IS_FUNC(obj, _core_then)) {
			if(ast->prev && _IS_FUNC(ast->prev->data, _core_then)) {
				ast = ast->prev;
			} else {
				_handle_error_on_obj(s, SE_RN_THEN_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
		}

		if(ast && ast->next && _IS_EOS(ast->next->data)) {
			multi_line = true;
		} else {
			if(!s->jump_set || (s->jump_set & _JMP_INS))
				s->skip_to_eoi = _ls_back(s->sub_stack);
		}
		do {
			ast = ast->next;
			while(ast && _IS_EOS(ast->data))
				ast = ast->next;
			if(ast && _IS_FUNC(ast->data, _core_endif)) {
				ast = ast->prev;

				break;
			}
			if(!ast)
				break;
			if(multi_line && (_IS_FUNC(ast->data, _core_else) || _IS_FUNC(ast->data, _core_elseif)))
				break;
			result = _execute_statement(s, &ast, true);
			if(result != MB_FUNC_OK)
				goto _exit;
			if(ast) {
				obj = (_object_t*)ast->data;
				if(obj->type == _DT_PREV_IMPORT || obj->type == _DT_POST_IMPORT)
					_execute_statement(s, &ast, true);
				else
					ast = ast->prev;
			}
		} while(ast && (
				(!multi_line && _IS_SEP(ast->data, ':')) || (
					multi_line && ast->next && (
						!_IS_FUNC(ast->next->data, _core_elseif) &&
						!_IS_FUNC(ast->next->data, _core_else) &&
						!_IS_FUNC(ast->next->data, _core_endif)
					)
				)
			)
		);

		if(!ast)
			goto _exit;

		obj = (_object_t*)ast->data;
		if(!_IS_EOS(obj)) {
			s->skip_to_eoi = 0;
			result = _skip_to(s, &ast, 0, _DT_EOS);
			if(result != MB_FUNC_OK)
				goto _exit;
		}
	} else {
		if(ast && ast->next && _IS_EOS(ast->next->data)) {
			multi_line = true;

			result = _skip_if_chunk(s, &ast);
			if(result != MB_FUNC_OK)
				goto _exit;
		}
		if(multi_line && ast && _IS_FUNC(ast->data, _core_elseif)) {
			ast = ast->next;

			goto _elseif;
		}
		if(multi_line && ast && _IS_FUNC(ast->data, _core_endif))
			goto _exit;

		result = _skip_to(s, &ast, _core_else, _DT_EOS);
		if(result != MB_FUNC_OK)
			goto _exit;

		obj = (_object_t*)ast->data;
		if(!_IS_EOS(obj)) {
			skip = true;

			if(!_IS_FUNC(obj, _core_else)) {
				_handle_error_on_obj(s, SE_RN_ELSE_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}

			do {
				_ls_node_t* la = 0;
				la = ast = ast->next;
				while(ast && _IS_EOS(ast->data)) {
					ast = ast->next;
					if(ast) la = ast;
				}
				if(!ast) {
					mb_get_last_error(s, 0, 0, 0, 0);
					_handle_error_on_obj(s, SE_RN_ENDIF_EXPECTED, s->source_file, DON(la), MB_FUNC_ERR, _exit, result);
				}
				if(ast && _IS_FUNC(ast->data, _core_endif)) {
					ast = ast->prev;

					break;
				}
				result = _execute_statement(s, &ast, true);
				if(result != MB_FUNC_OK)
					goto _exit;
				if(ast)
					ast = ast->prev;
			} while(ast && (
					(!multi_line && _IS_SEP(ast->data, ':')) ||
					(multi_line && !_IS_FUNC(ast->next->data, _core_endif))
				)
			);
		}
	}

_exit:
	if(result == MB_SUB_RETURN) {
		if(ast)
			ast = ast->prev;
	} else {
		if(multi_line) {
			int ret = MB_FUNC_OK;
			if(skip)
				ret = _skip_struct(s, &ast, _core_if, _core_then, _core_endif);
			if(result != MB_FUNC_END && result != MB_LOOP_BREAK && result != MB_LOOP_CONTINUE && result != MB_SUB_RETURN) {
				if(ret != MB_FUNC_OK)
					result = ret;
			}
		}
	}

	*l = ast;

	switch(val->type) {
#ifdef MB_ENABLE_COLLECTION_LIB
	case _DT_LIST_IT: /* Fall through */
	case _DT_DICT_IT:
		_destroy_object_capsule_only(val, 0);

		break;
#endif /* MB_ENABLE_COLLECTION_LIB */
	case _DT_UNKNOWN: /* Do nothing */
		break;
	default:
		_destroy_object(val, 0);

		break;
	}

	return result;
}

/* THEN statement */
int _core_then(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* ELSEIF statement */
int _core_elseif(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* ELSE statement */
int _core_else(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* ENDIF statement */
int _core_endif(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* FOR statement */
int _core_for(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_var_t* var_loop = 0;

	mb_assert(s && l);

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	obj = (_object_t*)ast->data;
	if(!_IS_VAR(obj)) {
		_handle_error_on_obj(s, SE_RN_LOOP_VAR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	var_loop = obj->data.variable;

#ifdef MB_ENABLE_COLLECTION_LIB
	if(ast && ast->next && _IS_FUNC(ast->next->data, _core_in))
		result = _execute_ranged_for_loop(s, &ast, var_loop);
	else
		result = _execute_normal_for_loop(s, &ast, var_loop);
#else /* MB_ENABLE_COLLECTION_LIB */
	result = _execute_normal_for_loop(s, &ast, var_loop);
#endif /* MB_ENABLE_COLLECTION_LIB */

_exit:
	*l = ast;

	return result;
}

/* IN statement */
int _core_in(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* TO statement */
int _core_to(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* STEP statement */
int _core_step(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* NEXT statement */
int _core_next(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_running_context_t* running = 0;

	mb_assert(s && l);

	running = s->running_context;
	ast = (_ls_node_t*)*l;

	result = MB_LOOP_CONTINUE;

	ast = ast->next;
	if(ast)
		obj = (_object_t*)ast->data;
	if(_IS_VAR(obj))
		running->next_loop_var = obj->data.variable;

	*l = ast;

	return result;
}

/* WHILE statement */
int _core_while(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_ls_node_t* loop_begin_node = 0;
	_object_t* obj = 0;
	_object_t loop_cond;
	_object_t* loop_cond_ptr = 0;

	mb_assert(s && l);

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	loop_cond_ptr = &loop_cond;
	_MAKE_NIL(loop_cond_ptr);

	loop_begin_node = ast;

_loop_begin:
	ast = loop_begin_node;

	result = _calc_expression(s, &ast, &loop_cond_ptr);
	_ONCOND(s, loop_cond_ptr, 0);
	if(result != MB_FUNC_OK)
		goto _exit;

	if(loop_cond_ptr->data.integer) {
		/* Keep looping */
		if(!ast)
			goto _exit;
		obj = (_object_t*)ast->data;
		while(!_IS_FUNC(obj, _core_wend)) {
			result = _execute_statement(s, &ast, true);
			if(result == MB_LOOP_BREAK) { /* EXIT */
				if(_skip_struct(s, &ast, _core_while, 0, _core_wend) != MB_FUNC_OK)
					goto _exit;
				_skip_to(s, &ast, 0, _DT_EOS);
				result = MB_FUNC_OK;

				goto _exit;
			} else if(result == MB_SUB_RETURN && s->last_routine) { /* RETURN */
				if(ast) ast = ast->prev;
				if(ast) ast = ast->prev;

				goto _exit;
			} else if(result != MB_FUNC_OK && result != MB_SUB_RETURN) { /* Normally */
				goto _exit;
			}

			if(!ast) {
				_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
			obj = (_object_t*)ast->data;
		}

		goto _loop_begin;
	} else {
		/* End looping */
		if(_skip_struct(s, &ast, _core_while, 0, _core_wend) != MB_FUNC_OK)
			goto _exit;
		_skip_to(s, &ast, 0, _DT_EOS);

		goto _exit;
	}

_exit:
	*l = ast;

	return result;
}

/* WEND statement */
int _core_wend(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* DO statement */
int _core_do(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_ls_node_t* loop_begin_node = 0;
	_object_t* obj = 0;
	_object_t loop_cond;
	_object_t* loop_cond_ptr = 0;

	mb_assert(s && l);

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	obj = (_object_t*)ast->data;
	if(!_IS_EOS(obj)) {
		_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;

	loop_cond_ptr = &loop_cond;
	_MAKE_NIL(loop_cond_ptr);

	loop_begin_node = ast;

_loop_begin:
	ast = loop_begin_node;

	obj = ast ? (_object_t*)ast->data : 0;
	while(obj && !_IS_FUNC(obj, _core_until)) {
		result = _execute_statement(s, &ast, true);
		if(result == MB_LOOP_BREAK) { /* EXIT */
			if(_skip_struct(s, &ast, _core_do, 0, _core_until) != MB_FUNC_OK)
				goto _exit;
			_skip_to(s, &ast, 0, _DT_EOS);
			result = MB_FUNC_OK;

			goto _exit;
		} else if(result == MB_SUB_RETURN && s->last_routine) { /* RETURN */
			if(ast) ast = ast->prev;
			if(ast) ast = ast->prev;

			goto _exit;
		} else if(result != MB_FUNC_OK && result != MB_SUB_RETURN) { /* Normally */
			goto _exit;
		}

		obj = ast ? (_object_t*)ast->data : 0;
	}

	obj = ast ? (_object_t*)ast->data : 0;
	if(!obj || !_IS_FUNC(obj, _core_until)) {
		_handle_error_on_obj(s, SE_RN_UNTIL_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;

	result = _calc_expression(s, &ast, &loop_cond_ptr);
	_ONCOND(s, loop_cond_ptr, 0);
	if(result != MB_FUNC_OK)
		goto _exit;

	if(loop_cond_ptr->data.integer) {
		/* End looping */
		if(ast)
			_skip_to(s, &ast, 0, _DT_EOS);

		goto _exit;
	} else {
		/* Keep looping */
		goto _loop_begin;
	}

_exit:
	*l = ast;

	return result;
}

/* UNTIL statement */
int _core_until(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* EXIT statement */
int _core_exit(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	result = MB_LOOP_BREAK;

	return result;
}

/* GOTO statement */
int _core_goto(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_label_t* label = 0;
	_ls_node_t* glbsyminscope = 0;

	mb_assert(s && l);

	running = s->running_context;

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	_using_jump_set_of_instructional(s, ast, _exit, result);

	obj = (_object_t*)ast->data;
	if(obj->type != _DT_LABEL) {
		_handle_error_on_obj(s, SE_RN_JUMP_LABEL_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

	label = (_label_t*)obj->data.label;
	if(!label->node) {
		glbsyminscope = _ht_find(running->var_dict, label->name);
		if(!(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_LABEL)) {
			_handle_error_on_obj(s, SE_RN_LABEL_DOES_NOT_EXIST, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		label->node = ((_object_t*)glbsyminscope->data)->data.label->node;
	}

	mb_assert(label->node && label->node->prev);
	ast = label->node->prev;
	if(ast && !ast->data)
		ast = ast->next;

_exit:
	*l = ast;

	return result;
}

/* GOSUB statement */
int _core_gosub(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_running_context_t* running = 0;

	mb_assert(s && l);

	running = s->running_context;
	ast = (_ls_node_t*)*l;

	_using_jump_set_of_instructional(s, ast, _exit, result);

	result = _core_goto(s, l);
	if(result == MB_FUNC_OK)
		_ls_pushback(s->sub_stack, ast);

_exit:
	return result;
}

/* RETURN statement */
int _core_return(mb_interpreter_t* s, void** l) {
	int result = MB_SUB_RETURN;
	_ls_node_t* ast = 0;
	_ls_node_t* sub_stack = 0;
	_running_context_t* running = 0;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	running = s->running_context;
	sub_stack = s->sub_stack;

	if(running->prev) {
		ast = (_ls_node_t*)*l;
		ast = ast->next;
		if(mb_has_arg(s, (void**)&ast)) {
			mb_check(mb_pop_value(s, (void**)&ast, &arg));
			mb_check(mb_push_value(s, (void**)&ast, arg));
		}
	}
	ast = (_ls_node_t*)_ls_popback(sub_stack);
	if(!ast) {
		_handle_error_on_obj(s, SE_RN_NO_RETURN_POINT, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	*l = ast;

_exit:
	return result;
}

/* CALL statement */
int _core_call(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_routine_t* routine = 0;
	mb_value_t ret;
	_ls_node_t* pathed = 0;

	mb_assert(s && l);

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	obj = (_object_t*)ast->data;

_retry:
	switch(obj->type) {
	case _DT_FUNC:
		if(_IS_FUNC(obj, _core_open_bracket)) {
			mb_check(mb_attempt_open_bracket(s, l));

			ast = ast->next;
			obj = (_object_t*)ast->data;
#ifdef MB_ENABLE_CLASS
			if(_IS_VAR(obj)) {
				pathed = _search_identifier_in_scope_chain(s, 0, obj->data.variable->name, _PN(obj->data.variable->pathing), 0, 0);
				if(pathed && pathed->data)
					obj = (_object_t*)pathed->data;
			}
#endif /* MB_ENABLE_CLASS */
			if(!obj || obj->type != _DT_ROUTINE) {
				_handle_error_on_obj(s, SE_RN_ROUTINE_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
			ret.type = MB_DT_ROUTINE;
			ret.value.routine = obj->data.routine;
			ast = ast->next;
			*l = ast;

			mb_check(mb_attempt_close_bracket(s, l));

			mb_check(mb_push_value(s, l, ret));

			ast = (_ls_node_t*)*l;
		}

		break;
	case _DT_VAR:
#ifdef MB_ENABLE_LAMBDA
		if(obj->data.variable->data->type == _DT_ROUTINE && obj->data.variable->data->data.routine->type == MB_RT_LAMBDA) {
#ifdef MB_ENABLE_CLASS
			int fp = _PN(obj->data.variable->pathing);
#else /* MB_ENABLE_CLASS */
			int fp = _PATHING_NORMAL;
#endif /* MB_ENABLE_CLASS */
			pathed = _search_identifier_in_scope_chain(s, 0, obj->data.variable->name, fp, 0, 0);
			if(pathed && pathed->data)
				obj = (_object_t*)pathed->data;
			if(!_IS_VAR(obj))
				goto _retry;
		}
#endif /* MB_ENABLE_LAMBDA */
		if(obj->data.variable->data->type == _DT_ROUTINE) {
			obj = obj->data.variable->data;

			goto _retry;
		}
#ifdef MB_ENABLE_CLASS
		pathed = _search_identifier_in_scope_chain(s, 0, obj->data.variable->name, _PN(obj->data.variable->pathing), 0, 0);
		if(pathed && pathed->data)
			obj = (_object_t*)pathed->data;
		/* Fall through */
#else /* MB_ENABLE_CLASS */
		mb_unrefvar(pathed);
#endif /* MB_ENABLE_CLASS */
	case _DT_ROUTINE:
		obj = _GET_ROUTINE(obj);
		routine = obj->data.routine;
#ifdef MB_ENABLE_CLASS
#ifdef MB_ENABLE_LAMBDA
		if(routine->type != MB_RT_LAMBDA) {
#else /* MB_ENABLE_LAMBDA */
		{
#endif /* MB_ENABLE_LAMBDA */
			bool_t is_a0 = false;
			bool_t is_a1 = false;
			if(s->last_instance && routine->instance) {
				_traverse_class(s->last_instance->created_from, 0, _is_a_class, _META_LIST_MAX_DEPTH, true, routine->instance->created_from, &is_a0);
				_traverse_class(routine->instance->created_from, 0, _is_a_class, _META_LIST_MAX_DEPTH, true, s->last_instance->created_from, &is_a1);
			}
			if(routine->instance && (
				!s->last_instance || (
					s->last_instance &&
						!is_a0 && !is_a1 &&
						s->last_instance->created_from != routine->instance &&
						routine->instance->created_from != s->last_instance
					)
				)
			) {
				pathed = _search_identifier_in_class(s, routine->instance, routine->name, 0, 0);
			} else {
				pathed = _search_identifier_in_scope_chain(s, 0, routine->name, _PATHING_NONE, 0, 0);
			}
			if(pathed && pathed->data) {
				obj = (_object_t*)pathed->data;
				obj = _GET_ROUTINE(obj);
				routine = obj->data.routine;
			}
		}
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_CLASS
		s->calling = true;
#endif /* MB_ENABLE_CLASS */
		result = _eval_routine(s, &ast, 0, 0, routine, _has_routine_lex_arg, _pop_routine_lex_arg);
#ifdef MB_ENABLE_CLASS
		s->calling = false;
#endif /* MB_ENABLE_CLASS */
		if(ast)
			ast = ast->prev;

		break;
	default: /* Do nothing */
		break;
	}

	*l = ast;

_exit:
	return result;
}

/* DEF statement */
int _core_def(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_running_context_t* running = 0;
	_object_t* obj = 0;
	_var_t* var = 0;
	_ls_node_t* rnode = 0;
	_routine_t* routine = 0;

	mb_assert(s && l);

	running = s->running_context;

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	_using_jump_set_of_structured(s, ast, _exit, result);

	if(s->has_run)
		goto _skip;

	obj = ast ? (_object_t*)ast->data : 0;
	if(!_IS_ROUTINE(obj)) {
		_handle_error_on_obj(s, SE_RN_ROUTINE_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	if(obj->data.routine->func.basic.entry) {
		_handle_error_on_obj(s, SE_RN_DUPLICATE_ROUTINE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	routine = (_routine_t*)((_object_t*)ast->data)->data.routine;
	ast = ast->next;
	obj = (_object_t*)ast->data;
	if(!_IS_FUNC(obj, _core_open_bracket)) {
		_handle_error_on_obj(s, SE_RN_OPEN_BRACKET_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;
	obj = (_object_t*)ast->data;
	while(obj && !_IS_FUNC(obj, _core_close_bracket)) {
		if(_IS_VAR(obj)) {
			var = obj->data.variable;
			rnode = _search_identifier_in_scope_chain(s, routine->func.basic.scope, var->name, _PATHING_NONE, 0, 0);
			if(rnode)
				var = ((_object_t*)rnode->data)->data.variable;
			if(!routine->func.basic.parameters)
				routine->func.basic.parameters = _ls_create();
			_ls_pushback(routine->func.basic.parameters, var);
		} else if(_IS_FUNC(obj, _core_args)) {
			if(!routine->func.basic.parameters)
				routine->func.basic.parameters = _ls_create();
			_ls_pushback(routine->func.basic.parameters, (void*)&_VAR_ARGS);
			ast = ast->next;
			obj = (_object_t*)ast->data;

			break;
		}

		ast = ast->next;
		obj = ast ? (_object_t*)ast->data : 0;
	}
	if(ast) ast = ast->next;
	routine->func.basic.entry = ast;

_skip:
	_skip_to(s, &ast, _core_enddef, _DT_INVALID);

	if(ast) ast = ast->next;

_exit:
	*l = ast;

	return result;
}

/* ENDDEF statement */
int _core_enddef(mb_interpreter_t* s, void** l) {
	int result = MB_SUB_RETURN;
	_ls_node_t* ast = 0;
	_ls_node_t* sub_stack = 0;

	mb_assert(s && l);

	sub_stack = s->sub_stack;

	ast = (_ls_node_t*)_ls_popback(sub_stack);
	if(!ast) {
		_handle_error_on_obj(s, SE_RN_NO_RETURN_POINT, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	*l = ast;

_exit:
	return result;
}

/* ... (variable argument list) statement */
int _core_args(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_ls_node_t* var_args = 0;
	bool_t pushed = false;

	mb_assert(s && l);

	ast = (_ls_node_t*)*l;
	if(ast) ast = ast->next;
	*l = ast;

	var_args = s->var_args;
	if(var_args) {
		_object_t* obj = (_object_t*)_ls_popfront(var_args);
		if(obj) {
			mb_value_t arg;
			mb_make_nil(arg);
			_internal_object_to_public_value(obj, &arg);
			mb_check(mb_push_value(s, l, arg));
			_UNREF(obj)
			pushed = true;
			_destroy_object_capsule_only(obj, 0);
		}
	}

	if(!pushed) {
		mb_value_t arg;
		mb_make_nil(arg);
		arg.type = MB_DT_UNKNOWN;
		mb_check(mb_push_value(s, l, arg));
	}

	return result;
}

#ifdef MB_ENABLE_CLASS
/* CLASS statement */
int _core_class(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_running_context_t* running = 0;
	_object_t* obj = 0;
	_class_t* instance = 0;
	_class_t* inherit = 0;
	_class_t* last_inst = 0;

	mb_assert(s && l);

	running = s->running_context;

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	_using_jump_set_of_structured(s, ast, _exit, result);

	if(s->has_run) {
		if(ast) {
			_skip_to(s, &ast, _core_endclass, _DT_NIL);

			ast = ast->next;
		}

		*l = ast;

		return result;
	}

	obj = (_object_t*)ast->data;
	obj = _GET_CLASS(obj);
	if(!_IS_CLASS(obj)) {
		_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	instance = obj->data.instance;
	ast = ast->next;
	obj = (_object_t*)ast->data;

	last_inst = s->last_instance;
	s->last_instance = instance;

	/* Process meta prototype list */
	if(_IS_FUNC(obj, _core_open_bracket)) {
		do {
			ast = ast->next;
			obj = (_object_t*)ast->data;
			if(_IS_VAR(obj)) {
				_ls_node_t* tmp =_search_identifier_in_scope_chain(s, _OUTTER_SCOPE(running), obj->data.variable->name, _PATHING_NONE, 0, 0);
				if(tmp && tmp->data)
					obj = (_object_t*)tmp->data;
			}
			obj = _GET_CLASS(obj);
			if(!_IS_CLASS(obj)) {
				_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
			inherit = obj->data.instance;
			_link_meta_class(s, instance, inherit);
			ast = ast->next;
			obj = (_object_t*)ast->data;
		} while(_IS_CLASS(obj) || _IS_SEP(obj, ','));
		if(_IS_FUNC(obj, _core_close_bracket)) {
			ast = ast->next;
		} else {
			_handle_error_on_obj(s, SE_RN_CLOSE_BRACKET_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
	}

	*l = ast;

	/* Execute class body */
	running = _push_scope_by_class(s, instance->scope);
	do {
		result = _execute_statement(s, (_ls_node_t**)l, true);
		if(result != MB_FUNC_OK) {
			if(result >= MB_EXTENDED_ABORT)
				s->last_error = SE_EA_EXTENDED_ABORT;
			_handle_error_now(s, s->last_error, s->last_error_file, result);

			goto _pop_exit;
		}
		ast = (_ls_node_t*)*l;
		if(!ast)
			break;
		obj = (_object_t*)ast->data;
	} while(ast && !_IS_FUNC(obj, _core_endclass));
	_pop_scope(s, false);

	/* Search for meta functions */
	if(_search_class_hash_and_compare_functions(s, instance) != MB_FUNC_OK) {
		_handle_error_on_obj(s, SE_RN_HASH_AND_COMPARE_MUST_BE_PROVIDED_TOGETHER, s->source_file, DON(ast), MB_FUNC_WARNING, _exit, result);
	}

	/* Finished */
	if(ast) {
		_skip_to(s, &ast, _core_endclass, _DT_NIL);

		ast = ast->next;
	}

_pop_exit:
	if(result != MB_FUNC_OK)
		_pop_scope(s, false);

_exit:
	*l = ast;

	s->last_instance = last_inst;

	return result;
}

/* ENDCLASS statement */
int _core_endclass(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	_do_nothing(s, l, _exit, result);

_exit:
	return result;
}

/* NEW statement */
int _core_new(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;
	_object_t obj;
	_object_t tgt;
	mb_value_t ret;
	_class_t* instance = 0;

	mb_assert(s && l);

	mb_make_nil(ret);

	mb_check(mb_attempt_func_begin(s, l));

	mb_check(mb_pop_value(s, l, &arg));

	mb_check(mb_attempt_func_end(s, l));

	_MAKE_NIL(&obj);
	_MAKE_NIL(&tgt);
	switch(arg.type) {
	case MB_DT_STRING:
		arg.value.string = mb_strupr(arg.value.string);
		if((instance = _reflect_string_to_class(s, arg.value.string, &arg)) == 0)
			goto _default;
		_ref(&instance->ref, instance);
		/* Fall through */
	case MB_DT_CLASS:
		_public_value_to_internal_object(&arg, &obj);
		_clone_object(s, &obj, &tgt, false, true);
		ret.type = MB_DT_CLASS;
		ret.value.instance = tgt.data.instance;

		break;
	default:
_default:
		_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}

	mb_check(mb_push_value(s, l, ret));
	_assign_public_value(s, &ret, 0, false);

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* VAR statement */
int _core_var(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_IGNORE;
	_ls_node_t* ast = 0;
	mb_unrefvar(s);

	ast = (_ls_node_t*)*l;
	ast = ast->next;

	if(!s->last_instance) {
		_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);
	}

_exit:
	*l = ast;

	return result;
}

/* REFLECT statement */
int _core_reflect(mb_interpreter_t* s, void** l) {
#ifdef MB_ENABLE_COLLECTION_LIB
	int result = MB_FUNC_OK;
	mb_value_t arg;
	_object_t obj;
	mb_value_t ret;
	_class_t* instance = 0;
	_dict_t* coll = 0;

	mb_assert(s && l);

	mb_make_nil(ret);

	mb_check(mb_attempt_func_begin(s, l));

	mb_check(mb_pop_value(s, l, &arg));

	mb_check(mb_attempt_func_end(s, l));

	_MAKE_NIL(&obj);
	switch(arg.type) {
	case MB_DT_STRING:
		arg.value.string = mb_strupr(arg.value.string);
		if((instance = _reflect_string_to_class(s, arg.value.string, &arg)) == 0)
			goto _default;
		_ref(&instance->ref, instance);
		/* Fall through */
	case MB_DT_CLASS:
		_public_value_to_internal_object(&arg, &obj);
		coll = _create_dict(s);
		_traverse_class(obj.data.instance, _reflect_class_field, 0, _META_LIST_MAX_DEPTH, false, coll, 0);
		ret.type = MB_DT_DICT;
		ret.value.dict = coll;

		break;
	default:
_default:
		_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}

	mb_check(mb_push_value(s, l, ret));

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(s);
	mb_unrefvar(l);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_COLLECTION_LIB */
}
#endif /* MB_ENABLE_CLASS */

#ifdef MB_ENABLE_LAMBDA
/* LAMBDA statement */
int _core_lambda(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t ret;
	_running_context_t* running = 0;
	_routine_t* routine = 0;
	_ls_node_t* ast = 0;
	int brackets = 0;
	_var_t* var = 0;
	_object_t* obj = 0;
	unsigned ul = 0;
	bool_t popped = false;

	mb_assert(s && l);

	/* Create lambda struct */
	routine = (_routine_t*)mb_malloc(sizeof(_routine_t));
	running = _init_lambda(s, routine);

	/* Parameter list */
	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _error);

	while(mb_has_arg(s, l)) {
#ifdef MB_ENABLE_CLASS
		unsigned char fp = _PATHING_NONE;
#endif /* MB_ENABLE_CLASS */
		void* v = 0;

		if(!routine->func.lambda.parameters)
			routine->func.lambda.parameters = _ls_create();

		ast = (_ls_node_t*)*l;
		if(ast && _IS_FUNC(ast->data, _core_args)) {
			_ls_pushback(routine->func.lambda.parameters, (void*)&_VAR_ARGS);
			ast = ast->next;
			*l = ast;

			break;
		}

		_mb_check_mark_exit(mb_get_var(s, l, &v, true), result, _error);

		if(!_IS_VAR(v)) {
			_handle_error_on_obj(s, SE_RN_INVALID_LAMBDA, s->source_file, DON2(l), MB_FUNC_ERR, _error, result);
		}
		var = ((_object_t*)v)->data.variable;
#ifdef MB_ENABLE_CLASS
		fp = var->pathing;
#endif /* MB_ENABLE_CLASS */

		/* Add lambda parameters */
		obj = 0;
		var = _create_var(&obj, var->name, 0, true);
#ifdef MB_ENABLE_CLASS
		var->pathing = fp;
#endif /* MB_ENABLE_CLASS */
		ul = _ht_set_or_insert(routine->func.lambda.scope->var_dict, var->name, obj);
		mb_assert(ul);
		_ls_pushback(routine->func.lambda.parameters, var);

		ast = (_ls_node_t*)*l;
		if(_IS_FUNC(ast->data, _core_close_bracket))
			break;
		*l = ast;
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _error);

	/* Lambda body */
	ast = (_ls_node_t*)*l;
	if(ast) ast = ast->prev;
	while(ast && _IS_EOS(ast->next->data))
		ast = ast->next;
	*l = ast;

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _error);

	ast = (_ls_node_t*)*l;
	routine->func.lambda.entry = ast;
	while(ast && (brackets || !_IS_FUNC(ast->data, _core_close_bracket))) {
		if(_IS_FUNC(ast->data, _core_open_bracket))
			brackets++;
		else if(_IS_FUNC(ast->data, _core_close_bracket))
			brackets--;

		if(ast && !_is_valid_lambda_body_node(s, &routine->func.lambda, (_object_t*)ast->data)) {
			_handle_error_on_obj(s, SE_RN_INVALID_LAMBDA, s->source_file, DON2(l), MB_FUNC_ERR, _error, result);
		}

		/* Mark upvalues */
		if(ast)
			_try_mark_upvalue(s, routine, (_object_t*)ast->data);

		ast = ast->next;
	}
	*l = ast;
	routine->func.lambda.end = ast;

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _error);

	_pop_scope(s, false);
	popped = true;

	/* Push the return value */
	ret.type = MB_DT_ROUTINE;
	ret.value.routine = routine;

	_mb_check_mark_exit(mb_push_value(s, l, ret), result, _error);

	/* Error processing */
	while(0) {
_error:
		if(!popped)
			_pop_scope(s, false);
		if(routine)
			_destroy_routine(s, routine);
	}

	return result;
}
#endif /* MB_ENABLE_LAMBDA */

#ifdef MB_ENABLE_ALLOC_STAT
/* MEM statement */
int _core_mem(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	mb_check(mb_attempt_func_begin(s, l));

	mb_check(mb_attempt_func_end(s, l));

	mb_check(mb_push_int(s, l, (int_t)_mb_allocated));

	return result;
}
#endif /* MB_ENABLE_ALLOC_STAT */

/* TYPE statement */
int _core_type(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;
	int i = 0;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(arg);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_value(s, l, &arg));
	if(arg.type == MB_DT_STRING) {
		mb_data_e types[] = {
			MB_DT_NIL,
			MB_DT_UNKNOWN,
			MB_DT_INT,
			MB_DT_REAL,
			MB_DT_NUM,
			MB_DT_STRING,
			MB_DT_TYPE,
			MB_DT_USERTYPE,
#ifdef MB_ENABLE_USERTYPE_REF
			MB_DT_USERTYPE_REF,
#endif /* MB_ENABLE_USERTYPE_REF */
			MB_DT_ARRAY,
#ifdef MB_ENABLE_COLLECTION_LIB
			MB_DT_LIST,
			MB_DT_LIST_IT,
			MB_DT_DICT,
			MB_DT_DICT_IT,
			MB_DT_COLLECTION,
			MB_DT_ITERATOR,
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
			MB_DT_CLASS,
#endif /* MB_ENABLE_CLASS */
			MB_DT_ROUTINE
		};
		for(i = 0; i < countof(types); i++) {
			unsigned e = types[i];
			if(!mb_stricmp(mb_get_type_string((mb_data_e)e), arg.value.string)) {
				arg.value.type = (mb_data_e)e;
				arg.type = MB_DT_TYPE;

				goto _found;
			}
		}
	}
	os = _try_overridden(s, l, &arg, _CORE_ID_TYPE, MB_MF_FUNC);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		arg.value.type = arg.type;
		arg.type = MB_DT_TYPE;
	}

_found:
	mb_check(mb_attempt_close_bracket(s, l));

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		mb_check(mb_push_value(s, l, arg));
	}

	return result;
}

/* IMPORT statement */
int _core_import(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	mb_check(mb_attempt_func_begin(s, l));

	mb_check(mb_attempt_func_end(s, l));

	return result;
}

/* END statement */
int _core_end(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	mb_check(mb_attempt_func_begin(s, l));

	mb_check(mb_attempt_func_end(s, l));

	result = MB_FUNC_END;

	return result;
}

