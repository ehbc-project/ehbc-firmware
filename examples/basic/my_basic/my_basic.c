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

#include "mb_lib_core.h"
#include "mb_lib_std.h"
#include "mb_lib_coll.h"
#include "mb_lib_dmm.h"

/*
** {========================================================
** Global Variables
*/

#ifdef MB_ENABLE_FULL_ERROR
/* Error description text */
MBCONST const char* const _ERR_DESC[] = {
	"No error",
	/** Common */
	"Function already exists",
	"Function does not exist",
	"Not supported",
	/** Parsing */
	"Failed to open file",
	"Symbol too long",
	"Invalid character",
	"Invalid module",
	"Duplicate IMPORT",
	/** Running */
	"Empty program",
	"Program too long",
	"Syntax error",
	"Out of memory",
	"Overflow",
	"Unexpected type",
	"Invalid string",
	"Integer expected",
	"Number expected",
	"String expected",
	"Variable expected",
	"Index out of bound",
	"Cannot find with the specific index",
	"Too many dimensions",
	"Rank out of bound",
	"Invalid identifier usage",
	"Cannot assign to reserved word",
	"Duplicate identifier",
	"Incomplete structure",
	"Label does not exist",
	"No return point",
	"Colon expected",
	"Comma expected",
	"Comma or semicolon expected",
	"Open bracket expected",
	"Close bracket expected",
	"Too many nested",
	"Failed to operation",
	"Operator expected",
	"Assign operator expected",
	"THEN statement expected",
	"ELSE statement expected",
	"ENDIF statement expected",
	"TO statement expected",
	"NEXT statement expected",
	"UNTIL statement expected",
	"Loop variable expected",
	"Jump label expected",
	"Calculation error",
	"Invalid expression",
	"Divide by zero",
	"Reached to wrong function",
	"Cannot suspend in a routine",
	"Cannot mix instructional and structured sub routines",
	"Invalid routine",
	"Routine expected",
	"Duplicate routine",
	"Invalid class",
	"Class expected",
	"Duplicate class",
	"HASH and COMPARE must be provided together",
	"Invalid lambda",
	"Empty collection",
	"List expected",
	"Invalid iterator",
	"Iterable expected",
	"Collection expected",
	"Collection or iterator expected",
	"Referenced type expected",
	/** Extended abort */
	"Extended abort"
};

mb_static_assert(countof(_ERR_DESC) == SE_COUNT);
#endif /* MB_ENABLE_FULL_ERROR */

/* Data type */

MBAPI size_t MB_SIZEOF_4BYTES = 4;
MBAPI size_t MB_SIZEOF_8BYTES = 8;
MBAPI size_t MB_SIZEOF_32BYTES = 32;
MBAPI size_t MB_SIZEOF_64BYTES = 64;
MBAPI size_t MB_SIZEOF_128BYTES = 128;
MBAPI size_t MB_SIZEOF_256BYTES = 256;
MBAPI size_t MB_SIZEOF_512BYTES = 512;
#ifdef MB_ENABLE_ALLOC_STAT
MBAPI size_t MB_SIZEOF_INT = _MB_MEM_TAG_SIZE + sizeof(int);
MBAPI size_t MB_SIZEOF_PTR = _MB_MEM_TAG_SIZE + sizeof(intptr_t);
MBAPI size_t MB_SIZEOF_LSN = _MB_MEM_TAG_SIZE + sizeof(_ls_node_t);
MBAPI size_t MB_SIZEOF_HTN = _MB_MEM_TAG_SIZE + sizeof(_ht_node_t);
MBAPI size_t MB_SIZEOF_HTA = _MB_MEM_TAG_SIZE + sizeof(_ht_node_t*) * _HT_ARRAY_SIZE_DEFAULT;
MBAPI size_t MB_SIZEOF_OBJ = _MB_MEM_TAG_SIZE + sizeof(_object_t);
#ifdef MB_ENABLE_USERTYPE_REF
MBAPI size_t MB_SIZEOF_UTR = _MB_MEM_TAG_SIZE + sizeof(_usertype_ref_t);
#endif /* MB_ENABLE_USERTYPE_REF */
MBAPI size_t MB_SIZEOF_FUN = _MB_MEM_TAG_SIZE + sizeof(_func_t);
MBAPI size_t MB_SIZEOF_VAR = _MB_MEM_TAG_SIZE + sizeof(_var_t);
MBAPI size_t MB_SIZEOF_ARR = _MB_MEM_TAG_SIZE + sizeof(_array_t);
#ifdef MB_ENABLE_COLLECTION_LIB
MBAPI size_t MB_SIZEOF_LST = _MB_MEM_TAG_SIZE + sizeof(_list_t);
MBAPI size_t MB_SIZEOF_DCT = _MB_MEM_TAG_SIZE + sizeof(_dict_t);
#endif /* MB_ENABLE_COLLECTION_LIB */
MBAPI size_t MB_SIZEOF_LBL = _MB_MEM_TAG_SIZE + sizeof(_label_t);
#ifdef MB_ENABLE_CLASS
MBAPI size_t MB_SIZEOF_CLS = _MB_MEM_TAG_SIZE + sizeof(_class_t);
#endif /* MB_ENABLE_CLASS */
MBAPI size_t MB_SIZEOF_RTN = _MB_MEM_TAG_SIZE + sizeof(_routine_t);
#else /* MB_ENABLE_ALLOC_STAT */
MBAPI size_t MB_SIZEOF_INT = sizeof(int);
MBAPI size_t MB_SIZEOF_PTR = sizeof(intptr_t);
MBAPI size_t MB_SIZEOF_LSN = sizeof(_ls_node_t);
MBAPI size_t MB_SIZEOF_HTN = sizeof(_ht_node_t);
MBAPI size_t MB_SIZEOF_HTA = sizeof(_ht_node_t*) * _HT_ARRAY_SIZE_DEFAULT;
MBAPI size_t MB_SIZEOF_OBJ = sizeof(_object_t);
#ifdef MB_ENABLE_USERTYPE_REF
MBAPI size_t MB_SIZEOF_UTR = sizeof(_usertype_ref_t);
#endif /* MB_ENABLE_USERTYPE_REF */
MBAPI size_t MB_SIZEOF_FUN = sizeof(_func_t);
MBAPI size_t MB_SIZEOF_VAR = sizeof(_var_t);
MBAPI size_t MB_SIZEOF_ARR = sizeof(_array_t);
#ifdef MB_ENABLE_COLLECTION_LIB
MBAPI size_t MB_SIZEOF_LST = sizeof(_list_t);
MBAPI size_t MB_SIZEOF_DCT = sizeof(_dict_t);
#endif /* MB_ENABLE_COLLECTION_LIB */
MBAPI size_t MB_SIZEOF_LBL = sizeof(_label_t);
#ifdef MB_ENABLE_CLASS
MBAPI size_t MB_SIZEOF_CLS = sizeof(_class_t);
#endif /* MB_ENABLE_CLASS */
MBAPI size_t MB_SIZEOF_RTN = sizeof(_routine_t);
#endif /* MB_ENABLE_ALLOC_STAT */

MBCONST const _object_t _OBJ_INT_UNIT = { _DT_INT, { (int_t)1 }, _CONST_TAIL };
MBCONST const _object_t _OBJ_INT_ZERO = { _DT_INT, { (int_t)0 }, _CONST_TAIL };

_object_t* _OBJ_BOOL_TRUE = 0;
_object_t* _OBJ_BOOL_FALSE = 0;

#ifdef MB_ENABLE_CLASS
MBCONST const _object_t _OBJ_UNKNOWN = { _DT_UNKNOWN, { (int_t)0 }, _CONST_TAIL };
MBCONST const _ls_node_t _LS_NODE_UNKNOWN = { (void*)&_OBJ_UNKNOWN, 0, 0, 0 };
#endif /* MB_ENABLE_CLASS */

#ifdef MB_ENABLE_CLASS
MBCONST const _var_t _VAR_ARGS = { _VAR_ARGS_STR, 0, 0, 0 };
#else /* MB_ENABLE_CLASS */
MBCONST const _var_t _VAR_ARGS = { _VAR_ARGS_STR, 0 };
#endif /* MB_ENABLE_CLASS */

/* Parsing context */

MBCONST const char _MULTI_LINE_COMMENT_PREFIX[3] = "'[";
MBCONST const char _MULTI_LINE_COMMENT_POSTFIX[3] = "']";

MBCONST const char _UNICODE_OPEN_QUOTE[3] = { (char)0xe2, (char)0x80, (char)0x9c };
MBCONST const char _UNICODE_CLOSE_QUOTE[3] = { (char)0xe2, (char)0x80, (char)0x9d };

/* Operations */
MBCONST const char _PRECEDE_TABLE[20][20] = { /* Operator priority table */
	/* +    -    *    /   MOD   ^    (    )    =    >    <    >=   <=   ==   <>  AND   OR  NOT  NEG   IS */
	{ '>', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>' }, /* + */
	{ '>', '>', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>' }, /* - */
	{ '>', '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>' }, /* * */
	{ '>', '>', '>', '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>' }, /* / */
	{ '>', '>', '<', '<', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>' }, /* MOD */
	{ '>', '>', '>', '>', '>', '>', '<', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>' }, /* ^ */
	{ '<', '<', '<', '<', '<', '<', '<', '=', ' ', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<' }, /* ( */
	{ '>', '>', '>', '>', '>', '>', ' ', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>' }, /* ) */
	{ '<', '<', '<', '<', '<', '<', '<', ' ', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<' }, /* = */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', '>', '>' }, /* > */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', '>', '>' }, /* < */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', '>', '>' }, /* >= */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', '>', '>' }, /* <= */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', '>', '>' }, /* == */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '>', '>', '>', '>' }, /* <> */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<', '<', '<', '>', '>', '<', '>', '>' }, /* AND */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<', '<', '<', '>', '>', '<', '>', '>' }, /* OR */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<', '<', '<', '>', '>', '>', '>', '>' }, /* NOT */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<' }, /* NEG */
	{ '<', '<', '<', '<', '<', '<', '<', '>', '>', '<', '<', '<', '<', '<', '<', '>', '>', '<', '>', '>' }  /* IS */
};

_object_t* _exp_assign = 0;

/** Memory manipulations */

#ifdef MB_ENABLE_ALLOC_STAT
volatile size_t _mb_allocated = 0;
#else /* MB_ENABLE_ALLOC_STAT */
const size_t _mb_allocated = (size_t)(~0);
#endif /* MB_ENABLE_ALLOC_STAT */

mb_memory_allocate_func_t _mb_allocate_func = 0;
mb_memory_free_func_t _mb_free_func = 0;


/* ========================================================} */

/*
** {========================================================
** Private function definitions
*/

/** Expression processing */

/* Determine whether a function is an operator */
bool_t _is_operator(mb_func_t op) {
	return (
		(op == _core_dummy_assign) ||
		(op == _core_add) ||
		(op == _core_min) ||
		(op == _core_mul) ||
		(op == _core_div) ||
		(op == _core_mod) ||
		(op == _core_pow) ||
		(op == _core_open_bracket) ||
		(op == _core_close_bracket) ||
		(op == _core_equal) ||
		(op == _core_less) ||
		(op == _core_greater) ||
		(op == _core_less_equal) ||
		(op == _core_greater_equal) ||
		(op == _core_not_equal) ||
		(op == _core_and) ||
		(op == _core_or) ||
		(op == _core_is)
	);
}

/* Determine whether a function is for flow control */
bool_t _is_flow(mb_func_t op) {
	return (
		(op == _core_if) ||
		(op == _core_then) ||
		(op == _core_elseif) ||
		(op == _core_else) ||
		(op == _core_endif) ||
		(op == _core_for) ||
		(op == _core_to) ||
		(op == _core_step) ||
		(op == _core_next) ||
		(op == _core_while) ||
		(op == _core_wend) ||
		(op == _core_do) ||
		(op == _core_until) ||
		(op == _core_exit) ||
		(op == _core_goto) ||
		(op == _core_gosub) ||
		(op == _core_return) ||
		(op == _core_end)
	);
}

/* Determine whether a function is unary */
bool_t _is_unary(mb_func_t op) {
	return (op == _core_neg) || (op == _core_not);
}

/* Determine whether a function is binary */
bool_t _is_binary(mb_func_t op) {
	return (
		(op == _core_add) ||
		(op == _core_min) ||
		(op == _core_mul) ||
		(op == _core_div) ||
		(op == _core_mod) ||
		(op == _core_pow) ||
		(op == _core_equal) ||
		(op == _core_less) ||
		(op == _core_greater) ||
		(op == _core_less_equal) ||
		(op == _core_greater_equal) ||
		(op == _core_not_equal) ||
		(op == _core_and) ||
		(op == _core_or) ||
		(op == _core_is)
	);
}

/* Get the priority of two operators */
char _get_priority(mb_func_t op1, mb_func_t op2) {
	char result = _ZERO_CHAR;
	int idx1 = 0;
	int idx2 = 0;

	mb_assert(op1 && op2);

	idx1 = _get_priority_index(op1);
	idx2 = _get_priority_index(op2);
	mb_assert(idx1 < countof(_PRECEDE_TABLE) && idx2 < countof(_PRECEDE_TABLE[0]));
	result = _PRECEDE_TABLE[idx1][idx2];

	return result;
}

/* Get the index of an operator in the priority table */
int _get_priority_index(mb_func_t op) {
	int i = 0;

	MBCONST mb_func_t funcs[] = {
		_core_add,
		_core_min,
		_core_mul,
		_core_div,
		_core_mod,
		_core_pow,
		_core_open_bracket,
		_core_close_bracket,
		_core_dummy_assign,
		_core_greater,
		_core_less,
		_core_greater_equal,
		_core_less_equal,
		_core_equal,
		_core_not_equal,
		_core_and,
		_core_or,
		_core_not,
		_core_neg,
		_core_is
	};

	mb_assert(op);

	for(i = 0; i < countof(funcs); i++) {
		if(op == funcs[i])
			return i;
	}

	mb_assert(0 && "Unknown operator.");

	return -1;
}

/* Operate two operands */
_object_t* _operate_operand(mb_interpreter_t* s, _object_t* optr, _object_t* opnd1, _object_t* opnd2, int* status) {
	_object_t* result = 0;
	_tuple3_t tp;
	_tuple3_t* tpptr = 0;
	int ret = 0;

	mb_assert(s && optr);
	mb_assert(optr->type == _DT_FUNC);

	if(!opnd1)
		return result;

	result = _create_object();

	memset(&tp, 0, sizeof(_tuple3_t));
	tp.e1 = opnd1;
	tp.e2 = opnd2;
	tp.e3 = result;
	tpptr = &tp;

	ret = (optr->data.func->pointer)(s, (void**)&tpptr);
	if(status)
		*status = ret;
	if(ret != MB_FUNC_OK) {
		if(ret != MB_FUNC_WARNING) {
			safe_free(result);
		}
		if(_set_current_error(s, SE_RN_FAILED_TO_OPERATE, 0)) {
#ifdef MB_ENABLE_SOURCE_TRACE
			_set_error_pos(s, optr->source_pos, optr->source_row, optr->source_col);
#else /* MB_ENABLE_SOURCE_TRACE */
			_set_error_pos(s, 0, 0, 0);
#endif /* MB_ENABLE_SOURCE_TRACE */
		}
	}

	return result;
}

/* Determine whether an object is an expression termination */
bool_t _is_expression_terminal(mb_interpreter_t* s, _object_t* obj) {
	bool_t result = false;

	mb_assert(s && obj);

	result = (
		(obj->type == _DT_EOS) ||
		(obj->type == _DT_SEP) ||
		(obj->type == _DT_FUNC &&
			(obj->data.func->pointer == _core_then ||
			obj->data.func->pointer == _core_elseif ||
			obj->data.func->pointer == _core_else ||
			obj->data.func->pointer == _core_endif ||
			obj->data.func->pointer == _core_to ||
			obj->data.func->pointer == _core_step))
	);

	return result;
}

/* Determine whether an object is an unexpected calculation result */
bool_t _is_unexpected_calc_type(mb_interpreter_t* s, _object_t* obj) {
	mb_assert(s);

	return !obj || (
		(obj->type == _DT_FUNC) ||
		(obj->type == _DT_LABEL) ||
#ifdef MB_ENABLE_LAMBDA
		(obj->type == _DT_OUTER_SCOPE) ||
#endif /* MB_ENABLE_LAMBDA */
		(obj->type == _DT_SEP) ||
#ifdef MB_ENABLE_SOURCE_TRACE
		(obj->type == _DT_PREV_IMPORT) ||
		(obj->type == _DT_POST_IMPORT) ||
#endif /* MB_ENABLE_SOURCE_TRACE */
		(obj->type == _DT_EOS)
	);
}

/* Determine whether an object is a referenced calculation result */
bool_t _is_referenced_calc_type(mb_interpreter_t* s, _object_t* obj) {
	mb_assert(s && obj);

	return (
#ifdef MB_ENABLE_USERTYPE_REF
		(obj->type == _DT_USERTYPE_REF) ||
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_COLLECTION_LIB
		(obj->type == _DT_LIST) || (obj->type == _DT_DICT) || (obj->type == _DT_LIST_IT) || (obj->type == _DT_DICT_IT) ||
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
		(obj->type == _DT_CLASS) ||
#endif /* MB_ENABLE_CLASS */
		(obj->type == _DT_ARRAY) ||
		(obj->type == _DT_ROUTINE)
	);
}

/* Calculate an expression */
int _calc_expression(mb_interpreter_t* s, _ls_node_t** l, _object_t** val) {
	int result = 0;
	_ls_node_t* ast = 0;
	_running_context_t* running = 0;
	_ls_node_t* garbage = 0;
	_ls_node_t* optr = 0;
	_ls_node_t* opnd = 0;
	_object_t* c = 0;
	_object_t* x = 0;
	_object_t* a = 0;
	_object_t* b = 0;
	_object_t* r = 0;
	_object_t* theta = 0;
	char pri = _ZERO_CHAR;
	int* inep = 0;
	int f = 0;

	_object_t* guard_val = 0;
	int bracket_count = 0;
	bool_t hack = false;
	_ls_node_t* errn = 0;
	bool_t gce = true;

	mb_assert(s && l);

	gce = mb_get_gc_enabled(s);
	mb_set_gc_enabled(s, false);

	running = s->running_context;
	ast = *l;

	if(!ast) {
		_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
	}
	c = (_object_t*)ast->data;
#ifdef MB_PREFER_SPEED
	if(c->is_const) {
		ast = ast->next;

		goto _fast;
	}
#endif /* MB_PREFER_SPEED */

	optr = _ls_create();
	opnd = _ls_create();

#define _LAZY_INIT_GLIST do { if(!garbage) garbage = _ls_create(); } while(0)

	inep = (int*)mb_malloc(sizeof(int));
	*inep = 0;
	_ls_pushback(s->in_neg_expr, inep);

	do {
		if(c->type == _DT_STRING) {
			if(ast->next) {
				_object_t* _fsn = (_object_t*)ast->next->data;
				if(_IS_FUNC(_fsn, _core_add) || _IS_FUNC(_fsn, _core_equal) || _IS_FUNC(_fsn, _core_not_equal) || _IS_FUNC(_fsn, _core_and) || _IS_FUNC(_fsn, _core_or) || _IS_FUNC(_fsn, _core_is))
					break;
			}

			(*val)->type = _DT_STRING;
			(*val)->data.string = c->data.string;
			(*val)->is_ref = true;
			ast = ast->next;

			goto _exit;
		}
	} while(0);
	guard_val = c;
	ast = ast->next;
	_ls_pushback(optr, _exp_assign);
	while(
		c &&
		(!(c->type == _DT_FUNC && strcmp(c->data.func->name, _DUMMY_ASSIGN_CHAR) == 0) ||
		!(((_object_t*)(_ls_back(optr)->data))->type == _DT_FUNC && strcmp(((_object_t*)(_ls_back(optr)->data))->data.func->name, _DUMMY_ASSIGN_CHAR) == 0))
	) {
		if(!hack) {
			if(_IS_FUNC(c, _core_open_bracket)) {
				++bracket_count;
			} else if(_IS_FUNC(c, _core_close_bracket)) {
				--bracket_count;
				if(bracket_count < 0) {
					c = _exp_assign;
					ast = ast->prev;

					continue;
				}
			}
		}
		hack = false;
		if(!(c->type == _DT_FUNC && _is_operator(c->data.func->pointer))) {
			if(_is_expression_terminal(s, c)) {
				c = _exp_assign;
				if(ast)
					ast = ast->prev;
				if(bracket_count) {
					_object_t _cb;
					_func_t _cbf;
					_MAKE_NIL(&_cb);
					_cb.type = _DT_FUNC;
					_cb.data.func = &_cbf;
					_cb.data.func->name = ")";
					_cb.data.func->pointer = _core_close_bracket;
					while(bracket_count) {
						_ls_pushback(optr, &_cb);
						bracket_count--;
						f = 0;
					}
					errn = ast;
				}
			} else {
				if(c->type == _DT_ARRAY) {
#ifdef MB_ENABLE_CLASS
					if(s->last_instance) {
						_ls_node_t* cs = _search_identifier_in_scope_chain(s, 0, c->data.array->name, _PATHING_NORMAL, 0, 0);
						if(cs)
							c = (_object_t*)cs->data;
					}
#endif /* MB_ENABLE_CLASS */
_array:
					if(ast && !_IS_FUNC(((_object_t*)ast->data), _core_open_bracket)) {
						_ls_pushback(opnd, c);
						f++;
					} else {
						unsigned arr_idx = 0;
						mb_value_u arr_val;
						_data_e arr_type;
						_object_t* arr_elem = 0;
						ast = ast->prev;
						result = _get_array_index(s, &ast, c, &arr_idx, 0);
						if(result != MB_FUNC_OK) {
							_handle_error_on_obj(s, SE_RN_CALCULATION_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
						}
						ast = ast->next;
						_get_array_elem(s, c->data.array, arr_idx, &arr_val, &arr_type);
						arr_elem = _create_object();
						_LAZY_INIT_GLIST;
						_ls_pushback(garbage, arr_elem);
						arr_elem->type = arr_type;
						arr_elem->is_ref = true;
						_copy_bytes(arr_elem->data.bytes, arr_val.bytes);
						if(f) {
							_handle_error_on_obj(s, SE_RN_OPERATOR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
						}
						_ls_pushback(opnd, arr_elem);
						f++;
					}
				} else if(c->type == _DT_FUNC) {
					if(ast) ast = ast->prev;
					if(_IS_UNARY_FUNC(c)) {
#ifdef MB_ENABLE_STACK_TRACE
						_ls_pushback(s->stack_frames, c->data.func->name);
#endif /* MB_ENABLE_STACK_TRACE */
						result = (c->data.func->pointer)(s, (void**)&ast);
#ifdef MB_ENABLE_STACK_TRACE
						_ls_popback(s->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */
					} else {
						int calc_depth = running->calc_depth;
						running->calc_depth = _INFINITY_CALC_DEPTH;
#ifdef MB_ENABLE_STACK_TRACE
						_ls_pushback(s->stack_frames, c->data.func->name);
#endif /* MB_ENABLE_STACK_TRACE */
						result = (c->data.func->pointer)(s, (void**)&ast);
#ifdef MB_ENABLE_STACK_TRACE
						_ls_popback(s->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */
						running->calc_depth = calc_depth;
					}
					if(result != MB_FUNC_OK) {
						_handle_error_on_obj(s, SE_RN_CALCULATION_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
					}
					c = _create_object();
					_LAZY_INIT_GLIST;
					_ls_pushback(garbage, c);
					result = _public_value_to_internal_object(&running->intermediate_value, c);
					switch(c->type) {
					case _DT_ROUTINE:
						if(c->data.routine->type != MB_RT_SCRIPT)
							break;
						mb_make_nil(running->intermediate_value);
						/* Fall through */
					case _DT_STRING:
						c->is_ref = true;
					default: /* Do nothing */
						break;
					}
					if(result != MB_FUNC_OK)
						goto _error;
					if(f) {
						_handle_error_on_obj(s, SE_RN_OPERATOR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
					}
					if(_is_array(c)) {
						goto _array;
					} else {
						if(ast && _IS_FUNC(ast->data, _core_open_bracket)) {
							_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
						}
					}
					_ls_pushback(opnd, c);
					f++;
				} else if(c->type == _DT_ROUTINE) {
_routine:
					do {
#ifdef MB_ENABLE_CLASS
						bool_t calling = false;
						_object_t* obj = 0;
						_ls_node_t* fn = 0;
#endif /* MB_ENABLE_CLASS */
						if(!ast)
							break;
						ast = ast->prev;
#ifdef MB_ENABLE_CLASS
						calling = s->calling;
						s->calling = false;
#endif /* MB_ENABLE_CLASS */
						result = _eval_routine(s, &ast, 0, 0, c->data.routine, _has_routine_lex_arg, _pop_routine_lex_arg);
#ifdef MB_ENABLE_CLASS
						s->calling = calling;
#endif /* MB_ENABLE_CLASS */
#ifdef MB_ENABLE_CLASS
						obj = ast ? (_object_t*)ast->data : 0;
						if(_IS_VAR(obj) && _is_valid_class_accessor_following_routine(s, obj->data.variable, ast, &fn)) {
							if(fn) {
								if(ast) ast = ast->next;
								obj = (_object_t*)fn->data;
								if(_IS_VAR(obj)) {
									c = obj;

									goto _var;
								}
							}
						}
#endif /* MB_ENABLE_CLASS */
					} while(0);
					if(ast)
						ast = ast->prev;
					if(result == MB_FUNC_END)
						goto _error;
					if(result != MB_FUNC_OK) {
						_handle_error_on_obj(s, SE_RN_CALCULATION_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
					}
					c = _create_object();
					_LAZY_INIT_GLIST;
					_ls_pushback(garbage, c);
					result = _public_value_to_internal_object(&running->intermediate_value, c);
					if(result != MB_FUNC_OK)
						goto _error;
					if(f) {
						_handle_error_on_obj(s, SE_RN_OPERATOR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
					}
					_ls_pushback(opnd, c);
					f++;
				} else if(c->type == _DT_VAR && c->data.variable->data->type == _DT_ARRAY) {
					unsigned arr_idx = 0;
					mb_value_u arr_val;
					_data_e arr_type;
					_object_t* arr_elem = 0;

					if(ast && !_IS_FUNC(((_object_t*)ast->data), _core_open_bracket)) {
						c = c->data.variable->data;
						_ls_pushback(opnd, c);
						f++;
					} else {
						ast = ast->prev;
						result = _get_array_index(s, &ast, 0, &arr_idx, 0);
						if(result != MB_FUNC_OK) {
							_handle_error_on_obj(s, SE_RN_CALCULATION_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
						}
						ast = ast->next;
						_get_array_elem(s, c->data.variable->data->data.array, arr_idx, &arr_val, &arr_type);
						arr_elem = _create_object();
						_LAZY_INIT_GLIST;
						_ls_pushback(garbage, arr_elem);
						arr_elem->type = arr_type;
						arr_elem->is_ref = true;
						if(arr_type == _DT_INT) {
							arr_elem->data.integer = arr_val.integer;
						} else if(arr_type == _DT_REAL) {
							arr_elem->data.float_point = arr_val.float_point;
						} else if(arr_type == _DT_STRING) {
							arr_elem->data.string = arr_val.string;
						} else if(arr_type == _DT_USERTYPE) {
							arr_elem->data.usertype = arr_val.usertype;
						} else {
#ifdef MB_SIMPLE_ARRAY
							mb_assert(0 && "Unsupported.");
#else /* MB_SIMPLE_ARRAY */
							_copy_bytes(arr_elem->data.bytes, arr_val.bytes);
#endif /* MB_SIMPLE_ARRAY */
						}
						if(f) {
							_handle_error_on_obj(s, SE_RN_OPERATOR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
						}
						_ls_pushback(opnd, arr_elem);
						f++;
					}
				} else {
					if(c->type == _DT_VAR) {
						do {
							_ls_node_t* cs = _search_identifier_in_scope_chain(s, 0, c->data.variable->name,
#ifdef MB_ENABLE_CLASS
								_PU(c->data.variable->pathing),
#else /* MB_ENABLE_CLASS */
								0,
#endif /* MB_ENABLE_CLASS */
								0,
								0
							);
							if(cs) {
#ifdef MB_ENABLE_USERTYPE_REF
								_ls_node_t* fn = ast;
								if(fn) fn = fn->prev;
								if(_try_call_func_on_usertype_ref(s, &fn, c, cs, 0)) {
									ast = fn;
									c = _create_object();
									_LAZY_INIT_GLIST;
									_ls_pushback(garbage, c);
									_public_value_to_internal_object(&running->intermediate_value, c);
									_REF(c)
								} else {
#else /* MB_ENABLE_USERTYPE_REF */
								{
#endif /* MB_ENABLE_USERTYPE_REF */
									c = (_object_t*)cs->data;
									if(c && c->type == _DT_VAR && c->data.variable->data->type == _DT_ROUTINE) {
										c = c->data.variable->data;
									}
									if(ast && ast && _IS_FUNC(ast->data, _core_open_bracket)) {
										if(c && c->type == _DT_ROUTINE)
											goto _routine;
									}
								}
							}
						} while(0);
#ifdef MB_ENABLE_CLASS
_var:
#endif /* MB_ENABLE_CLASS */
						if(ast) {
							_object_t* _err_or_bracket = (_object_t*)ast->data;
							do {
#ifdef MB_ENABLE_COLLECTION_LIB
								if(_IS_VAR(c) && _IS_COLL(c->data.variable->data)) {
									if(_IS_FUNC(_err_or_bracket, _core_open_bracket)) {
										int_t idx = 0;
										mb_value_t key;
										mb_value_t ret;
										_object_t* ocoll = c->data.variable->data;

										mb_make_nil(ret);

										*l = ast->prev;

										_mb_check_exit(mb_attempt_open_bracket(s, (void**)l), _error);

										switch(ocoll->type) {
										case _DT_LIST:
											_mb_check_exit(mb_pop_int(s, (void**)l, &idx), _error);
											if(!_at_list(ocoll->data.list, idx, &ret)) {
												_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, TON(l), MB_FUNC_ERR, _error, result);
											}

											break;
										case _DT_DICT:
											mb_make_nil(key);
											_mb_check_exit(mb_pop_value(s, (void**)l, &key), _error);
											if(!_find_dict(ocoll->data.dict, &key, &ret)) {
												_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, TON(l), MB_FUNC_ERR, _error, result);
											}

											break;
										default: /* Do nothing */
											break;
										}

										_mb_check_exit(mb_attempt_close_bracket(s, (void**)l), _error);

										c = _create_object();
										_LAZY_INIT_GLIST;
										_ls_pushback(garbage, c);
										_public_value_to_internal_object(&ret, c);

										ast = *l;
									}

									break;
								}
#endif /* MB_ENABLE_COLLECTION_LIB */
								if(_IS_FUNC(_err_or_bracket, _core_open_bracket)) {
									_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
								}
							} while(0);
						}
					}
					if(f) {
						_handle_error_on_obj(s, SE_RN_OPERATOR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
					}
					_ls_pushback(opnd, c);
					f++;
				}
				if(running->calc_depth != _INFINITY_CALC_DEPTH)
					running->calc_depth--;
				if(ast && (running->calc_depth == _INFINITY_CALC_DEPTH || running->calc_depth)) {
					c = ast ? (_object_t*)ast->data : 0;
					if(c->type == _DT_FUNC && !_is_operator(c->data.func->pointer) && !_is_flow(c->data.func->pointer)) {
						_ls_foreach(opnd, _remove_source_object);

						_handle_error_on_obj(s, SE_RN_COLON_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
					}
					ast = ast->next;
				} else {
					c = _exp_assign;
				}
			}
		} else {
			pri = _get_priority(((_object_t*)(_ls_back(optr)->data))->data.func->pointer, c->data.func->pointer);
			switch(pri) {
			case '<':
				_ls_pushback(optr, c);
				c = ast ? (_object_t*)ast->data : 0;
				if(ast) ast = ast->next;
				f = 0;

				break;
			case '=':
				x = (_object_t*)_ls_popback(optr);
				c = ast ? (_object_t*)ast->data : 0;
				if(ast) ast = ast->next;

				break;
			case '>':
				theta = (_object_t*)_ls_popback(optr);
				b = (_object_t*)_ls_popback(opnd);
				a = (_object_t*)_ls_popback(opnd);
				r = _operate_operand(s, theta, a, b, &result);
				if(!r) {
					_ls_clear(optr);
					_handle_error_on_obj(s, SE_RN_FAILED_TO_OPERATE, s->source_file, errn ? DON(errn) : DON(ast), MB_FUNC_ERR, _error, result);
				}
				_ls_pushback(opnd, r);
				_LAZY_INIT_GLIST;
				_ls_pushback(garbage, r);
				if(_IS_FUNC(c, _core_close_bracket))
					hack = true;

				break;
			case ' ':
				_handle_error_on_obj(s, SE_RN_FAILED_TO_OPERATE, s->source_file, errn ? DON(errn) : DON(ast), MB_FUNC_ERR, _error, result);

				break;
			}
		}
	}

	if(errn) {
		_handle_error_on_obj(s, SE_RN_CLOSE_BRACKET_EXPECTED, s->source_file, DON(errn), MB_FUNC_ERR, _error, result);
	}

	c = (_object_t*)(_ls_popback(opnd));
	if(_is_unexpected_calc_type(s, c)) {
		_handle_error_on_obj(s, SE_RN_UNEXPECTED_TYPE, s->source_file, DON(ast), MB_FUNC_ERR, _error, result);
	}
#ifdef MB_PREFER_SPEED
	if(ast && ast->prev == *l) {
		_object_t* obj = (_object_t*)(*l)->data;
		switch(obj->type) {
		case _DT_NIL: /* Fall through */
		case _DT_INT: /* Fall through */
		case _DT_REAL:
			obj->is_const = true;

			break;
		default: /* Do nothing */
			break;
		}
	}
_fast:
#endif /* MB_PREFER_SPEED */
	if(c->type == _DT_VAR) {
		(*val)->type = c->data.variable->data->type;
		(*val)->data = c->data.variable->data->data;
		if(_is_string(c))
			(*val)->is_ref = true;
	} else {
		(*val)->type = c->type;
		if(_is_string(c)) {
			char* _str = _extract_string(c);
			(*val)->data.string = mb_strdup(_str, strlen(_str) + 1);
			(*val)->is_ref = false;
		} else {
			(*val)->data = c->data;
		}
	}
	if(guard_val != c && garbage && _ls_try_remove(garbage, c, _ls_cmp_data, 0)) {
		_try_clear_intermediate_value(c, 0, s);

		if(_is_referenced_calc_type(s, c))
			_destroy_object_capsule_only(c, 0);
		else
			_destroy_object(c, 0);
	}

	while(0) {
_error:
		if(garbage) {
			_LS_FOREACH(garbage, _do_nothing_on_object, _remove_if_exists, opnd);
		}
	}

_exit:
	if(garbage) {
		_LS_FOREACH(garbage, _destroy_object, _try_clear_intermediate_value, s);
		_ls_destroy(garbage);
	}
	if(optr) {
		_ls_foreach(optr, _destroy_object_not_compile_time);
		_ls_destroy(optr);
	}
	if(opnd) {
		_ls_foreach(opnd, _destroy_object_not_compile_time);
		_ls_destroy(opnd);
	}
	if(inep) {
		mb_free(_ls_popback(s->in_neg_expr));
	}
	*l = ast;
	mb_set_gc_enabled(s, gce);

	return result;
#undef _LAZY_INIT_GLIST
}

/* Push current variable argument list */
_ls_node_t* _push_var_args(mb_interpreter_t* s) {
	_ls_node_t* result = s->var_args;

	s->var_args = 0;

	return result;
}

/* Pop current variable argument list */
void _pop_var_args(mb_interpreter_t* s, _ls_node_t* last_var_args) {
	_ls_node_t* var_args = s->var_args;

	s->var_args = last_var_args;
	if(var_args) {
		_LS_FOREACH(var_args, _do_nothing_on_object, _destroy_var_arg, &s->gc);
		_ls_destroy(var_args);
	}
}

/* Pop an argument from the caller or a variable argument list */
int _pop_arg(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, unsigned* ia, _routine_t* r, mb_pop_routine_arg_func_t pop_arg, _ls_node_t* args, mb_value_t* arg) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = *l;

	mb_make_nil(*arg);
#if _MULTILINE_STATEMENT
	if(_multiline_statement(s)) {
		_object_t* obj = 0;
		obj = (_object_t*)ast->data;
		while(_IS_EOS(obj)) {
			ast = ast->next;
			obj = ast ? (_object_t*)ast->data : 0;
		}
	}
#endif /* _MULTILINE_STATEMENT */
	if(ast && ast->data && _IS_FUNC(ast->data, _core_args)) {
		if(args) {
			_object_t* obj = (_object_t*)_ls_popfront(args);
			if(obj) {
				_internal_object_to_public_value(obj, arg);
				_destroy_object_capsule_only(obj, 0);
			}
		} else {
			arg->type = MB_DT_UNKNOWN;
		}
	} else {
		result = pop_arg(s, (void**)l, va, ca, ia, r, arg);
	}

	return result;
}

/* Process arguments of a routine */
int _proc_args(mb_interpreter_t* s, _ls_node_t** l, _running_context_t* running, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg, bool_t proc_ref, _ls_node_t* args) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_ls_node_t* parameters = 0;
	mb_value_t arg;
	_ls_node_t* pars = 0;
	_var_t* var = 0;
	_ls_node_t* rnode = 0;
	unsigned ia = 0;
	_ls_node_t* var_args = 0;

	parameters = r->func.basic.parameters;
#ifdef MB_ENABLE_LAMBDA
	if(r->type == MB_RT_LAMBDA)
		parameters = r->func.lambda.parameters;
#endif /* MB_ENABLE_LAMBDA */

	if(parameters) {
		mb_make_nil(arg);
		pars = parameters;
		pars = pars->next;
		while(pars && (!has_arg || (has_arg && has_arg(s, (void**)l, va, ca, &ia, r)))) {
			var = (_var_t*)pars->data;
			pars = pars->next;
			if(_IS_VAR_ARGS(var))
				break;

			if(pop_arg) {
				mb_check(_pop_arg(s, l, va, ca, &ia, r, pop_arg, args, &arg));
#ifdef MB_ENABLE_COLLECTION_LIB
				if(_try_purge_it(s, &arg, 0)) {
					_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
				}
#endif /* MB_ENABLE_COLLECTION_LIB */
			}

			if(running->meta == _SCOPE_META_REF) {
				_object_t* obj = (_object_t*)(_ht_find(running->var_dict, var->name)->data);
				var = obj->data.variable;

				if(proc_ref)
					var->data->is_ref = false;
			} else {
				rnode = _search_identifier_in_scope_chain(s, running, var->name, _PATHING_NONE, 0, 0);
				if(rnode)
					var = ((_object_t*)rnode->data)->data.variable;

				if(proc_ref)
					var->data->is_ref = true;
			}

			if(!pop_arg && var->data->type == _DT_STRING && !var->data->is_ref)
				_mark_lazy_destroy_string(s, var->data->data.string);
			result = _public_value_to_internal_object(&arg, var->data);

			if(result != MB_FUNC_OK)
				break;

			if(args && _ls_empty(args))
				break;
		}

		if(_IS_VAR_ARGS(var)) {
			if(has_arg && !var_args && _IS_VAR_ARGS(var))
				var_args = s->var_args = _ls_create();

			while(has_arg && has_arg(s, (void**)l, va, ca, &ia, r)) {
				if(pop_arg) {
					mb_check(_pop_arg(s, l, va, ca, &ia, r, pop_arg, args, &arg));
				}

				if(var_args) {
					_object_t* obj = _create_object();
					result = _public_value_to_internal_object(&arg, obj);
					if(obj->type == _DT_ROUTINE && obj->data.routine->type == MB_RT_SCRIPT)
						obj->is_ref = true;
					_ls_pushback(var_args, obj);
				}

				if(args && _ls_empty(args))
					break;
			}
		}

		ast = *l;
		if(ast) {
			_object_t* obj = (_object_t*)ast->data;
			if(obj && _IS_FUNC(obj, _core_args)) {
				if(ast) ast = ast->next;
				*l = ast;
			}
		}
	}

#ifdef MB_ENABLE_COLLECTION_LIB
_exit :
#endif /* MB_ENABLE_COLLECTION_LIB */
	return result;
}

/* Evaluate a routine */
int _eval_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg) {
	int result = MB_FUNC_OK;
#ifdef MB_ENABLE_SOURCE_TRACE
	char* src = 0;
#endif /* MB_ENABLE_SOURCE_TRACE */
#if defined MB_ENABLE_STACK_TRACE && defined MB_ENABLE_LAMBDA
	char ln[_LAMBDA_NAME_MAX_LENGTH];
#endif /* MB_ENABLE_STACK_TRACE && MB_ENABLE_LAMBDA */

	_PREVCALL(s, l, r);

#ifdef MB_ENABLE_STACK_TRACE
	_ls_pushback(s->stack_frames, r->name);
#endif /* MB_ENABLE_STACK_TRACE */

#ifdef MB_ENABLE_SOURCE_TRACE
	src = s->source_file;
	s->source_file = r->source_file;
#endif /* MB_ENABLE_SOURCE_TRACE */

	if(r->type == MB_RT_SCRIPT && r->func.basic.entry) {
		result = _eval_script_routine(s, l, va, ca, r, has_arg, pop_arg);
#ifdef MB_ENABLE_LAMBDA
	} else if(r->type == MB_RT_LAMBDA && r->func.lambda.entry) {
#ifdef MB_ENABLE_STACK_TRACE
		_ls_node_t* top = _ls_back(s->stack_frames);
		if(top) {
			sprintf(ln, "LAMBDA_0x%p", (void*)&r->func.lambda.ref);
			top->data = ln;
		}
#endif /* MB_ENABLE_STACK_TRACE */
		result = _eval_lambda_routine(s, l, va, ca, r, has_arg, pop_arg);
#endif /* MB_ENABLE_LAMBDA */
	} else if(r->type == MB_RT_NATIVE && r->func.native.entry) {
		result = _eval_native_routine(s, l, va, ca, r, has_arg, pop_arg);
	} else {
		_handle_error_on_obj(s, SE_RN_INVALID_ROUTINE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

_exit:
#ifdef MB_ENABLE_SOURCE_TRACE
	s->source_file = src;
#endif /* MB_ENABLE_SOURCE_TRACE */

#ifdef MB_ENABLE_STACK_TRACE
	_ls_popback(s->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */

	_POSTCALL(s, l, r);

	return result;
}

/* Evaluate a script routine */
int _eval_script_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_running_context_t* running = 0;
	_routine_t* lastr = 0;
	mb_value_t inte;
	_ls_node_t* lastv = 0;
	bool_t succ = false;
#ifdef MB_ENABLE_CLASS
	bool_t pushed_inst = false;
	_class_t* last_inst = 0;
	bool_t same_inst = s->last_routine ? s->last_routine->instance == r->instance : false;
#else /* MB_ENABLE_CLASS */
	bool_t same_inst = true;
#endif /* MB_ENABLE_CLASS */

	mb_assert(s && l && r);

	if(!va && s->last_routine && !s->last_routine->func.basic.parameters && same_inst && (s->last_routine->name == r->name || !strcmp(s->last_routine->name, r->name))) {
		ast = *l;
		_skip_to(s, &ast, 0, _DT_EOS);
		if(ast)
			obj = (_object_t*)ast->data;
		if(_IS_EOS(obj))
			ast = ast->next;
		if(ast && _IS_FUNC((_object_t*)ast->data, _core_enddef)) { /* Tail recursion optimization */
			*l = r->func.basic.entry;
			if(*l)
				*l = (*l)->next;

			goto _tail;
		}
	}

	lastr = s->last_routine;
	s->last_routine = r;

	lastv = _push_var_args(s);

	if(!va) {
		mb_check(mb_attempt_open_bracket(s, (void**)l));
	}

	running = _push_weak_scope_by_routine(s, r->func.basic.scope, r);
	result = _proc_args(s, l, running, va, ca, r, has_arg, pop_arg, true, lastv);
	if(result != MB_FUNC_OK) {
		if(running->meta == _SCOPE_META_REF)
			_destroy_scope(s, running);
		else
			_pop_weak_scope(s, running);

		goto _error;
	}
	running = _pop_weak_scope(s, running);

	if(!va) {
		_mb_check_mark_exit(mb_attempt_close_bracket(s, (void**)l), result, _error);
	}

	ast = *l;
	_ls_pushback(s->sub_stack, ast);

#ifdef MB_ENABLE_CLASS
	if(r->instance && s->last_instance != r->instance) {
		pushed_inst = true;
		last_inst = s->last_instance;
		s->last_instance = r->instance;
		if(r->instance)
			_push_scope_by_class(s, r->instance->scope);
	}
#endif /* MB_ENABLE_CLASS */

	running = _push_scope_by_routine(s, running);

	*l = r->func.basic.entry;
	if(!(*l)) {
		_handle_error_on_obj(s, SE_RN_INVALID_ROUTINE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

	do {
		result = _execute_statement(s, l, true);
		if(ast == *l) {
			_handle_error_now(s, SE_RN_INVALID_EXPRESSION, s->last_error_file, result);

			goto _exit;
		}
		ast = *l;
		if(result == MB_SUB_RETURN) {
			result = MB_FUNC_OK;

			break;
		}
		if(result == MB_FUNC_SUSPEND) {
			_handle_error_now(s, SE_RN_CANNOT_SUSPEND_HERE, s->last_error_file, result);

			goto _exit;
		}
		if(result != MB_FUNC_OK) {
			if(result >= MB_EXTENDED_ABORT)
				s->last_error = SE_EA_EXTENDED_ABORT;
			_handle_error_now(s, s->last_error, s->last_error_file, result);

			goto _exit;
		}
	} while(ast);

#ifdef MB_ENABLE_CLASS
	_out_of_scope(s, running, r->instance, r, true);
#else /* MB_ENABLE_CLASS */
	_out_of_scope(s, running, 0, r, true);
#endif /* MB_ENABLE_CLASS */

	result = _proc_args(s, l, running, 0, 0, r, 0, 0, false, 0);
	if(result != MB_FUNC_OK)
		goto _exit;

	succ = true;

	mb_make_nil(inte);
	_swap_public_value(&inte, &running->intermediate_value);

	_pop_scope(s, true);

_exit:
	if(!succ)
		_pop_scope(s, true);

#ifdef MB_ENABLE_CLASS
	if(pushed_inst) {
		if(r->instance)
			_pop_scope(s, false);
		s->last_instance = last_inst;
	}
#endif /* MB_ENABLE_CLASS */

	if(succ)
		_assign_public_value(s, &s->running_context->intermediate_value, &inte, false);

_error:
	s->last_routine = lastr;

	_pop_var_args(s, lastv);

_tail:
	return result;
}

#ifdef MB_ENABLE_LAMBDA
/* Evaluate a lambda routine */
int _eval_lambda_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_running_context_t* running = 0;
	_routine_t* lastr = 0;
	mb_value_t inte;
	_ls_node_t* lastv = 0;

	mb_assert(s && l && r);

	lastr = s->last_routine;
	s->last_routine = r;

	lastv = _push_var_args(s);

	if(!va) {
		mb_check(mb_attempt_open_bracket(s, (void**)l));
	}

	running = _link_lambda_scope_chain(s, &r->func.lambda, true);
	if(!running) {
		_handle_error_on_obj(s, SE_RN_INVALID_ROUTINE, s->source_file, DON2(l), MB_FUNC_ERR, _error, result);
	}
	result = _proc_args(s, l, running, va, ca, r, has_arg, pop_arg, true, lastv);
	ast = *l;
	if(result != MB_FUNC_OK) {
		_unlink_lambda_scope_chain(s, &r->func.lambda, true);

		goto _error;
	}
	running = _unlink_lambda_scope_chain(s, &r->func.lambda, true);

	if(!va) {
		_mb_check_mark_exit(mb_attempt_close_bracket(s, (void**)l), result, _error);
	}

	ast = *l;
	_ls_pushback(s->sub_stack, ast);

	running = _link_lambda_scope_chain(s, &r->func.lambda, false);

	*l = r->func.lambda.entry;
	if(!(*l)) {
		_handle_error_on_obj(s, SE_RN_INVALID_ROUTINE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

	do {
		result = _execute_statement(s, l, true);
		ast = *l;
		if(result == MB_SUB_RETURN) {
			result = MB_FUNC_OK;

			break;
		}
		if(result == MB_FUNC_SUSPEND) {
			_handle_error_now(s, SE_RN_CANNOT_SUSPEND_HERE, s->last_error_file, result);

			goto _exit;
		}
		if(result != MB_FUNC_OK) {
			if(result >= MB_EXTENDED_ABORT)
				s->last_error = SE_EA_EXTENDED_ABORT;
			_handle_error_now(s, s->last_error, s->last_error_file, result);

			goto _exit;
		}
	} while(ast);

	_out_of_scope(s, running, 0, r, true);

	result = _proc_args(s, l, running, 0, 0, r, 0, 0, false, 0);
	if(result != MB_FUNC_OK)
		goto _exit;

	mb_make_nil(inte);
	_swap_public_value(&inte, &running->intermediate_value);

	running = _unlink_lambda_scope_chain(s, &r->func.lambda, false);

	_assign_public_value(s, &s->running_context->intermediate_value, &inte, false);

_exit:
	if(result != MB_FUNC_OK)
		_unlink_lambda_scope_chain(s, &r->func.lambda, false);

_error:
	s->last_routine = lastr;

	_pop_var_args(s, lastv);

	*l = ast;

	return result;
}
#endif /* MB_ENABLE_LAMBDA */

/* Evaluate a native routine */
int _eval_native_routine(mb_interpreter_t* s, _ls_node_t** l, mb_value_t* va, unsigned ca, _routine_t* r, mb_has_routine_arg_func_t has_arg, mb_pop_routine_arg_func_t pop_arg) {
	int result = MB_FUNC_OK;
	_routine_t* lastr = 0;
	mb_routine_func_t entry = 0;
	_ls_node_t* lastv = 0;

	mb_assert(s && l && r);

	lastr = s->last_routine;
	s->last_routine = r;

	lastv = _push_var_args(s);

	entry = r->func.native.entry;
	if(!entry) {
		_handle_error_on_obj(s, SE_RN_INVALID_ROUTINE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	result = entry(s, (void**)l, va, ca, r, has_arg, pop_arg);

_exit:
	s->last_routine = lastr;

	_pop_var_args(s, lastv);

	return result;
}

/* Detect if there is any more lexical argument */
int _has_routine_lex_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r) {
	mb_unrefvar(va);
	mb_unrefvar(ca);
	mb_unrefvar(ia);
	mb_unrefvar(r);

	return mb_has_arg(s, l);
}

/* Pop a lexical argument */
int _pop_routine_lex_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r, mb_value_t* val) {
	mb_unrefvar(va);
	mb_unrefvar(ca);
	mb_unrefvar(ia);
	mb_unrefvar(r);

	return mb_pop_value(s, l, val);
}

/* Detect if there is any more argument in the argument list */
int _has_routine_fun_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r) {
	mb_unrefvar(s);
	mb_unrefvar(l);
	mb_unrefvar(va);
	mb_unrefvar(r);

	return *ia < ca;
}

/* Pop an argument from the argument list */
int _pop_routine_fun_arg(mb_interpreter_t* s, void** l, mb_value_t* va, unsigned ca, unsigned* ia, void* r, mb_value_t* val) {
	mb_unrefvar(s);
	mb_unrefvar(l);
	mb_unrefvar(ca);
	mb_unrefvar(r);

	memcpy(val, &(va[*ia]), sizeof(mb_value_t));
	(*ia)++;

	return MB_FUNC_OK;
}

/* Determine whether an object is a PRINT termination */
bool_t _is_print_terminal(mb_interpreter_t* s, _object_t* obj) {
	bool_t result = false;

	mb_assert(s && obj);

	result = (
		_IS_EOS(obj) ||
		_IS_SEP(obj, ':') ||
		_IS_FUNC(obj, _core_elseif) ||
		_IS_FUNC(obj, _core_else) ||
		_IS_FUNC(obj, _core_endif)
	);

	return result;
}

/* Try to call overridden function */
mb_meta_status_e _try_overridden(mb_interpreter_t* s, void** l, mb_value_t* d, const char* f, mb_meta_func_e t) {
	mb_assert(s && l && d && f);

#ifdef MB_ENABLE_USERTYPE_REF
	if(d->type == MB_DT_USERTYPE_REF) {
		_object_t obj;
		_MAKE_NIL(&obj);
		_public_value_to_internal_object(d, &obj);
		if(t == MB_MF_COLL && obj.data.usertype_ref->coll_func)
			return obj.data.usertype_ref->coll_func(s, l, d, f);
		else if(t == MB_MF_FUNC && obj.data.usertype_ref->generic_func)
			return obj.data.usertype_ref->generic_func(s, l, d, f);
	}
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_CLASS
	if(d->type == MB_DT_CLASS) {
		char buf[_TEMP_FORMAT_MAX_LENGTH];
		_ls_node_t* ofn = 0;
		_object_t obj;
		_MAKE_NIL(&obj);
		_public_value_to_internal_object(d, &obj);
		sprintf(buf, _CLASS_OVERRIDE_FMT, f);
		ofn = _search_identifier_in_class(s, obj.data.instance, buf, 0, 0);
		if(ofn) {
			_object_t* ofo = (_object_t*)ofn->data;
			_ls_node_t* ast = (_ls_node_t*)*l;
			mb_value_t va[1];
			mb_make_nil(va[0]);
			if(_eval_routine(s, &ast, va, 0, ofo->data.routine, _has_routine_lex_arg, _pop_routine_lex_arg) == MB_FUNC_OK) {
				if(ast)
					*l = ast->prev;

				return (mb_meta_status_e)(MB_MS_DONE | MB_MS_RETURNED);
			}
		}
	}
#endif /* MB_ENABLE_CLASS */
#if !defined MB_ENABLE_USERTYPE_REF || !defined MB_ENABLE_CLASS
	mb_unrefvar(t);
#endif /* !MB_ENABLE_USERTYPE_REF && !MB_ENABLE_CLASS */

	return MB_MS_NONE;
}

/** Handlers */

/* Set current error information */
bool_t _set_current_error(mb_interpreter_t* s, mb_error_e err, char* f) {
	mb_assert(s && err >= 0);

	if(s->last_error == SE_NO_ERR) {
		s->last_error = err;
		s->last_error_file = f;

		return true;
	}

	return false;
}

/* Get a print functor of an interpreter */
mb_print_func_t _get_printer(mb_interpreter_t* s) {
	mb_assert(s);

	if(s->printer)
		return s->printer;

	return _standard_printer;
}

/* Get an input functor of an interpreter */
mb_input_func_t _get_inputer(mb_interpreter_t* s) {
	mb_assert(s);

	if(s->inputer)
		return s->inputer;

	return mb_gets;
}

/* Standard printer adapter */
int _standard_printer(mb_interpreter_t* s, const char* fmt, ...) {
	int result;
	va_list args;
	mb_unrefvar(s);

	va_start(args, fmt);
	result = vprintf(fmt, args);
	va_end(args);

	return result;
}

/* Print a string */
void _print_string(mb_interpreter_t* s, _object_t* obj) {
#if defined MB_CP_VC && defined MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING
	char* str = 0;
	_dynamic_buffer_t buf;
	size_t lbuf = 0;

	mb_assert(s && obj);

	str = obj->data.string ? obj->data.string : MB_NULL_STRING;
	_INIT_BUF(buf);
	while((lbuf = (size_t)mb_bytes_to_wchar(str, &_WCHAR_BUF_PTR(buf), _WCHARS_OF_BUF(buf))) > _WCHARS_OF_BUF(buf)) {
		_RESIZE_WCHAR_BUF(buf, lbuf);
	}
	_get_printer(s)(s, "%ls", _WCHAR_BUF_PTR(buf));
	_DISPOSE_BUF(buf);
#else /* MB_CP_VC && MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING */
	mb_assert(s && obj);

	_get_printer(s)(s, "%s", obj->data.string ? obj->data.string : MB_NULL_STRING);
#endif /* MB_CP_VC && MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING */
}

/** Parsing helpers */

/* Read all content of a file into a buffer */
char* _load_file(mb_interpreter_t* s, const char* f, const char* prefix, bool_t importing) {
#ifndef MB_DISABLE_LOAD_FILE
	FILE* fp = 0;
	char* buf = 0;
	long curpos = 0;
	long l = 0;
	long i = 0;
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = (_parsing_context_t*)s->parsing_context;

	if(_ls_find(context->imported, (void*)f, (_ls_compare_t)_ht_cmp_string, 0)) {
		buf = (char*)f;
	} else {
		fp = fopen(f, "rb");
		if(fp) {
			if(importing) {
				buf = mb_strdup(f, strlen(f) + 1);
				_ls_pushback(context->imported, buf);
				buf = 0;
			}

			curpos = ftell(fp);
			fseek(fp, 0L, SEEK_END);
			l = ftell(fp);
			fseek(fp, curpos, SEEK_SET);
			if(prefix) {
				i = (long)strlen(prefix);
				l += i;
			}
			buf = (char*)mb_malloc((size_t)(l + 1));
			mb_assert(buf);
			if(prefix)
				memcpy(buf, prefix, i);
			fread(buf + i, 1, l, fp);
			do {
				char* off = buf + i;
				int b = mb_uu_getbom((const char**)&off);
				if(b) {
					memmove(buf + i, buf + i + b, l - b - i);
					buf[l - b] = _ZERO_CHAR;
				}
			} while(0);
			fclose(fp);
			buf[l] = _ZERO_CHAR;
		}
	}

	return buf;
#else /* MB_DISABLE_LOAD_FILE */
	return 0;
#endif /* MB_DISABLE_LOAD_FILE */
}

/* Finish loading a file */
void _end_of_file(_parsing_context_t* context) {
	if(context)
		context->parsing_state = _PS_NORMAL;
}

/* Determine whether a character is blank */
bool_t _is_blank_char(char c) {
	return (c == ' ') || (c == '\t');
}

/* Determine whether a character is end of file */
bool_t _is_eof_char(char c) {
#ifdef __cplusplus
	union { signed char s; char c; } u;
	u.c = c;

	return u.s == EOF;
#else /* __cplusplus */
	return (c == EOF);
#endif /* __cplusplus */
}

/* Determine whether a character is newline */
bool_t _is_newline_char(char c) {
	return (c == _RETURN_CHAR) || (c == _NEWLINE_CHAR) || _is_eof_char(c);
}

/* Determine whether a character is separator */
bool_t _is_separator_char(char c) {
	return (c == ',') || (c == ';') || (c == ':');
}

/* Determine whether a character is bracket */
bool_t _is_bracket_char(char c) {
	return (c == '(') || (c == ')');
}

/* Determine whether a character is quotation mark */
bool_t _is_quotation_char(char c) {
	return (c == '"');
}

/* Determine whether a character is comment mark */
bool_t _is_comment_char(char c) {
	return (c == '\'');
}

/* Determine whether a character is accessor char */
bool_t _is_accessor_char(char c) {
	return (c == '.');
}

/* Determine whether a character is numeric char */
bool_t _is_numeric_char(char c) {
	return (c >= '0' && c <= '9') || _is_accessor_char(c);
}

/* Determine whether a character is identifier char */
bool_t _is_identifier_char(char c) {
#if defined MB_ENABLE_LAMBDA && defined MB_LAMBDA_ALIAS
	char* p = MB_LAMBDA_ALIAS;
	while(*p) {
		if(c == *p) return true;
		++p;
	}
#endif /* MB_ENABLE_LAMBDA && MB_LAMBDA_ALIAS */

	return (
		(c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
		(c == '_') ||
		_is_numeric_char(c) ||
		(c == _STRING_POSTFIX_CHAR)
	);
}

/* Determine whether a character is operator char */
bool_t _is_operator_char(char c) {
	return (
		(c == '+') || (c == '-') || (c == '*') || (c == '/') ||
		(c == '^') ||
		(c == '(') || (c == ')') ||
		(c == '=') ||
		(c == '>') || (c == '<')
	);
}

/* Determine whether a character is exponential char */
bool_t _is_exponential_char(char c) {
	return (c == 'e') || (c == 'E');
}

/* Determine whether a character is module using char */
bool_t _is_using_at_char(char c) {
	return (c == '@');
}

/* Determine whether current symbol is exponent prefix */
bool_t _is_exponent_prefix(char* s, int begin, int end) {
	int i = 0;

	mb_assert(s);

	if(end < 0)
		return false;

	for(i = begin; i <= end; i++) {
		if(!_is_numeric_char(s[i]))
			return false;
	}

	return true;
}

/* Parse a character and append it to current parsing symbol */
int _append_char_to_symbol(mb_interpreter_t* s, char c) {
	int result = MB_FUNC_OK;
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;

	if(_is_accessor_char(c))
		context->current_symbol_contains_accessor++;

	if(context->current_symbol_nonius + 1 >= _SINGLE_SYMBOL_MAX_LENGTH) {
		_set_current_error(s, SE_PS_SYMBOL_TOO_LONG, 0);

		result = MB_FUNC_ERR;
	} else {
		context->current_symbol[context->current_symbol_nonius] = c;
		++context->current_symbol_nonius;
	}

	return result;
}

#ifdef MB_ENABLE_UNICODE_ID
/* Parse a UTF8 character and append it to current parsing symbol */
int _append_uu_char_to_symbol(mb_interpreter_t* s, const char* str, int n) {
	int result = MB_FUNC_OK;
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;

	if(context->current_symbol_nonius + n >= _SINGLE_SYMBOL_MAX_LENGTH) {
		_set_current_error(s, SE_PS_SYMBOL_TOO_LONG, 0);

		result = MB_FUNC_ERR;
	} else {
		memcpy(&context->current_symbol[context->current_symbol_nonius], str, n);
		context->current_symbol_nonius += n;
	}

	return result;
}
#endif /* MB_ENABLE_UNICODE_ID */

/* Cut current symbol when current one parsing is finished */
int _cut_symbol(mb_interpreter_t* s, int pos, unsigned short row, unsigned short col) {
	int result = MB_FUNC_OK;
	_parsing_context_t* context = 0;
	char* sym = 0;
	bool_t delsym = false;

	mb_assert(s);

	context = s->parsing_context;
	if(context->current_symbol_nonius && context->current_symbol[0] != _ZERO_CHAR) {
		sym = (char*)mb_malloc(context->current_symbol_nonius + 1);
		memcpy(sym, context->current_symbol, context->current_symbol_nonius + 1);

		result = _append_symbol(s, sym, &delsym, pos, row, col);
		if(result != MB_FUNC_OK || delsym) {
			safe_free(sym);
		}
	}
	memset(context->current_symbol, 0, sizeof(context->current_symbol));
	context->current_symbol_nonius = 0;
	context->current_symbol_contains_accessor = 0;

	return result;
}

/* Append cut symbol to the AST list */
int _append_symbol(mb_interpreter_t* s, char* sym, bool_t* delsym, int pos, unsigned short row, unsigned short col) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_ls_node_t** assign = 0;
	_ls_node_t* node = 0;
	_parsing_context_t* context = 0;

	mb_assert(s && sym);

	ast = s->ast;
	result = _create_symbol(s, ast, sym, &obj, &assign, delsym);
	if(obj) {
#ifdef MB_ENABLE_SOURCE_TRACE
		obj->source_pos = pos;
		obj->source_row = row;
		obj->source_col = col;
#else /* MB_ENABLE_SOURCE_TRACE */
		mb_unrefvar(row);
		mb_unrefvar(col);

		obj->source_pos = (char)!!pos;
#endif /* MB_ENABLE_SOURCE_TRACE */

		node = _ls_pushback(ast, obj);
		if(assign)
			*assign = node;

		context = s->parsing_context;
		context->last_symbol = obj;
	}

	return result;
}

/* Create a syntax symbol */
int _create_symbol(mb_interpreter_t* s, _ls_node_t* l, char* sym, _object_t** obj, _ls_node_t*** asgn, bool_t* delsym) {
	int result = MB_FUNC_OK;
	_data_e type;
	union {
		_func_t* func; _array_t* array;
#ifdef MB_ENABLE_CLASS
		_class_t* instance;
#endif /* MB_ENABLE_CLASS */
		_routine_t* routine; _var_t* var; _label_t* label; real_t float_point; int_t integer; _raw_t any;
	} tmp;
	_raw_t value;
	unsigned ul = 0;
	_parsing_context_t* context = 0;
	_running_context_t* running = 0;
	_ls_node_t* glbsyminscope = 0;
	bool_t is_field = false;
	mb_unrefvar(l);

	mb_assert(s && sym && obj);

	memset(value, 0, sizeof(_raw_t));

	context = s->parsing_context;
	running = s->running_context;

	*obj = _create_object();
#ifdef MB_ENABLE_SOURCE_TRACE
	(*obj)->source_pos = -1;
	(*obj)->source_row = (*obj)->source_col = 0xFFFF;
#else /* MB_ENABLE_SOURCE_TRACE */
	(*obj)->source_pos = -1;
#endif /* MB_ENABLE_SOURCE_TRACE */

	type = _get_symbol_type(s, sym, &value);
	if(s->last_error != SE_NO_ERR) {
		result = MB_FUNC_ERR;

		goto _exit;
	}
	(*obj)->type = type;
	switch(type) {
	case _DT_NIL:
		memcpy(tmp.any, value, sizeof(_raw_t));
		if(tmp.integer) { /* Nil type */
			(*obj)->type = _DT_NIL;
		} else { /* End of line character */
			safe_free(*obj);
		}
		safe_free(sym);

		break;
	case _DT_INT:
		memcpy(tmp.any, value, sizeof(_raw_t));
		(*obj)->data.integer = tmp.integer;
		safe_free(sym);

		break;
	case _DT_REAL:
		memcpy(tmp.any, value, sizeof(_raw_t));
		(*obj)->data.float_point = tmp.float_point;
		safe_free(sym);

		break;
	case _DT_STRING: {
			size_t _sl = strlen(sym);
			(*obj)->data.string = (char*)mb_malloc(_sl - 2 + 1);
			memcpy((*obj)->data.string, sym + sizeof(char), _sl - 2);
			(*obj)->data.string[_sl - 2] = _ZERO_CHAR;
			*delsym = true;
		}

		break;
	case _DT_FUNC:
		tmp.func = (_func_t*)mb_malloc(sizeof(_func_t));
		memset(tmp.func, 0, sizeof(_func_t));
		tmp.func->name = sym;
		memcpy(&tmp.func->pointer, value, sizeof(tmp.func->pointer));
		(*obj)->data.func = tmp.func;

		break;
	case _DT_ARRAY:
		glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
		if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_ARRAY) {
			(*obj)->data.array = ((_object_t*)glbsyminscope->data)->data.array;
			(*obj)->is_ref = true;
			*delsym = true;
		} else {
			tmp.array = _create_array(s, sym, _DT_UNKNOWN);
			memcpy(&tmp.array->type, value, sizeof(tmp.array->type));
			(*obj)->data.array = tmp.array;

			ul = _ht_set_or_insert(running->var_dict, sym, *obj);
			mb_assert(ul);

			*obj = _create_object();
			(*obj)->type = type;
			(*obj)->data.array = tmp.array;
			(*obj)->is_ref = true;
		}

		break;
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		if(!_is_identifier_char(*sym))
			*sym = _INVALID_CLASS_CHAR;
		glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
		if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_CLASS) {
			(*obj)->data.instance = ((_object_t*)glbsyminscope->data)->data.instance;
			(*obj)->is_ref = true;
			*delsym = true;
			if(running != (*obj)->data.instance->scope &&
				(context->class_state != _CLASS_STATE_NONE) &&
				_IS_FUNC(context->last_symbol, _core_class)) {
				_push_scope_by_class(s, (*obj)->data.instance->scope);
			}
		} else {
			tmp.instance = (_class_t*)mb_malloc(sizeof(_class_t));
			_init_class(s, tmp.instance, sym);
			_push_scope_by_class(s, tmp.instance->scope);
			s->last_instance = tmp.instance;

			(*obj)->data.instance = tmp.instance;

			ul = _ht_set_or_insert(running->var_dict, sym, *obj);
			mb_assert(ul);

			*obj = _create_object();
			(*obj)->type = type;
			(*obj)->data.instance = tmp.instance;
			(*obj)->is_ref = true;
		}

		break;
#endif /* MB_ENABLE_CLASS */
	case _DT_ROUTINE:
		if(!_is_identifier_char(*sym))
			*sym = _INVALID_ROUTINE_CHAR;
		glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
		if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_ROUTINE) {
			(*obj)->data.routine = ((_object_t*)glbsyminscope->data)->data.routine;
			(*obj)->is_ref = true;
			*delsym = true;
			if(running != (*obj)->data.routine->func.basic.scope &&
				context->routine_state &&
				_IS_FUNC(context->last_symbol, _core_def)) {
				_push_scope_by_routine(s, (*obj)->data.routine->func.basic.scope);
			}
		} else {
			_running_context_t* tba = 0;
			tmp.routine = (_routine_t*)mb_malloc(sizeof(_routine_t));
			_init_routine(s, tmp.routine, sym, 0);
			_push_scope_by_routine(s, tmp.routine->func.basic.scope);
			(*obj)->data.routine = tmp.routine;

			tba = _get_scope_to_add_routine(s);
			ul = _ht_set_or_insert(tba->var_dict, sym, *obj);
			mb_assert(ul);
			if(tba != _OUTTER_SCOPE(running) && tba != running)
				_pop_scope(s, false);

			*obj = _create_object();
			(*obj)->type = type;
			(*obj)->data.routine = tmp.routine;
			(*obj)->is_ref = true;

#ifdef MB_ENABLE_CLASS
			tmp.routine->instance = s->last_instance;
#endif /* MB_ENABLE_CLASS */
		}

		break;
	case _DT_VAR:
		if(context->routine_params_state == _ROUTINE_STATE_PARAMS)
			glbsyminscope = _ht_find(running->var_dict, sym);
		else
			glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
#ifdef MB_ENABLE_CLASS
		is_field = context->last_symbol && _IS_FUNC(context->last_symbol, _core_var);
#endif /* MB_ENABLE_CLASS */
		if(!is_field && glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_VAR) {
			(*obj)->data.variable = ((_object_t*)glbsyminscope->data)->data.variable;
			(*obj)->is_ref = true;
			*delsym = true;
		} else {
#ifdef MB_ENABLE_CLASS
			if(strcmp(sym, _CLASS_ME) == 0) {
				_handle_error_now(s, SE_RN_INVALID_ID_USAGE, s->source_file, MB_FUNC_ERR);
				(*obj)->is_ref = true;
				*delsym = true;

				goto _exit;
			}
#endif /* MB_ENABLE_CLASS */
			tmp.var = (_var_t*)mb_malloc(sizeof(_var_t));
			memset(tmp.var, 0, sizeof(_var_t));
			tmp.var->name = sym;
			tmp.var->data = _create_object();
			tmp.var->data->type = (sym[strlen(sym) - 1] == _STRING_POSTFIX_CHAR) ? _DT_STRING : _DT_INT;
			tmp.var->data->data.integer = 0;
#ifdef MB_ENABLE_CLASS
			if(context->class_state != _CLASS_STATE_NONE)
				tmp.var->pathing = _PATHING_NORMAL;
			else if(!is_field)
				tmp.var->pathing = context->current_symbol_contains_accessor ? _PATHING_NORMAL : _PATHING_NONE;
#endif /* MB_ENABLE_CLASS */
			(*obj)->data.variable = tmp.var;

			ul = _ht_set_or_insert(running->var_dict, sym, *obj);
			mb_assert(ul);

			*obj = _create_object();
			(*obj)->type = type;
			(*obj)->data.variable = tmp.var;
			(*obj)->is_ref = true;
		}

		break;
	case _DT_LABEL:
		if(context->current_char == ':') {
			if(mb_memtest(value, sizeof(_raw_t))) {
				memcpy(&((*obj)->data.label), value, sizeof((*obj)->data.label));
				(*obj)->is_ref = true;
				*delsym = true;
			} else {
				tmp.label = (_label_t*)mb_malloc(sizeof(_label_t));
				memset(tmp.label, 0, sizeof(_label_t));
				tmp.label->name = sym;
				*asgn = &(tmp.label->node);
				(*obj)->data.label = tmp.label;

				ul = _ht_set_or_insert(running->var_dict, sym, *obj);
				mb_assert(ul);

				*obj = _create_object();
				(*obj)->type = type;
				(*obj)->data.label = tmp.label;
				(*obj)->is_ref = true;
			}
		} else {
			(*obj)->data.label = (_label_t*)mb_malloc(sizeof(_label_t));
			memset((*obj)->data.label, 0, sizeof(_label_t));
			(*obj)->data.label->name = sym;
		}

		break;
	case _DT_SEP:
		(*obj)->data.separator = sym[0];
		safe_free(sym);

		break;
	case _DT_EOS:
		safe_free(sym);

		break;
	default: /* Do nothing */
		break;
	}

_exit:
	return result;
}

/* Get the type of a syntax symbol */
_data_e _get_symbol_type(mb_interpreter_t* s, char* sym, _raw_t* value) {
	_data_e result = _DT_NIL;
	union { real_t float_point; int_t integer; _object_t* obj; _raw_t any; } tmp;
	char* conv_suc = 0;
	_parsing_context_t* context = 0;
	_running_context_t* running = 0;
	_ls_node_t* glbsyminscope = 0;
	size_t _sl = 0;
	_data_e en = _DT_UNKNOWN;
	intptr_t ptr = 0;
	bool_t mod = false;

	mb_assert(s && sym);
	_sl = strlen(sym);
	mb_assert(_sl > 0);

	context = s->parsing_context;
	running = s->running_context;

	/* int_t */
	tmp.integer = (int_t)mb_strtol(sym, &conv_suc, 0);
	if(*conv_suc == _ZERO_CHAR) {
		memcpy(*value, tmp.any, sizeof(_raw_t));

		result = _DT_INT;

		goto _exit;
	}
	/* real_t */
	tmp.float_point = (real_t)mb_strtod(sym, &conv_suc);
	if(*conv_suc == _ZERO_CHAR) {
		memcpy(*value, tmp.any, sizeof(_raw_t));

		result = _DT_REAL;

		goto _exit;
	}
	/* String */
	if(_is_quotation_char(sym[0]) && _is_quotation_char(sym[_sl - 1]) && _sl >= 2) {
		result = _DT_STRING;

		if(context->last_symbol && _IS_FUNC(context->last_symbol, _core_import)) {
			/* IMPORT statement */
			int n = context->current_symbol_nonius;
			char current_symbol[_SINGLE_SYMBOL_MAX_LENGTH + 1];
			char* buf = 0;
			memcpy(current_symbol, context->current_symbol, sizeof(current_symbol));
			memset(context->current_symbol, 0, sizeof(current_symbol));
			context->current_symbol_nonius = 0;
			context->last_symbol = 0;
			sym[_sl - 1] = _ZERO_CHAR;
			context->parsing_state = _PS_NORMAL;
			/* Using a module */
			if(_is_using_at_char(*(sym + 1))) {
#ifdef MB_ENABLE_MODULE
				char* ns = mb_strdup(sym + 2, strlen(sym + 2) + 1);
				mb_strupr(ns);
				if(_ls_find(s->using_modules, ns, (_ls_compare_t)_ht_cmp_string, 0)) {
					safe_free(ns);
				} else {
					_ls_pushback(s->using_modules, ns);
				}

				goto _end_import;
#else /* MB_ENABLE_MODULE */
				_handle_error_now(s, SE_CM_NOT_SUPPORTED, s->source_file, MB_FUNC_ERR);

				goto _end_import;
#endif /* MB_ENABLE_MODULE */
			}
			/* Import another file */
			buf = _load_file(s, sym + 1, ":", true);
			if(buf) {
				if(buf == sym + 1) {
					_handle_error_now(s, SE_PS_DUPLICATE_IMPORT, s->source_file, MB_FUNC_WARNING);
				} else {
					char* lf = (char*)(_ls_back(context->imported)->data);
					int pos = 0; unsigned short row = 0, col = 0;
					lf = _prev_import(s, lf, &pos, &row, &col);
					mb_load_string(s, buf, true);
					safe_free(buf);
					_post_import(s, lf, &pos, &row, &col);
				}
			} else {
				if(!_ls_find(context->imported, (void*)(sym + 1), (_ls_compare_t)_ht_cmp_string, 0)) {
					if(s->import_handler) {
						_object_t* sep = 0;
						char* lf = 0;
						int pos = 0; unsigned short row = 0, col = 0;
						sep = _create_object();
						sep->type = _DT_SEP;
						sep->data.separator = ':';
						_ls_pushback(s->ast, sep);
						_ls_pushback(context->imported, mb_strdup(sym + 1, strlen(sym + 1) + 1));
						lf = (char*)(_ls_back(context->imported)->data);
						lf = _prev_import(s, lf, &pos, &row, &col);
						if(s->import_handler(s, sym + 1) != MB_FUNC_OK) {
							_ls_node_t* last = _ls_back(context->imported);
							if(s->last_error == SE_NO_ERR) {
								context->parsing_pos = pos;
								context->parsing_row = row;
								context->parsing_col = col;
								_handle_error_now(s, SE_PS_FAILED_TO_OPEN_FILE, lf, MB_FUNC_ERR);
							}
							_destroy_memory(last->data, last->extra);
							_ls_popback(context->imported);
						}
						_post_import(s, lf, &pos, &row, &col);
					} else {
						_handle_error_now(s, SE_PS_FAILED_TO_OPEN_FILE, s->source_file, MB_FUNC_ERR);
					}
				}
			}

_end_import:
			context->parsing_state = _PS_STRING;
			sym[_sl - 1] = '"';
			context->current_symbol_nonius = n;
			memcpy(context->current_symbol, current_symbol, sizeof(current_symbol));
			result = _DT_NIL;
		}

		goto _exit;
	}
	/* Nil */
	if(!strcmp(sym, MB_NIL)) {
		tmp.integer = ~0;
		memcpy(*value, tmp.any, sizeof(_raw_t));

		result = _DT_NIL;

		goto _exit;
	}
	/* REM */
	if(!strcmp(sym, _REMARK_STR)) {
		context->parsing_state = _PS_COMMENT;

		result = _DT_EOS;

		goto _exit;
	}
	/* _array_t */
	glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
	if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_ARRAY) {
		tmp.obj = (_object_t*)glbsyminscope->data;
		memcpy(*value, &(tmp.obj->data.array->type), sizeof(tmp.obj->data.array->type));

		result = _DT_ARRAY;

		goto _exit;
	}
	if(context->last_symbol && _IS_FUNC(context->last_symbol, _core_dim)) {
#ifdef MB_SIMPLE_ARRAY
		en = (sym[_sl - 1] == _STRING_POSTFIX_CHAR ? _DT_STRING : _DT_REAL);
#else /* MB_SIMPLE_ARRAY */
		en = _DT_REAL;
#endif /* MB_SIMPLE_ARRAY */
		memcpy(*value, &en, sizeof(en));

		result = _DT_ARRAY;

		goto _exit;
	}
	/* _class_t */
#ifdef MB_ENABLE_CLASS
	if(context->last_symbol) {
		glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
		if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_ROUTINE)
			goto _routine;
		if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_CLASS) {
			if(_IS_FUNC(context->last_symbol, _core_class)) {
				_handle_error_now(s, SE_RN_DUPLICATE_CLASS, s->source_file, MB_FUNC_ERR);
			}
			result = _DT_CLASS;

			goto _exit;
		}
		if(_IS_FUNC(context->last_symbol, _core_class)) {
			if(s->last_instance) {
				_handle_error_now(s, SE_RN_DUPLICATE_CLASS, s->source_file, MB_FUNC_ERR);

				goto _exit;
			}
			_begin_class(s);
#ifdef MB_ENABLE_UNICODE_ID
			if(!_is_identifier_char(sym[0]) && !mb_uu_ischar(sym)) {
#else /* MB_ENABLE_UNICODE_ID */
			if(!_is_identifier_char(sym[0])) {
#endif /* MB_ENABLE_UNICODE_ID */
				result = _DT_NIL;

				goto _exit;
			}
			if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_VAR) {
				_handle_error_now(s, SE_RN_INVALID_CLASS, s->source_file, MB_FUNC_ERR);

				goto _exit;
			}

			if(context->routine_state > 1) {
				_handle_error_now(s, SE_RN_INVALID_CLASS, s->source_file, MB_FUNC_ERR);

				goto _exit;
			}

			result = _DT_CLASS;

			goto _exit;
		} else if(_IS_FUNC(context->last_symbol, _core_endclass)) {
			if(_end_class(s))
				_pop_scope(s, false);
		}
	}
_routine:
#endif /* MB_ENABLE_CLASS */
	/* _routine_t */
	if(context->last_symbol && !_is_bracket_char(sym[0])) {
		glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
		if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_ROUTINE) {
			if(_IS_FUNC(context->last_symbol, _core_def)) {
				if(_begin_routine(s) != MB_FUNC_OK)
					goto _exit;
			}
			result = _DT_ROUTINE;

			goto _exit;
		}
		if(_IS_FUNC(context->last_symbol, _core_def) || _IS_FUNC(context->last_symbol, _core_call)) {
			if(_IS_FUNC(context->last_symbol, _core_def)) {
				if(_begin_routine(s) != MB_FUNC_OK)
					goto _exit;
			}
#ifdef MB_ENABLE_UNICODE_ID
			if(!_is_identifier_char(sym[0]) && !mb_uu_ischar(sym)) {
#else /* MB_ENABLE_UNICODE_ID */
			if(!_is_identifier_char(sym[0])) {
#endif /* MB_ENABLE_UNICODE_ID */
				result = _DT_NIL;

				goto _exit;
			}
			if(glbsyminscope && ((_object_t*)glbsyminscope->data)->type == _DT_VAR) {
				_handle_error_now(s, SE_RN_INVALID_ROUTINE, s->source_file, MB_FUNC_ERR);

				goto _exit;
			}

			if(_IS_FUNC(context->last_symbol, _core_def)) {
				if(context->routine_state > 1) {
					_handle_error_now(s, SE_RN_INVALID_ROUTINE, s->source_file, MB_FUNC_ERR);

					goto _exit;
				}
			}

			result = _DT_ROUTINE;

			goto _exit;
		} else if(_IS_FUNC(context->last_symbol, _core_enddef)) {
			if(_end_routine(s))
				_pop_scope(s, false);
		}
	}
	/* _func_t */
	if(!context->last_symbol ||
		(context->last_symbol && ((context->last_symbol->type == _DT_FUNC && context->last_symbol->data.func->pointer != _core_close_bracket) ||
		context->last_symbol->type == _DT_SEP || context->last_symbol->type == _DT_EOS))) {
		if(strcmp("-", sym) == 0) {
			ptr = (intptr_t)_core_neg;
			memcpy(*value, &ptr, sizeof(intptr_t));

			result = _DT_FUNC;

			goto _exit;
		}
	}
	glbsyminscope = _find_func(s, sym, &mod);
	if(glbsyminscope) {
		if(context->last_symbol && context->last_symbol->type == _DT_ROUTINE) {
			if(_sl == 1 && sym[0] == '(') {
				if(context->routine_params_state == _ROUTINE_STATE_DEF)
					_begin_routine_parameter_list(s);
			}
		} else if(context->routine_params_state == _ROUTINE_STATE_PARAMS) {
			if(_sl == 1 && sym[0] == ')')
				_end_routine_parameter_list(s);
		}

#ifdef MB_ENABLE_MODULE
		if(mod) {
			_module_func_t* mp = (_module_func_t*)glbsyminscope->data;
			ptr = (intptr_t)mp->func;
			memcpy(*value, &ptr, sizeof(intptr_t));
		} else {
			ptr = (intptr_t)glbsyminscope->data;
			memcpy(*value, &ptr, sizeof(intptr_t));
			if(ptr == (intptr_t)_core_def)
				_begin_routine_definition(s);
		}
#else /* MB_ENABLE_MODULE */
		ptr = (intptr_t)glbsyminscope->data;
		memcpy(*value, &ptr, sizeof(intptr_t));
		if(ptr == (intptr_t)_core_def)
			_begin_routine_definition(s);
#endif /* MB_ENABLE_MODULE */

		result = _DT_FUNC;

		goto _exit;
	}
	/* MB_EOS */
	if(_sl == 1 && sym[0] == MB_EOS) {
		if(_IS_EOS(context->last_symbol))
			result = _DT_NIL;
		else
			result = _DT_EOS;

		goto _exit;
	}
	/* Separator */
	if(_sl == 1 && _is_separator_char(sym[0])) {
		result = _DT_SEP;

		goto _exit;
	}
	/* _var_t */
	glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
	if(glbsyminscope) {
		if(((_object_t*)glbsyminscope->data)->type != _DT_LABEL) {
			memcpy(*value, &glbsyminscope->data, sizeof(glbsyminscope->data));

			result = _DT_VAR;

			goto _exit;
		}
	}
	/* _label_t */
	if(context->current_char == ':') {
		if(!context->last_symbol || _IS_EOS(context->last_symbol)) {
			glbsyminscope = _search_identifier_in_scope_chain(s, 0, sym, _PATHING_NONE, 0, 0);
			if(glbsyminscope)
				memcpy(*value, &glbsyminscope->data, sizeof(glbsyminscope->data));

			result = _DT_LABEL;

			goto _exit;
		}
	}
	if(context->last_symbol && (_IS_FUNC(context->last_symbol, _core_goto) || _IS_FUNC(context->last_symbol, _core_gosub))) {
		result = _DT_LABEL;

		goto _exit;
	}
	/* Otherwise */
	result = _DT_VAR;

_exit:
	return result;
}

/* Parse a character */
int _parse_char(mb_interpreter_t* s, const char* str, int n, int pos, unsigned short row, unsigned short col) {
	int result = MB_FUNC_OK;
	_parsing_context_t* context = 0;
	char last_char = _ZERO_CHAR;
	char c = _ZERO_CHAR;
#ifdef MB_ENABLE_UNICODE_ID
	unsigned uc = 0;
#else /* MB_ENABLE_UNICODE_ID */
	mb_unrefvar(n);
#endif /* MB_ENABLE_UNICODE_ID */

	mb_assert(s && s->parsing_context);

	context = s->parsing_context;

	if(str) {
#ifdef MB_ENABLE_UNICODE_ID
		if(n == 0)
			result = MB_FUNC_ERR;
		else if(n == 1)
			c = *str;
		else
			memcpy(&uc, str, n);
#else /* MB_ENABLE_UNICODE_ID */
		c = *str;
#endif /* MB_ENABLE_UNICODE_ID */
	} else {
		c = MB_EOS;
	}

	last_char = context->current_char;
	context->current_char = c;

	switch(context->parsing_state) {
	case _PS_NORMAL:
#ifdef MB_ENABLE_UNICODE_ID
		if(uc) {
			if(n == countof(_UNICODE_OPEN_QUOTE)) {
				if(memcmp(str, _UNICODE_OPEN_QUOTE, n) == 0) {
					_handle_error_at_pos(s, SE_PS_INVALID_CHAR, s->source_file, pos, row, col, MB_FUNC_ERR, _exit, result);
				}
			}
			if(n == countof(_UNICODE_CLOSE_QUOTE)) {
				if(memcmp(str, _UNICODE_CLOSE_QUOTE, n) == 0) {
					_handle_error_at_pos(s, SE_PS_INVALID_CHAR, s->source_file, pos, row, col, MB_FUNC_ERR, _exit, result);
				}
			}
			if(context->symbol_state == _SS_IDENTIFIER) {
				_mb_check_exit(result = _append_uu_char_to_symbol(s, str, n), _exit);
			} else if(context->symbol_state == _SS_OPERATOR) {
				context->symbol_state = _SS_IDENTIFIER;
				_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
				_mb_check_exit(result = _append_uu_char_to_symbol(s, str, n), _exit);
			}

			break;
		}
#endif /* MB_ENABLE_UNICODE_ID */

		c = toupper(c);
		if(_is_blank_char(c)) { /* \t space */
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
		} else if(_is_newline_char(c)) { /* \r \n EOF */
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
			_mb_check_exit(result = _append_char_to_symbol(s, MB_EOS), _exit);
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
		} else if(_is_separator_char(c) || _is_bracket_char(c)) { /* , ; : ( ) */
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
			_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
		} else if(_is_quotation_char(c)) { /* " */
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
			_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
			context->parsing_state = _PS_STRING;
		} else if(_is_comment_char(c)) { /* ' */
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
			_mb_check_exit(result = _append_char_to_symbol(s, MB_EOS), _exit);
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
			context->parsing_state = _PS_COMMENT;
			context->multi_line_comment_count = 1;
		} else {
			if(context->symbol_state == _SS_IDENTIFIER) {
				if(_is_identifier_char(c)) {
					_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
				} else if(_is_operator_char(c)) {
					if(_is_exponent_prefix(context->current_symbol, 0, context->current_symbol_nonius - 2) && _is_exponential_char(last_char) && (c == '+' || c == '-')) {
						_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
					} else {
						context->symbol_state = _SS_OPERATOR;
						_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
						_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
					}
				} else {
					_handle_error_at_pos(s, SE_PS_INVALID_CHAR, s->source_file, pos, row, col, MB_FUNC_ERR, _exit, result);
				}
			} else if(context->symbol_state == _SS_OPERATOR) {
				if(_is_identifier_char(c)) {
					context->symbol_state = _SS_IDENTIFIER;
					_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
					_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
				} else if(_is_operator_char(c)) {
					if(c == '-')
						_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
					_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
				} else {
					_handle_error_at_pos(s, SE_PS_INVALID_CHAR, s->source_file, pos, row, col, MB_FUNC_ERR, _exit, result);
				}
			} else {
				mb_assert(0 && "Impossible.");
			}
		}

		break;
	case _PS_STRING:
		if(_is_quotation_char(c)) { /* " */
			_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
			_mb_check_exit(result = _cut_symbol(s, pos, row, col), _exit);
			context->parsing_state = _PS_NORMAL;
		} else {
			_mb_check_exit(result = _append_char_to_symbol(s, c), _exit);
		}

		break;
	case _PS_COMMENT:
		if(_is_eof_char(c)) { /* EOF */
			context->parsing_state = _PS_NORMAL;

			break;
		}
		if(context->multi_line_comment_count != 0 && c == _MULTI_LINE_COMMENT_PREFIX[context->multi_line_comment_count++]) {
			if(context->multi_line_comment_count >= countof(_MULTI_LINE_COMMENT_PREFIX) - 1) {
				context->parsing_state = _PS_MULTI_LINE_COMMENT;
				context->multi_line_comment_count = 0;

				break;
			}

			break;
		} else {
			context->multi_line_comment_count = 0;
		}
		if(_is_newline_char(c)) /* \r \n EOF */
			context->parsing_state = _PS_NORMAL;

		break;
	case _PS_MULTI_LINE_COMMENT:
		if(_is_eof_char(c)) { /* EOF */
			context->parsing_state = _PS_NORMAL;

			break;
		}
		if(_is_comment_char(c) && context->multi_line_comment_count == 0) {
			context->multi_line_comment_count = 1;
		} else if(context->multi_line_comment_count != 0 && c == _MULTI_LINE_COMMENT_POSTFIX[context->multi_line_comment_count++]) {
			if(context->multi_line_comment_count >= countof(_MULTI_LINE_COMMENT_POSTFIX) - 1) {
				context->parsing_state = _PS_NORMAL;
				context->multi_line_comment_count = 0;
			}
		} else {
			context->multi_line_comment_count = 0;
		}

		break;
	default:
		mb_assert(0 && "Unknown parsing state.");

		break;
	}

_exit:
	return result;
}

/* Set the position of an error */
void _set_error_pos(mb_interpreter_t* s, int pos, unsigned short row, unsigned short col) {
	mb_assert(s);

	s->last_error_pos = pos;
	s->last_error_row = row;
	s->last_error_col = col;
}

/* Do something before importing another file */
char* _prev_import(mb_interpreter_t* s, char* lf, int* pos, unsigned short* row, unsigned short* col) {
#ifdef MB_ENABLE_SOURCE_TRACE
	char* result = 0;
	_parsing_context_t* context = 0;
	_import_info_t* info = 0;
	_object_t* obj = 0;

	mb_assert(s);

	context = s->parsing_context;
	if(context) {
		if(pos) *pos = context->parsing_pos;
		if(row) *row = context->parsing_row;
		if(col) *col = context->parsing_col;
		context->parsing_pos = 0;
		context->parsing_row = 1;
		context->parsing_col = 0;
	}

	result = s->source_file;
	s->source_file = lf;

	obj = _create_object();
	obj->type = _DT_EOS;
	obj->is_ref = false;
	_ls_pushback(s->ast, obj);

	info = (_import_info_t*)mb_malloc(sizeof(_import_info_t));
	info->source_file = lf ? mb_strdup(lf, strlen(lf) + 1) : 0;
	obj = _create_object();
	obj->type = _DT_PREV_IMPORT;
	obj->is_ref = false;
	obj->data.import_info = info;
	_ls_pushback(s->ast, obj);

	return result;
#else /* MB_ENABLE_SOURCE_TRACE */
	mb_unrefvar(s);
	mb_unrefvar(lf);
	mb_unrefvar(pos);
	mb_unrefvar(row);
	mb_unrefvar(col);

	return 0;
#endif /* MB_ENABLE_SOURCE_TRACE */
}

/* Do something after importing another file */
char* _post_import(mb_interpreter_t* s, char* lf, int* pos, unsigned short* row, unsigned short* col) {
#ifdef MB_ENABLE_SOURCE_TRACE
	char* result = 0;
	_parsing_context_t* context = 0;
	_import_info_t* info = 0;
	_object_t* obj = 0;

	mb_assert(s);

	context = s->parsing_context;
	if(context) {
		if(pos) context->parsing_pos = *pos;
		if(row) context->parsing_row = *row;
		if(col) context->parsing_col = *col;
	}

	s->source_file = lf;
	result = s->source_file;

	info = (_import_info_t*)mb_malloc(sizeof(_import_info_t));
	info->source_file = lf ? mb_strdup(lf, strlen(lf) + 1) : 0;
	obj = _create_object();
	obj->type = _DT_POST_IMPORT;
	obj->is_ref = false;
	obj->data.import_info = info;
	_ls_pushback(s->ast, obj);

	obj = _create_object();
	obj->type = _DT_EOS;
	obj->is_ref = false;
	_ls_pushback(s->ast, obj);

	return result;
#else /* MB_ENABLE_SOURCE_TRACE */
	mb_unrefvar(s);
	mb_unrefvar(lf);
	mb_unrefvar(pos);
	mb_unrefvar(row);
	mb_unrefvar(col);

	return 0;
#endif /* MB_ENABLE_SOURCE_TRACE */
}

/** Object processors */

/* Get the size of a data type */
int_t _get_size_of(_data_e type) {
	int_t result = 0;

#ifdef MB_SIMPLE_ARRAY
	if(type == _DT_INT) {
		result = sizeof(int_t);
	} else if(type == _DT_REAL) {
		result = sizeof(real_t);
	} else if(type == _DT_STRING) {
		result = sizeof(char*);
	} else {
		mb_assert(0 && "Unsupported.");
	}
#else /* MB_SIMPLE_ARRAY */
	mb_unrefvar(type);

	result = sizeof(_raw_u);
#endif /* MB_SIMPLE_ARRAY */

	return result;
}

/* Try to get a value (typed as int_t, real_t or char*) */
bool_t _try_get_value(_object_t* obj, mb_value_u* val, _data_e expected) {
	bool_t result = false;

	mb_assert(obj && val);

	if(obj->type == _DT_INT && (expected == _DT_UNKNOWN || expected == _DT_INT)) {
		val->integer = obj->data.integer;
		result = true;
	} else if(obj->type == _DT_REAL && (expected == _DT_UNKNOWN || expected == _DT_REAL)) {
		val->float_point = obj->data.float_point;
		result = true;
	} else if(obj->type == _DT_VAR) {
		result = _try_get_value(obj->data.variable->data, val, expected);
	}

	return result;
}

/* Determine if an object is a nil */
bool_t _is_nil(void* obj) {
	bool_t result = false;
	_object_t* o = 0;

	mb_assert(obj);

	o = (_object_t*)obj;
	if(o->type == _DT_NIL)
		result = true;
	else if(o->type == _DT_VAR)
		result = o->data.variable->data->type == _DT_NIL;

	return result;
}

/* Determine if an object is a number */
bool_t _is_number(void* obj) {
	bool_t result = false;
	_object_t* o = 0;

	mb_assert(obj);

	o = (_object_t*)obj;
	if(o->type == _DT_INT || o->type == _DT_REAL)
		result = true;
	else if(o->type == _DT_VAR)
		result = o->data.variable->data->type == _DT_INT || o->data.variable->data->type == _DT_REAL;

	return result;
}

/* Determine if an object is a string value or a string variable */
bool_t _is_string(void* obj) {
	bool_t result = false;
	_object_t* o = 0;

	mb_assert(obj);

	o = (_object_t*)obj;
	if(o->type == _DT_STRING)
		result = true;
	else if(o->type == _DT_VAR)
		result = o->data.variable->data->type == _DT_STRING;

	return result;
}

/* Extract a string from an object */
char* _extract_string(_object_t* obj) {
	char* result = 0;

	mb_assert(obj);

	if(obj->type == _DT_STRING)
		result = obj->data.string;
	else if(obj->type == _DT_VAR && obj->data.variable->data->type == _DT_STRING)
		result = obj->data.variable->data->data.string;

	if(!result)
		result = MB_NULL_STRING;

	return result;
}

#ifdef MB_MANUAL_REAL_FORMATTING
/* Convert a real number to string */
void _real_to_str(real_t r, char* str, size_t size, size_t afterpoint) {
	size_t pos = 0;
	size_t len = 0;
	char curr[4];
	int val = (int)r;
	char dot = 0;

	itoa(val, str, 10);
	if(r < 0) {
		r *= -1;
		val *= -1;
	}
	pos = len = strlen(str);
	while(pos < size - 1) {
		r = r - (real_t)val;
		if(r == 0.0)
			break;
		if(!dot) {
			dot = 1;
			str[pos++] = '.';
		}
		r *= 10;
		val = (int)r;
		itoa(val, curr, 10);
		str[pos++] = *curr;
		if(--afterpoint == 0)
			break;
	}
	str[pos] = _ZERO_CHAR;
}
#endif /* MB_MANUAL_REAL_FORMATTING */

/* Convert a real number to string the standard way */
void _real_to_str_std(real_t r, char* str, size_t size) {
	size_t i = 0;

	if((size_t)sprintf(str, MB_REAL_FMT, r) >= size) {
		mb_assert(0 && "Buffer overflow.");
	}
	for(i = 0; i < size; ++i) {
		if(str[i] == ',') {
			str[i] = '.';

			break;
		} else if(str[i] == _ZERO_CHAR) {
			break;
		}
	}
}

#ifdef _HAS_REF_OBJ_LOCK
/* Lock a referenced object */
bool_t _lock_ref_object(_lock_t* lk, _ref_t* ref, void* obj) {
	mb_assert(lk);

	_ref(ref, obj);
	if(*lk >= 0)
		++(*lk);
	else
		--(*lk);

	return true;
}

/* Unlock a referenced object */
bool_t _unlock_ref_object(_lock_t* lk, _ref_t* ref, void* obj) {
	bool_t result = true;

	mb_assert(lk);

	if(*lk > 0)
		--(*lk);
	else if(*lk < 0)
		++(*lk);
	else
		result = false;
	_unref(ref, obj);

	return result;
}

/* Write operation on a referenced object */
bool_t _write_on_ref_object(_lock_t* lk, _ref_t* ref, void* obj) {
	bool_t result = true;
	mb_unrefvar(ref);
	mb_unrefvar(obj);

	mb_assert(lk);

	if(*lk > 0)
		*lk = -(*lk);
	else
		result = false;

	return result;
}
#endif /* _HAS_REF_OBJ_LOCK */

/* Tell whether an object is referenced */
bool_t _is_ref(_object_t* obj) {
	switch(obj->type) {
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		return true;
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_ARRAY_REF
	case _DT_ARRAY:
		return true;
#endif /* MB_ENABLE_ARRAY_REF */
#ifdef MB_ENABLE_COLLECTION_LIB
	case _DT_LIST:
		return true;
	case _DT_DICT:
		return true;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		return true;
#endif /* MB_ENABLE_CLASS */
	case _DT_ROUTINE:
#ifdef MB_ENABLE_LAMBDA
		return obj->data.routine->type == MB_RT_LAMBDA;
#else /* MB_ENABLE_LAMBDA */
		return false;
#endif /* MB_ENABLE_LAMBDA */
	default:
		return false;
	}
}

/* Increase the reference of a stub by 1 */
_ref_count_t _ref(_ref_t* ref, void* data) {
	_ref_count_t before = *ref->count;

	mb_assert((intptr_t)ref == (intptr_t)data);

	++(*ref->count);
	if(before > *ref->count) {
		mb_assert(0 && "Too many referencing, count overflow, please redefine _ref_count_t.");

		_handle_error_now(ref->s, SE_RN_OVERFLOW, ref->s->last_error_file, MB_FUNC_ERR);
	}

	return *ref->count;
}

/* Decrease the reference of a stub by 1 */
bool_t _unref(_ref_t* ref, void* data) {
	bool_t result = true;
	_gc_t* gc = 0;
	bool_t cld = false;

	mb_assert((intptr_t)ref == (intptr_t)data);

	cld = *ref->count == _NONE_REF + 1;
	do {
		gc = &ref->s->gc;
		result = --(*ref->count) == _NONE_REF;
		mb_assert(*ref->count >= _NONE_REF);
		_gc_add(ref, data, &ref->s->gc);
		if(ref->count && *ref->count == _NONE_REF)
			_collect_intermediate_value(ref, data);
		ref->on_unref(ref, data);
		if(result)
			_gc_remove(ref, data, gc);
	} while(0);
	if(cld) {
		_ht_set_or_insert(gc->collected_table, ref, data);
		_ht_set_or_insert(gc->collected_table, data, ref);
	}

	return result;
}

/* Increase the weak reference of a stub by 1 */
_ref_count_t _weak_ref(_ref_t* ref, void* data, _ref_t* weak) {
	_ref_count_t before = *ref->weak_count;
	mb_unrefvar(data);

	++(*ref->weak_count);
	if(before > *ref->weak_count) {
		mb_assert(0 && "Too many referencing, weak count overflow, please redefine _ref_count_t.");

		_handle_error_now(ref->s, SE_RN_OVERFLOW, ref->s->last_error_file, MB_FUNC_ERR);
	}
	memcpy(weak, ref, sizeof(_ref_t));

	return *ref->weak_count;
}

/* Decrease the weak reference of a stub by 1 */
bool_t _weak_unref(_ref_t* weak) {
	bool_t result = true;

	--(*weak->weak_count);
	mb_assert(*weak->weak_count >= _NONE_REF);
	if(weak->count && *weak->count == _NONE_REF)
		result = false;
	if(weak->count && *weak->count == _NONE_REF && weak->weak_count && *weak->weak_count == _NONE_REF) {
		safe_free(weak->weak_count);
		safe_free(weak->count);
	}

	return result;
}

/* Create a reference stub, initialize the reference count with zero */
void _create_ref(_ref_t* ref, _unref_func_t dtor, _data_e t, mb_interpreter_t* s) {
#ifdef MB_ENABLE_FORK
	mb_interpreter_t* src = 0;
#endif /* MB_ENABLE_FORK */

	if(ref->count)
		return;

#ifdef MB_ENABLE_FORK
	while(mb_get_forked_from(s, &src) == MB_FUNC_OK)
		s = src;
#endif /* MB_ENABLE_FORK */

	ref->count = (_ref_count_t*)mb_malloc(sizeof(_ref_count_t));
	*ref->count = _NONE_REF;
	ref->weak_count = (_ref_count_t*)mb_malloc(sizeof(_ref_count_t));
	*ref->weak_count = _NONE_REF;
	ref->on_unref = dtor;
	ref->type = t;
	ref->s = s;
}

/* Destroy a reference stub */
void _destroy_ref(_ref_t* ref) {
	if(!ref->count || !ref->weak_count)
		return;

	if(*ref->weak_count == _NONE_REF) {
		safe_free(ref->weak_count);
		safe_free(ref->count);
	}
	ref->on_unref = 0;
}

#ifdef MB_ENABLE_USERTYPE_REF
/* Create a referenced usertype */
_usertype_ref_t* _create_usertype_ref(mb_interpreter_t* s, void* val, mb_dtor_func_t un, mb_clone_func_t cl, mb_hash_func_t hs, mb_cmp_func_t cp, mb_fmt_func_t ft) {
	_usertype_ref_t* result = 0;

	mb_assert(s);

	result = (_usertype_ref_t*)mb_malloc(sizeof(_usertype_ref_t));
	memset(result, 0, sizeof(_usertype_ref_t));
	result->usertype = val;
	result->dtor = un;
	result->clone = cl;
	result->hash = hs;
	result->cmp = cp;
	result->fmt = ft;
	_create_ref(&result->ref, _unref_usertype_ref, _DT_USERTYPE_REF, s);

	return result;
}

/* Destroy a referenced usertype */
void _destroy_usertype_ref(_usertype_ref_t* c) {
	mb_assert(c);

	if(c->dtor)
		c->dtor(c->ref.s, c->usertype);
	if(c->calc_operators) {
		safe_free(c->calc_operators);
	}
	_destroy_ref(&c->ref);
	safe_free(c);
}

/* Unreference a referenced usertype */
void _unref_usertype_ref(_ref_t* ref, void* data) {
	mb_assert(ref);

	if(*ref->count == _NONE_REF)
		_destroy_usertype_ref((_usertype_ref_t*)data);
}

/* Clone a referenced usertype to a target object */
void _clone_usertype_ref(_usertype_ref_t* src, _object_t* tgt) {
	void* cpy = 0;

	mb_assert(src && tgt);

	_MAKE_NIL(tgt);

	if(!src->clone)
		return;
	cpy = src->clone(src->ref.s, src->usertype);
	if(!cpy)
		return;
	tgt->type = _DT_USERTYPE_REF;
	tgt->data.usertype_ref = _create_usertype_ref(
		src->ref.s, cpy,
		src->dtor, src->clone, src->hash, src->cmp, src->fmt
	);
#ifdef MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF
	tgt->data.usertype_ref->alive_checker = src->alive_checker;
#endif /* MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF */
	if(src->calc_operators) {
		tgt->data.usertype_ref->calc_operators = (_calculation_operator_info_t*)mb_malloc(sizeof(_calculation_operator_info_t));
		memcpy(tgt->data.usertype_ref->calc_operators, src->calc_operators, sizeof(_calculation_operator_info_t));
	}
	tgt->data.usertype_ref->coll_func = src->coll_func;
	tgt->data.usertype_ref->generic_func = src->generic_func;
	_ref(&tgt->data.usertype_ref->ref, tgt->data.usertype_ref);
}

/* Try to call a registered function on a referenced usertype */
bool_t _try_call_func_on_usertype_ref(mb_interpreter_t* s, _ls_node_t** ast, _object_t* obj, _ls_node_t* pathed, int* ret) {
	_object_t* tmp = (_object_t*)pathed->data;
	if(_IS_VAR(tmp) && tmp->data.variable->data->type == _DT_USERTYPE_REF) {
		bool_t mod = false;
		_ls_node_t* fn = 0;
		mb_func_t func = 0;
		char* r = strrchr(obj->data.variable->name, '.');
		if(!r) return false;
		++r;
		fn = _find_func(s, r, &mod);
		if(fn && fn->data) {
#ifdef MB_ENABLE_MODULE
			if(mod) {
				_module_func_t* mp = (_module_func_t*)fn->data;
				func = (mb_func_t)(intptr_t)mp->func;
			} else {
				func = (mb_func_t)(intptr_t)fn->data;
			}
#else /* MB_ENABLE_MODULE */
			func = (mb_func_t)(intptr_t)fn->data;
#endif /* MB_ENABLE_MODULE */
			s->usertype_ref_ahead = (_object_t*)tmp->data.variable->data;
#ifdef MB_ENABLE_STACK_TRACE
			_ls_pushback(s->stack_frames, r);
#endif /* MB_ENABLE_STACK_TRACE */
			if(ret)
				*ret = (func)(s, (void**)ast);
			else
				(func)(s, (void**)ast);
#ifdef MB_ENABLE_STACK_TRACE
			_ls_popback(s->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */

			return true;
		}
	}

	return false;
}
#endif /* MB_ENABLE_USERTYPE_REF */

/* Create an array */
_array_t* _create_array(mb_interpreter_t* s, const char* n, _data_e t) {
	_array_t* result = 0;

	mb_assert(s);

	result = (_array_t*)mb_malloc(sizeof(_array_t));
	memset(result, 0, sizeof(_array_t));
	result->type = t;
	result->name = (char*)n;
#ifdef MB_ENABLE_ARRAY_REF
	_create_ref(&result->ref, _unref_array, _DT_ARRAY, s);
	_ref(&result->ref, result);
#else /* MB_ENABLE_ARRAY_REF */
	mb_unrefvar(s);
#endif /* MB_ENABLE_ARRAY_REF */

	return result;
}

/* Destroy an array */
void _destroy_array(_array_t* arr) {
	mb_assert(arr);

	_clear_array(arr);
	if(arr->name) {
		safe_free(arr->name);
	}
#ifndef MB_SIMPLE_ARRAY
	if(arr->types) {
		safe_free(arr->types);
	}
#endif /* MB_SIMPLE_ARRAY */
#ifdef MB_ENABLE_ARRAY_REF
	_destroy_ref(&arr->ref);
#endif /* MB_ENABLE_ARRAY_REF */
	safe_free(arr);
}

/* Initialize an array */
void _init_array(_array_t* arr) {
	int elemsize = 0;

	mb_assert(arr);

#ifdef MB_SIMPLE_ARRAY
	elemsize = (int)_get_size_of(arr->type);
#else /* MB_SIMPLE_ARRAY */
	elemsize = (int)_get_size_of(_DT_UNKNOWN);
#endif /* MB_SIMPLE_ARRAY */
	mb_assert(arr->count > 0);
	mb_assert(!arr->raw);
	arr->raw = (void*)mb_malloc(elemsize * arr->count);
	if(arr->raw)
		memset(arr->raw, 0, elemsize * arr->count);
#ifndef MB_SIMPLE_ARRAY
	arr->types = (_data_e*)mb_malloc(sizeof(_data_e) * arr->count);
	if(arr->types) {
		unsigned ul = 0;
		for(ul = 0; ul < arr->count; ++ul)
			arr->types[ul] = _DT_INT;
	}
#endif /* MB_SIMPLE_ARRAY */
}

/* Clone an array */
_array_t* _clone_array(mb_interpreter_t* s, _array_t* arr) {
	_array_t* result = 0;
	unsigned index = 0;
	mb_value_u val;
	_data_e type = _DT_NIL;

	mb_assert(s && arr);

	result = _create_array(s, mb_strdup(arr->name, 0), arr->type);
	result->count = arr->count;
	result->dimension_count = arr->dimension_count;
	memcpy(result->dimensions, arr->dimensions, sizeof(result->dimensions));
	_init_array(result);
	for(index = 0; index < arr->count; index++) {
		_get_array_elem(s, arr, index, &val, &type);
		_set_array_elem(s, 0, result, index, &val, &type);
	}

	return result;
}

/* Calculate the true index of an array */
int _get_array_pos(mb_interpreter_t* s, _array_t* arr, int* d, int c) {
	int result = 0;
	int i = 0;
	unsigned n = 0;
	int f = 1;

	mb_assert(s && arr && d);

	if(c < 0 || c > arr->dimension_count) {
		result = -1;

		goto _exit;
	}
	for(i = 0; i < c; i++) {
		n = d[i];
		if(n >= arr->dimensions[i]) {
			result = -1;

			goto _exit;
		}
		result += n * f;
		f *= arr->dimensions[i];
	}

_exit:
	return result;
}

/* Calculate the true index of an array, used when walking through an AST */
int _get_array_index(mb_interpreter_t* s, _ls_node_t** l, _object_t* c, unsigned* index, bool_t* literally) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* arr = 0;
	_object_t* len = 0;
	_object_t subscript;
	_object_t* subscript_ptr = 0;
	mb_value_u val;
	int dcount = 0;
	int f = 1;
	unsigned idx = 0;

	mb_assert(s && l && index);

	subscript_ptr = &subscript;
	_MAKE_NIL(subscript_ptr);

	if(literally) *literally = false;

	/* Array name */
	ast = *l;
	if(!c && ast && _is_array(ast->data))
		c = (_object_t*)ast->data;
	if(!_is_array(c)) {
		_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	if(((_object_t*)c)->type == _DT_ARRAY)
		arr = (_object_t*)c;
	else
		arr = ((_object_t*)c)->data.variable->data;
	/* = */
	if(literally && ast->next && _IS_FUNC((_object_t*)ast->next->data, _core_equal)) {
		*literally = true;

		goto _exit;
	}
	/* ( */
	if(!ast->next || ((_object_t*)ast->next->data)->type != _DT_FUNC || ((_object_t*)ast->next->data)->data.func->pointer != _core_open_bracket) {
		_handle_error_on_obj(
			s, SE_RN_OPEN_BRACKET_EXPECTED, s->source_file,
			(ast && ast->next) ? ((_object_t*)ast->next->data) : 0,
			MB_FUNC_ERR, _exit, result
		);
	}
	ast = ast->next;
	/* Array subscript */
	if(!ast->next) {
		_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;
	while(((_object_t*)ast->data)->type != _DT_FUNC || ((_object_t*)ast->data)->data.func->pointer != _core_close_bracket) {
		/* Calculate an integer value */
		result = _calc_expression(s, &ast, &subscript_ptr);
		if(result != MB_FUNC_OK)
			goto _exit;
		len = subscript_ptr;
		if(!_try_get_value(len, &val, _DT_INT)) {
			_handle_error_on_obj(s, SE_RN_UNEXPECTED_TYPE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		if(val.integer < 0) {
			_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		if(dcount + 1 > arr->data.array->dimension_count) {
			_handle_error_on_obj(s, SE_RN_TOO_MANY_DIMENSIONS, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		if((unsigned)val.integer >= arr->data.array->dimensions[dcount]) {
			_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		idx += (unsigned)val.integer * f;
		/* Comma? */
		if(_IS_SEP(ast->data, ','))
			ast = ast->next;

		f *= arr->data.array->dimensions[dcount];
		++dcount;
	}
	*index = idx;
	if(!arr->data.array->raw) {
		_handle_error_on_obj(s, SE_RN_RANK_OUT_OF_BOUND, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

_exit:
	*l = ast;

	return result;
}

/* Get the value of an element in an array */
bool_t _get_array_elem(mb_interpreter_t* s, _array_t* arr, unsigned index, mb_value_u* val, _data_e* type) {
	bool_t result = true;
	int_t elemsize = 0;
	unsigned pos = 0;
	void* rawptr = 0;

	mb_assert(s && arr && val && type);
	mb_assert(index < arr->count);

	elemsize = _get_size_of(arr->type);
	pos = (unsigned)(elemsize * index);
	rawptr = (void*)((intptr_t)arr->raw + pos);
	if(arr->type == _DT_REAL) {
#ifdef MB_SIMPLE_ARRAY
		val->float_point = *((real_t*)rawptr);
		*type = _DT_REAL;
#else /* MB_SIMPLE_ARRAY */
		_copy_bytes(val->bytes, *((mb_val_bytes_t*)rawptr));
		*type = arr->types[index];
#endif /* MB_SIMPLE_ARRAY */
	} else if(arr->type == _DT_STRING) {
		val->string = *((char**)rawptr);
		*type = _DT_STRING;
	} else {
		mb_assert(0 && "Unsupported.");
	}

	return result;
}

/* Set the value of an element in an array */
int _set_array_elem(mb_interpreter_t* s, _ls_node_t* ast, _array_t* arr, unsigned index, mb_value_u* val, _data_e* type) {
	int result = MB_FUNC_OK;
	int_t elemsize = 0;
	unsigned pos = 0;
	void* rawptr = 0;
	mb_unrefvar(ast);

	mb_assert(s && arr && val);
	mb_assert(index < arr->count);

	elemsize = _get_size_of(arr->type);
	pos = (unsigned)(elemsize * index);
	rawptr = (void*)((intptr_t)arr->raw + pos);
#ifdef MB_SIMPLE_ARRAY
	switch(*type) {
	case _DT_INT:
		*((real_t*)rawptr) = (real_t)val->integer;

		break;
	case _DT_REAL:
		*((real_t*)rawptr) = val->float_point;

		break;
	case _DT_STRING: {
			size_t _sl = 0;
			if(arr->type != _DT_STRING) {
				_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
			_sl = strlen(val->string);
			if(*((char**)rawptr))
				mb_free(*((char**)rawptr));
			*((char**)rawptr) = (char*)mb_malloc(_sl + 1);
			memcpy(*((char**)rawptr), val->string, _sl + 1);
		}

		break;
	default:
		result = MB_FUNC_ERR;

		break;
	}
#else /* MB_SIMPLE_ARRAY */
	if(arr->types[index] == _DT_STRING && *((char**)rawptr))
		mb_free(*((char**)rawptr));
	switch(*type) {
	case _DT_STRING: {
			size_t _sl = 0;
			_sl = strlen(val->string);
			*((char**)rawptr) = (char*)mb_malloc(_sl + 1);
			memcpy(*((char**)rawptr), val->string, _sl + 1);
			arr->types[index] = _DT_STRING;
		}

		break;
	default:
		_copy_bytes(*((mb_val_bytes_t*)rawptr), val->bytes);
		arr->types[index] = *type;

		break;
	}
#endif /* MB_SIMPLE_ARRAY */

	goto _exit; /* Avoid an unreferenced label warning */

_exit:
	return result;
}

/* Clear an array */
void _clear_array(_array_t* arr) {
	char* str = 0;
	int_t elemsize = 0;
	unsigned pos = 0;
	void* rawptr = 0;
	unsigned ul = 0;

	mb_assert(arr);

	if(arr->raw) {
#ifndef MB_SIMPLE_ARRAY
		if(arr->type == _DT_REAL) {
			for(ul = 0; ul < arr->count; ++ul) {
				elemsize = _get_size_of(arr->type);
				pos = (unsigned)(elemsize * ul);
				rawptr = (void*)((intptr_t)arr->raw + pos);
				if(arr->types[ul] == _DT_STRING) {
					str = *((char**)rawptr);
					if(str) {
						safe_free(str);
					}
				} else {
					_object_t obj;
					obj.type = arr->types[ul];
					_copy_bytes(obj.data.bytes, rawptr);
					_dispose_object(&obj);
				}
			}
		}
#endif /* MB_SIMPLE_ARRAY */
		if(arr->type == _DT_STRING) {
			for(ul = 0; ul < arr->count; ++ul) {
				elemsize = _get_size_of(arr->type);
				pos = (unsigned)(elemsize * ul);
				rawptr = (void*)((intptr_t)arr->raw + pos);
				str = *((char**)rawptr);
				if(str) {
					safe_free(str);
				}
			}
		}
		safe_free(arr->raw);
	}
}

/* Determine if an object is an array value or an array variable */
bool_t _is_array(void* obj) {
	bool_t result = false;
	_object_t* o = 0;

	o = (_object_t*)obj;
	if(o && o->type == _DT_ARRAY)
		result = true;
	else if(o && o->type == _DT_VAR)
		result = o->data.variable->data->type == _DT_ARRAY;

	return result;
}

#ifdef MB_ENABLE_ARRAY_REF
/* Unreference an array */
void _unref_array(_ref_t* ref, void* data) {
	mb_assert(ref);

	if(*ref->count == _NONE_REF)
		_destroy_array((_array_t*)data);
}
#endif /* MB_ENABLE_ARRAY_REF */

#ifdef MB_ENABLE_CLASS
/* Initialize a class */
void _init_class(mb_interpreter_t* s, _class_t* instance, char* n) {
	_running_context_t* running = 0;
	_object_t* meobj = 0;
	_var_t* me = 0;

	mb_assert(s && instance && n);

	running = s->running_context;

	memset(instance, 0, sizeof(_class_t));
	_create_ref(&instance->ref, _unref_class, _DT_CLASS, s);
	_ref(&instance->ref, instance);
	instance->name = n;
	instance->meta_list = _ls_create();
	instance->scope = _create_running_context(true);
	instance->created_from = instance;

	me = _create_var(&meobj, _CLASS_ME, strlen(_CLASS_ME) + 1, true);
	me->data->type = _DT_CLASS;
	me->data->data.instance = instance;
	me->pathing = _PATHING_NORMAL;
	me->is_me = true;
	_ht_set_or_insert(instance->scope->var_dict, me->name, meobj);
}

/* Begin parsing a class */
void _begin_class(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;
	context->class_state = _CLASS_STATE_PROC;
}

/* End parsing a class */
bool_t _end_class(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;
	if(context->routine_state) {
		_handle_error_now(s, SE_RN_INVALID_ROUTINE, s->source_file, MB_FUNC_ERR);
	}
	if(context->class_state == _CLASS_STATE_NONE) {
		_handle_error_now(s, SE_RN_INVALID_CLASS, s->source_file, MB_FUNC_ERR);

		return false;
	}
	context->class_state = _CLASS_STATE_NONE;
	s->last_instance = 0;

	return true;
}

/* Unreference a class instance */
void _unref_class(_ref_t* ref, void* data) {
	mb_assert(ref);

	if(ref->s->valid)
		_out_of_scope(ref->s, ((_class_t*)data)->scope, (_class_t*)data, 0, false);

	if(*ref->count == _NONE_REF)
		_destroy_class((_class_t*)data);
}

/* Destroy a class instance */
void _destroy_class(_class_t* c) {
	mb_assert(c);

	if(c->meta_list) {
		_unlink_meta_class(c->ref.s, c);
		_ls_destroy(c->meta_list);
	}
	if(c->scope->var_dict) {
		_ht_foreach(c->scope->var_dict, _destroy_object);
		_ht_destroy(c->scope->var_dict);
	}
#ifdef MB_ENABLE_LAMBDA
	if(c->scope->refered_lambdas) {
		_ls_destroy(c->scope->refered_lambdas);
		c->scope->refered_lambdas = 0;
	}
#endif /* MB_ENABLE_LAMBDA */
	safe_free(c->scope);
	_destroy_ref(&c->ref);
	safe_free(c->name);
	safe_free(c);
}

/* Traverse all fields of a class instance, and its meta class instances recursively as well */
bool_t _traverse_class(_class_t* c, _class_scope_walker_t scope_walker, _class_meta_walker_t meta_walker, unsigned meta_depth, bool_t meta_walk_on_self, void* extra_data, void* ret) {
	bool_t result = true;
	_ls_node_t* node = 0;
	_class_t* meta = 0;

	mb_assert(c);

	if(scope_walker) {
		_HT_FOREACH(c->scope->var_dict, _do_nothing_on_object, scope_walker, extra_data);
	}
	if(meta_walk_on_self) {
		if(meta_walker) {
			result = meta_walker(c, extra_data, ret);
			if(!result)
				goto _exit;
		}
	}
	node = c->meta_list ? c->meta_list->next : 0;
	while(node) {
		meta = (_class_t*)node->data;
		if(meta_walker && meta_depth) {
			result = meta_walker(meta, extra_data, ret);
			if(!result)
				break;
		}
		result = _traverse_class(
			meta,
			scope_walker,
			meta_walker, meta_depth ? meta_depth - 1 : 0,
			meta_walk_on_self,
			extra_data, ret
		);
		if(!result)
			break;
		node = node->next;
	}

_exit:
	return result;
}

/* Link a class instance to the meta list of another class instance */
bool_t _link_meta_class(mb_interpreter_t* s, _class_t* derived, _class_t* base) {
	mb_assert(s && derived && base);

	if(_ls_find(derived->meta_list, base, (_ls_compare_t)_ht_cmp_intptr, 0)) {
		_handle_error_now(s, SE_RN_INVALID_CLASS, s->source_file, MB_FUNC_ERR);

		return false;
	}

	_ls_pushback(derived->meta_list, base);
	_ref(&base->ref, base);

	return true;
}

/* Unlink all meta class instances of a class instance */
void _unlink_meta_class(mb_interpreter_t* s, _class_t* derived) {
	mb_assert(s && derived);

	_LS_FOREACH(derived->meta_list, _do_nothing_on_object, _unlink_meta_instance, derived);
	_ls_clear(derived->meta_list);
}

/* Unlink a meta class instance */
int _unlink_meta_instance(void* data, void* extra, _class_t* derived) {
	_class_t* base = 0;
	mb_unrefvar(extra);

	mb_assert(data && derived);

	base = (_class_t*)data;
	_unref(&base->ref, base);

	return 0;
}

/* Clone fields of a class instance to another */
int _clone_clsss_field(void* data, void* extra, void* n) {
	int result = _OP_RESULT_NORMAL;
	_object_t* obj = 0;
	_array_t* arr = 0;
	_var_t* var = 0;
	_routine_t* sub = 0;
	_class_t* instance = (_class_t*)n;
	_object_t* ret = 0;
	mb_unrefvar(extra);

	mb_assert(data && n);

	obj = (_object_t*)data;
	if(_is_internal_object(obj))
		goto _exit;
	switch(obj->type) {
	case _DT_VAR:
		var = obj->data.variable;
		if(!_IS_ME(var)) {
			if(_ht_find(instance->scope->var_dict, var->name))
				break;

			ret = _duplicate_parameter(var, 0, instance->scope);
			_clone_object(instance->ref.s, obj, ret->data.variable->data, false, var->data->type != _DT_CLASS);
#ifdef MB_ENABLE_SOURCE_TRACE
			ret->source_pos = -1;
			ret->source_row = ret->source_col = 0xFFFF;
#else /* MB_ENABLE_SOURCE_TRACE */
			ret->source_pos = -1;
#endif /* MB_ENABLE_SOURCE_TRACE */
		}

		break;
	case _DT_ARRAY:
		arr = obj->data.array;
		if(!_ht_find(instance->scope->var_dict, arr->name)) {
			ret = _create_object();
			ret->type = _DT_ARRAY;
			ret->is_ref = false;
			_clone_object(instance->ref.s, obj, ret, false, false);

			_ht_set_or_insert(instance->scope->var_dict, ret->data.array->name, ret);
		}

		break;
	case _DT_ROUTINE:
		sub = obj->data.routine;
		if(!_ht_find(instance->scope->var_dict, sub->name)) {
			_routine_t* routine = _clone_routine(sub, instance, false);
			ret = _create_object();
			ret->type = _DT_ROUTINE;
			ret->data.routine = routine;
			ret->is_ref = false;

			_ht_set_or_insert(instance->scope->var_dict, obj->data.routine->name, ret);
		}

		break;
	default: /* Do nothing */
		break;
	}

_exit:
	return result;
}

/* Link meta class to a new instance */
bool_t _clone_class_meta_link(_class_t* meta, void* n, void* ret) {
	_class_t* instance = (_class_t*)n;
	mb_unrefvar(ret);

	mb_assert(meta && n);

	_link_meta_class(instance->ref.s, instance, meta);

	return true;
}

/* Search for a meta function with a specific name and assign to a member field */
int _search_class_meta_function(mb_interpreter_t* s, _class_t* instance, const char* n, _routine_t* _UNALIGNED_ARG * f) {
	_ls_node_t* node = 0;

	mb_assert(s);

	node = _search_identifier_in_class(s, instance, n, 0, 0);
	if(f) *f = 0;
	if(node) {
		_object_t* obj = (_object_t*)node->data;
		if(obj && _IS_ROUTINE(obj)) {
			if(f) *f = obj->data.routine;

			return 1;
		}
	}

	return 0;
}

/* Search for the HASH and COMPARE meta function for a class */
int _search_class_hash_and_compare_functions(mb_interpreter_t* s, _class_t* instance) {
	mb_assert(s && instance);

	_search_class_meta_function(s, instance, _CLASS_HASH_FUNC, &instance->hash);
	_search_class_meta_function(s, instance, _CLASS_COMPARE_FUNC, &instance->compare);

	if(!instance->hash && !instance->compare) {
		return MB_FUNC_OK;
	} else if(instance->hash && instance->compare) {
		return MB_FUNC_OK;
	} else {
		instance->hash = 0;
		instance->compare = 0;

		return MB_FUNC_WARNING;
	}
}

/* Detect whether a class instance is inherited from another */
bool_t _is_a_class(_class_t* instance, void* m, void* ret) {
	_class_t* meta = (_class_t*)m;
	bool_t* r = (bool_t*)ret;
	bool_t is_a = false;

	mb_assert(instance && meta && ret);

	do {
		if(instance == meta) {
			is_a = true;

			break;
		}
		if(instance == instance->created_from)
			break;
		instance = instance->created_from;
	} while(1);

	*r = is_a;

	return !(*r);
}

/* Add a meta class instance to a GC reachable table */
bool_t _add_class_meta_reachable(_class_t* meta, void* ht, void* ret) {
	_ht_node_t* htable = (_ht_node_t*)ht;
	mb_unrefvar(ret);

	mb_assert(meta && ht);

	if(!_ht_find(htable, &meta->ref))
		_ht_set_or_insert(htable, &meta->ref, meta);

	return true;
}

#ifdef MB_ENABLE_COLLECTION_LIB
/* Reflect each field of a class instance to a dictionary */
int _reflect_class_field(void* data, void* extra, void* d) {
	int result = _OP_RESULT_NORMAL;
	_object_t* obj = 0;
	_var_t* var = 0;
	_routine_t* sub = 0;
	_dict_t* coll = (_dict_t*)d;
	_object_t tmp;
	mb_unrefvar(extra);

	mb_assert(data && d);

	_MAKE_NIL(&tmp);
	tmp.type = _DT_STRING;
	obj = (_object_t*)data;
	if(_is_internal_object(obj))
		goto _exit;
	switch(obj->type) {
	case _DT_VAR:
		var = (_var_t*)obj->data.variable;
		tmp.data.string = var->name;
		if(!_ht_find(coll->dict, &tmp)) {
			mb_value_t key, val;
			mb_make_string(key, var->name);
			_internal_object_to_public_value(obj, &val);
			_set_dict(coll, &key, &val, 0, 0);
		}

		break;
	case _DT_ROUTINE:
		sub = (_routine_t*)obj->data.routine;
		tmp.data.string = sub->name;
		if(!_ht_find(coll->dict, &tmp)) {
			mb_value_t key, val;
			mb_make_string(key, sub->name);
			mb_make_type(val, _internal_type_to_public_type(obj->type));
			_set_dict(coll, &key, &val, 0, 0);
		}

		break;
	default: /* Do nothing */
		break;
	}

_exit:
	return result;
}
#endif /* MB_ENABLE_COLLECTION_LIB */

/* Call the TOSTRING function of a class instance to format it */
int _format_class_to_string(mb_interpreter_t* s, void** l, _class_t* instance, _object_t* out, bool_t* got_tostr) {
	int result = MB_FUNC_OK;
	_ls_node_t* tsn = 0;

	mb_assert(s && l && instance && out);

	tsn = _search_identifier_in_class(s, instance, _CLASS_TO_STRING_FUNC, 0, 0);
	if(got_tostr) *got_tostr = false;
	if(tsn) {
		_object_t* tso = (_object_t*)tsn->data;
		_ls_node_t* tmp = (_ls_node_t*)*l;
		mb_value_t va[1];
		mb_make_nil(va[0]);
		if(_eval_routine(s, &tmp, va, 1, tso->data.routine, _has_routine_fun_arg, _pop_routine_fun_arg) == MB_FUNC_OK) {
			_MAKE_NIL(out);
			_public_value_to_internal_object(&s->running_context->intermediate_value, out);
			if(out->type == _DT_STRING) {
				out->data.string = mb_strdup(out->data.string, strlen(out->data.string) + 1);
				out->is_ref = false;
				mb_make_nil(s->running_context->intermediate_value);
			} else {
				_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}
			if(got_tostr)
				*got_tostr = true;
		}
	}

_exit:
	return result;
}

/* Reflect a class instance from a string */
_class_t* _reflect_string_to_class(mb_interpreter_t* s, const char* n, mb_value_t* arg) {
	_ls_node_t* cs = 0;
	_object_t* c = 0;

	cs = _search_identifier_in_scope_chain(s, 0, n, _PATHING_NONE, 0, 0);
	if(!cs)
		return 0;
	c = (_object_t*)cs->data;
	if(!c)
		return 0;
	c = _GET_CLASS(c);
	if(!c)
		return 0;
	if(arg)
		_internal_object_to_public_value(c, arg);

	return c->data.instance;
}

/* Detect whether it's accessing a member of a class instance following a sub routine */
bool_t _is_valid_class_accessor_following_routine(mb_interpreter_t* s, _var_t* var, _ls_node_t* ast, _ls_node_t** out) {
	bool_t result = false;
	_running_context_t* running = 0;

	mb_assert(s && var && ast);

	running = s->running_context;

	if(out) *out = 0;

	if(_is_accessor_char(*var->name) && (ast && ast->prev && ast->prev != s->ast && _IS_FUNC(ast->prev->data, _core_close_bracket)) && running->intermediate_value.type == MB_DT_CLASS) {
		_class_t* instance = (_class_t*)running->intermediate_value.value.instance;
		_ls_node_t* fn = _search_identifier_in_class(s, instance, var->name + 1, 0, 0);
		result = true;
		if(fn && out)
			*out = fn;
	}

	return result;
}
#endif /* MB_ENABLE_CLASS */

/* Initialize a routine */
void _init_routine(mb_interpreter_t* s, _routine_t* routine, char* n, mb_routine_func_t f) {
	_running_context_t* running = 0;

	mb_assert(s && routine);

	running = s->running_context;

	memset(routine, 0, sizeof(_routine_t));
	routine->name = n;

	if(n && f)
		routine->type = MB_RT_NATIVE;
	else if(n && !f)
		routine->type = MB_RT_SCRIPT;
#ifdef MB_ENABLE_LAMBDA
	else if(!n && !f)
		routine->type = MB_RT_LAMBDA;
#endif /* MB_ENABLE_LAMBDA */

	switch(routine->type) {
	case MB_RT_SCRIPT:
		routine->func.basic.scope = _create_running_context(true);

		break;
#ifdef MB_ENABLE_LAMBDA
	case MB_RT_LAMBDA:
		_create_ref(&routine->func.lambda.ref, _unref_routine, _DT_ROUTINE, s);
		_ref(&routine->func.lambda.ref, routine);

		break;
#endif /* MB_ENABLE_LAMBDA */
	case MB_RT_NATIVE:
		routine->func.native.entry = f;

		break;
	default: /* Do nothing */
		break;
	}

#ifdef MB_ENABLE_SOURCE_TRACE
	if(s->source_file)
		routine->source_file = mb_strdup(s->source_file, 0);
#endif /* MB_ENABLE_SOURCE_TRACE */
}

/* Begin parsing a routine */
int _begin_routine(mb_interpreter_t* s) {
	int result = MB_FUNC_OK;
	_parsing_context_t* context = 0;
	unsigned short before = 0;

	mb_assert(s);

	context = s->parsing_context;
	before = context->routine_state++;
	if(before > context->routine_state) {
		context->routine_state--;
		result = MB_FUNC_ERR;
		_handle_error_now(s, SE_RN_INVALID_ROUTINE, s->last_error_file, result);
	}

	return result;
}

/* End parsing a routine */
bool_t _end_routine(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;
	if(!context->routine_state) {
		_handle_error_now(s, SE_RN_INVALID_ROUTINE, s->source_file, MB_FUNC_ERR);

		return false;
	}
	context->routine_state--;

	return true;
}

/* Begin parsing the definition of a routine */
void _begin_routine_definition(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;
	context->routine_params_state = _ROUTINE_STATE_DEF;
}

/* Begin parsing the parameter list of a routine */
void _begin_routine_parameter_list(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;
	context->routine_params_state = _ROUTINE_STATE_PARAMS;
}

/* End parsing the parameter list of a routine */
void _end_routine_parameter_list(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;
	context->routine_params_state = _ROUTINE_STATE_NONE;
}

/* Duplicate a parameter from a parameter list to variable dictionary */
_object_t* _duplicate_parameter(void* data, void* extra, _running_context_t* running) {
	_var_t* ref = 0;
	_var_t* var = 0;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(running);

	if(data == 0)
		return 0;

	ref = (_var_t*)data;

	var = _create_var(&obj, ref->name, 0, true);

	_ht_set_or_insert(running->var_dict, var->name, obj);

	return obj;
}

/* Clone a routine */
_routine_t* _clone_routine(_routine_t* sub, void* c, bool_t toupval) {
	_routine_t* result = 0;
#ifdef MB_ENABLE_CLASS
	_class_t* instance = (_class_t*)c;
#else /* MB_ENABLE_CLASS */
	mb_unrefvar(c);
#endif /* MB_ENABLE_CLASS */

	mb_assert(sub);

#ifdef MB_ENABLE_LAMBDA
	if(toupval || sub->type == MB_RT_LAMBDA)
		result = sub;
#else /* MB_ENABLE_LAMBDA */
	mb_unrefvar(toupval);
#endif /* MB_ENABLE_LAMBDA */

	if(!result) {
		result = (_routine_t*)mb_malloc(sizeof(_routine_t));
		memset(result, 0, sizeof(_routine_t));
		result->name = sub->name;
#ifdef MB_ENABLE_SOURCE_TRACE
		result->source_file = sub->source_file;
#endif /* MB_ENABLE_SOURCE_TRACE */
#ifdef MB_ENABLE_CLASS
		result->instance = instance;
#endif /* MB_ENABLE_CLASS */
		result->is_cloned = true;
		result->type = sub->type;
		result->func = sub->func;
	}

	return result;
}

#ifdef MB_ENABLE_LAMBDA
/* Initialize a lambda */
_running_context_t* _init_lambda(mb_interpreter_t* s, _routine_t* routine) {
	_running_context_t* result = 0;
	_lambda_t* lambda = 0;

	mb_assert(s && routine);

	_init_routine(s, routine, 0, 0);
	mb_assert(routine->type == MB_RT_LAMBDA);
	lambda = &routine->func.lambda;
	lambda->scope = _create_running_context(true);
	result = _push_scope_by_routine(s, lambda->scope);

	return result;
}

/* Unreference a lambda routine */
void _unref_routine(_ref_t* ref, void* data) {
	mb_assert(ref);

	if(*ref->count == _NONE_REF)
		_destroy_routine(ref->s, (_routine_t*)data);
}

/* Destroy a lambda routine */
void _destroy_routine(mb_interpreter_t* s, _routine_t* r) {
	_gc_t* gc = 0;

	mb_assert(r);

	if(s) gc = &s->gc;
	if(!r->is_cloned) {
		if(r->name) {
			safe_free(r->name);
		}
#ifdef MB_ENABLE_SOURCE_TRACE
		if(r->source_file) {
			safe_free(r->source_file);
		}
#endif /* MB_ENABLE_SOURCE_TRACE */
		switch(r->type) {
		case MB_RT_SCRIPT:
			if(r->func.basic.scope) {
				_destroy_scope(s, r->func.basic.scope);
				r->func.basic.scope = 0;
			}
			if(r->func.basic.parameters)
				_ls_destroy(r->func.basic.parameters);

			break;
		case MB_RT_LAMBDA:
			_destroy_ref(&r->func.lambda.ref);
			if(r->func.lambda.scope->var_dict) {
				_ht_foreach(r->func.lambda.scope->var_dict, _destroy_object);
				_ht_destroy(r->func.lambda.scope->var_dict);
			}
			safe_free(r->func.lambda.scope);
			if(r->func.lambda.parameters)
				_ls_destroy(r->func.lambda.parameters);
			if(r->func.lambda.outer_scope && !_ht_find(gc->collected_table, &r->func.lambda.outer_scope->ref))
				_unref(&r->func.lambda.outer_scope->ref, r->func.lambda.outer_scope);
			if(r->func.lambda.upvalues)
				_ht_destroy(r->func.lambda.upvalues);

			break;
		case MB_RT_NATIVE: /* Do nothing */
			break;
		default: /* Do nothing */
			break;
		}
	}
	safe_free(r);
}

/* Mark an upvalue of a lambda */
void _mark_upvalue(mb_interpreter_t* s, _lambda_t* lambda, _object_t* obj, const char* n) {
	_running_context_t* running = 0;
	_running_context_t* found_in_scope = 0;
	_ls_node_t* scp = 0;

	mb_assert(s && lambda && obj);

	running = s->running_context;
	scp = _search_identifier_in_scope_chain(s, running, n, _PATHING_NORMAL, 0, &found_in_scope);
	if(scp && found_in_scope) {
		_object_t* rot = (_object_t*)scp->data;
		rot = _GET_ROUTINE(rot);
		if(rot && lambda->scope && lambda->scope->prev != found_in_scope)
			return;
		if(!found_in_scope->refered_lambdas)
			found_in_scope->refered_lambdas = _ls_create();
		if(!_ls_find(found_in_scope->refered_lambdas, lambda, (_ls_compare_t)_ht_cmp_intptr, 0))
			_ls_pushback(found_in_scope->refered_lambdas, lambda);
	}

	if(!lambda->upvalues)
		lambda->upvalues = _ht_create(0, _ht_cmp_string, _ht_hash_string, 0);
	_ht_set_or_insert(lambda->upvalues, obj->data.variable->name, obj);
}

/* Try to mark upvalues of a lambda */
void _try_mark_upvalue(mb_interpreter_t* s, _routine_t* r, _object_t* obj) {
	_lambda_t* lambda = 0;
	_ls_node_t* node = 0;

	mb_assert(s && r && obj);
	mb_assert(r->type == MB_RT_LAMBDA);

	lambda = &r->func.lambda;

	switch(obj->type) {
	case _DT_VAR:
		node = _ht_find(lambda->scope->var_dict, obj->data.variable->name);
		if(!node || !node->data) {
			/* Mark upvalues referencing outer scope chain */
			_mark_upvalue(s, lambda, obj, obj->data.variable->name);
		}

		break;
	default: /* Do nothing */
		break;
	}
}

/* Create an outer scope, which is a referenced type */
_running_context_ref_t* _create_outer_scope(mb_interpreter_t* s) {
	_running_context_ref_t* result = 0;

	mb_assert(s);

	result = (_running_context_ref_t*)mb_malloc(sizeof(_running_context_ref_t));
	memset(result, 0, sizeof(_running_context_ref_t));
	_create_ref(&result->ref, _unref_outer_scope, _DT_OUTER_SCOPE, s);
	result->scope = _create_running_context(true);

	return result;
}

/* Unreference an outer scope */
void _unref_outer_scope(_ref_t* ref, void* data) {
	mb_assert(ref);

	if(*ref->count == _NONE_REF)
		_destroy_outer_scope((_running_context_ref_t*)data);
}

/* Destroy an outer scope */
void _destroy_outer_scope(_running_context_ref_t* p) {
	mb_assert(p);

	if(p) {
		_running_context_ref_t* scope = p;
		p = p->prev;
		_destroy_scope(scope->ref.s, scope->scope);
		_destroy_ref(&scope->ref);
		safe_free(scope);
	}
	while(p) {
		_running_context_ref_t* scope = p;
		p = p->prev;
		_unref(&scope->ref, scope);
	}
}

/* Do nothing, this is a helper function for lambda */
int _do_nothing_on_ht_for_lambda(void* data, void* extra) {
	int result = _OP_RESULT_NORMAL;
	mb_unrefvar(data);
	mb_unrefvar(extra);

	return result;
}

/* Fill an outer scope with the original value */
int _fill_with_upvalue(void* data, void* extra, _upvalue_scope_tuple_t* tuple) {
	_object_t* obj = (_object_t*)data;
	const char* n = (const char*)extra;
	unsigned ul = 0;
	_ls_node_t* ast = 0;
	_ls_node_t* nput = 0;

	nput = _ht_find(tuple->outer_scope->scope->var_dict, (void*)n);
	if(!nput) {
		_ls_node_t* nori = 0;
#ifdef MB_ENABLE_CLASS
		if(tuple->instance)
			nori = _search_identifier_in_scope_chain(tuple->s, tuple->scope, n, _PATHING_NORMAL, 0, 0);
		else
			nori = _ht_find(tuple->scope->var_dict, (void*)n);
#else /* MB_ENABLE_CLASS */
		nori = _ht_find(tuple->scope->var_dict, (void*)n);
#endif /* MB_ENABLE_CLASS */
		if(nori) {
			_object_t* ovar = 0;
			_var_t* var = _create_var(&ovar, n, 0, true);
			obj = (_object_t*)nori->data;
			_clone_object(tuple->s, obj, var->data, true, true);
			_REF(var->data)
			if(_IS_ROUTINE(obj) && obj->data.routine->type != MB_RT_LAMBDA) {
				ovar->is_ref = true;
				var->data->is_ref = true;
			}
#ifdef MB_ENABLE_CLASS
			if(_IS_VAR(obj))
				var->pathing = obj->data.variable->pathing;
			else
				var->pathing = _PATHING_NONE;
#endif /* MB_ENABLE_CLASS */
			ul = _ht_set_or_insert(tuple->outer_scope->scope->var_dict, ovar->data.variable->name, ovar);
			mb_assert(ul);
			_ht_set_or_insert(tuple->filled, extra, data);

			ast = tuple->lambda->entry;
			while(ast && ast != tuple->lambda->end->next) {
				_object_t* aobj = (_object_t*)ast->data;
				if(aobj) {
					switch(aobj->type) {
					case _DT_VAR:
						if(!strcmp(aobj->data.variable->name, ovar->data.variable->name)) {
#ifdef MB_ENABLE_CLASS
							aobj->data.variable->pathing = _PATHING_UPVALUE;
#endif /* MB_ENABLE_CLASS */
						}

						break;
					default: /* Do nothing */
						break;
					}
				}
				ast = ast->next;
			}
		}
	}

	return 0;
}

/* Remove filled upvalues */
int _remove_filled_upvalue(void* data, void* extra, _ht_node_t* ht) {
	_ht_remove_existing(data, extra, ht);

	return _OP_RESULT_NORMAL;
}

/* Fill an outer scope with the original one */
int _fill_outer_scope(void* data, void* extra, _upvalue_scope_tuple_t* tuple) {
	_lambda_t* lambda = (_lambda_t*)data;
	mb_unrefvar(extra);

	if(lambda->upvalues) {
		tuple->filled = _ht_create(0, _ht_cmp_intptr, _ht_hash_intptr, 0); {
			tuple->lambda = lambda;
			_HT_FOREACH(lambda->upvalues, _do_nothing_on_ht_for_lambda, _fill_with_upvalue, tuple);
			tuple->lambda = 0;
		}
		_HT_FOREACH(tuple->filled, _do_nothing_on_ht_for_lambda, _remove_filled_upvalue, lambda->upvalues);
		if(!_ht_count(lambda->upvalues)) {
			_ht_destroy(lambda->upvalues);
			lambda->upvalues = 0;
		}
		_ht_destroy(tuple->filled);
	}

	if(lambda->outer_scope) {
		_running_context_ref_t* root_ref = _get_root_ref_scope(lambda->outer_scope);
		root_ref->prev = tuple->outer_scope;
		root_ref->scope->prev = tuple->outer_scope->scope;
	} else {
		lambda->outer_scope = tuple->outer_scope;
	}

	_ref(&tuple->outer_scope->ref, tuple->outer_scope);

	return 0;
}

/* Remove a lambda from outer scope, which collected the lambda itself as an upvalue */
int _remove_this_lambda_from_upvalue(void* data, void* extra, _routine_t* routine) {
	int result = _OP_RESULT_NORMAL;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(routine->type == MB_RT_LAMBDA);

	obj = (_object_t*)data;
	if(_IS_VAR(obj))
		obj = obj->data.variable->data;
	if(_IS_ROUTINE(obj)) {
		if(obj->data.routine == routine) {
			mb_assert(obj->data.routine->type == MB_RT_LAMBDA);
			_MAKE_NIL(obj);
		}
	}

	return result;
}

/* Link the local scope of a lambda and all upvalue scopes in chain to a given scope */
_running_context_t* _link_lambda_scope_chain(mb_interpreter_t* s, _lambda_t* lambda, bool_t weak) {
	_running_context_ref_t* root_ref = 0;
	_running_context_t* root = 0;

	if(lambda->outer_scope) {
		lambda->scope->prev = lambda->outer_scope->scope;
		if(_find_scope(s, lambda->scope->prev)) {
			lambda->overlapped = s->running_context;
			if(!weak)
				s->running_context = lambda->scope;

			return lambda->scope;
		}
		root_ref = _get_root_ref_scope(lambda->outer_scope);
		root_ref->scope->prev = 0;
	}
	root = _get_root_scope(lambda->scope);

	if(weak) {
		_running_context_t* ret = _push_weak_scope_by_routine(s, root, 0);
		if(ret != root) {
			_destroy_scope(s, ret);

			return 0;
		}
	} else {
		root->prev = s->running_context;
		s->running_context = lambda->scope;
	}

	return lambda->scope;
}

/* Unlink the local scope of a lambda and all upvalue scopes in chain from a given scope */
_running_context_t* _unlink_lambda_scope_chain(mb_interpreter_t* s, _lambda_t* lambda, bool_t weak) {
	_running_context_ref_t* root_ref = 0;
	_running_context_t* root = 0;

	if(lambda->outer_scope) {
		if(lambda->overlapped) {
			if(!weak)
				s->running_context = lambda->overlapped;
			lambda->overlapped = 0;
			lambda->scope->prev = 0;

			return lambda->scope;
		}
		root_ref = _get_root_ref_scope(lambda->outer_scope);
		root = root_ref->scope;
	} else {
		root = lambda->scope;
	}

	if(weak)
		_pop_weak_scope(s, root);
	else
		s->running_context = root->prev;

	root->prev = 0;
	lambda->scope->prev = 0;

	return lambda->scope;
}

/* Check whether an object is a valid lambda body node */
bool_t _is_valid_lambda_body_node(mb_interpreter_t* s, _lambda_t* lambda, _object_t* obj) {
	mb_unrefvar(s);
	mb_unrefvar(lambda);

	return (
		!_IS_FUNC(obj, _core_def) &&
		!_IS_FUNC(obj, _core_enddef) &&
#ifdef MB_ENABLE_CLASS
		!_IS_FUNC(obj, _core_class) &&
		!_IS_FUNC(obj, _core_endclass) &&
#endif /* MB_ENABLE_CLASS */
		true
	);
}
#endif /* MB_ENABLE_LAMBDA */

#ifdef MB_ENABLE_CLASS
/* Create a scope reference to an existing one by a class */
_running_context_t* _reference_scope_by_class(mb_interpreter_t* s, _running_context_t* p, _class_t* c) {
	_running_context_t* result = 0;
	mb_unrefvar(c);

	mb_assert(s && p);

	if(p->meta == _SCOPE_META_REF)
		p = p->ref;

	result = _create_running_context(false);
	result->meta = _SCOPE_META_REF;
	result->ref = p;

	return result;
}

/* Push a scope by a class */
_running_context_t* _push_scope_by_class(mb_interpreter_t* s, _running_context_t* p) {
	mb_assert(s);

	if(_find_scope(s, p))
		p = _reference_scope_by_class(s, p, 0);
	p->prev = s->running_context;
	s->running_context = p;

	return s->running_context;
}

/* Try to search an identifire from a class */
_ls_node_t* _search_identifier_in_class(mb_interpreter_t* s, _class_t* instance, const char* n, _ht_node_t** ht, _running_context_t** sp) {
	_ls_node_t* result = 0;
	_ls_node_t* node = 0;
	_class_t* meta = 0;

	mb_assert(s && instance && n);

	result = _ht_find(instance->scope->var_dict, (void*)n);
	if(result) {
		if(ht) *ht = instance->scope->var_dict;
		if(sp) *sp = instance->scope;
	}

	if(!result) {
		node = instance->meta_list ? instance->meta_list->next : 0;
		while(node) {
			meta = (_class_t*)node->data;
			result = _search_identifier_in_class(s, meta, n, ht, sp);
			if(result)
				break;
			node = node->next;
		}
	}

	return result;
}
#endif /* MB_ENABLE_CLASS */

/* Create a scope reference to an existing one by a routine */
_running_context_t* _reference_scope_by_routine(mb_interpreter_t* s, _running_context_t* p, _routine_t* r) {
	_running_context_t* result = 0;

	mb_assert(s && p);

	if(p->meta == _SCOPE_META_REF)
		p = p->ref;

	result = _create_running_context(false);
	result->meta = _SCOPE_META_REF;
	result->ref = p;
	if(r && r->func.basic.parameters) {
		result->var_dict = _ht_create(0, _ht_cmp_string, _ht_hash_string, 0);
		_LS_FOREACH(r->func.basic.parameters, _do_nothing_on_object, _duplicate_parameter, result);
	}

	return result;
}

/* Push a weak scope by a routine */
_running_context_t* _push_weak_scope_by_routine(mb_interpreter_t* s, _running_context_t* p, _routine_t* r) {
	mb_assert(s);

	if(_find_scope(s, p))
		p = _reference_scope_by_routine(s, p, r);
	if(p)
		p->prev = s->running_context;

	return p;
}

/* Push a scope by a routine */
_running_context_t* _push_scope_by_routine(mb_interpreter_t* s, _running_context_t* p) {
	mb_assert(s);

	if(_find_scope(s, p))
		p = _reference_scope_by_routine(s, p, 0);
	if(p) {
		p->prev = s->running_context;
		s->running_context = p;
	}

	return s->running_context;
}

/* Destroy a scope */
void _destroy_scope(mb_interpreter_t* s, _running_context_t* p) {
	mb_unrefvar(s);

	if(p->var_dict) {
		_ht_foreach(p->var_dict, _destroy_object);
		_ht_destroy(p->var_dict);
#ifdef MB_ENABLE_LAMBDA
		if(p->refered_lambdas) {
			_ls_destroy(p->refered_lambdas);
			p->refered_lambdas = 0;
		}
#endif /* MB_ENABLE_LAMBDA */
	}
	safe_free(p);
}

/* Pop a weak scope */
_running_context_t* _pop_weak_scope(mb_interpreter_t* s, _running_context_t* p) {
	mb_assert(s);

	if(p)
		p->prev = 0;

	return p;
}

/* Pop a scope */
_running_context_t* _pop_scope(mb_interpreter_t* s, bool_t tidy) {
	_running_context_t* running = 0;

	mb_assert(s);

	running = s->running_context;
	s->running_context = running->prev;
	running->prev = 0;
	if(running->meta == _SCOPE_META_REF)
		_destroy_scope(s, running);
	else if(tidy)
		_out_of_scope(s, running, 0, 0, true);

	return s->running_context;
}

/* Out of current scope */
void _out_of_scope(mb_interpreter_t* s, _running_context_t* running, void* instance, _routine_t* routine, bool_t lose) {
#ifdef MB_ENABLE_LAMBDA
	_upvalue_scope_tuple_t tuple;
#endif /* MB_ENABLE_LAMBDA */
	mb_unrefvar(routine);

	mb_assert(s);

#ifdef MB_ENABLE_LAMBDA
	if(running->refered_lambdas) {
		tuple.s = s;
#ifdef MB_ENABLE_CLASS
		tuple.instance = (_class_t*)instance;
#else /* MB_ENABLE_CLASS */
		mb_unrefvar(instance);
#endif /* MB_ENABLE_CLASS */
		tuple.scope = running;
		tuple.outer_scope = _create_outer_scope(s);
		tuple.lambda = 0;

		_LS_FOREACH(running->refered_lambdas, _do_nothing_on_ht_for_lambda, _fill_outer_scope, &tuple);

		_ls_destroy(running->refered_lambdas);
		running->refered_lambdas = 0;
	}
#else /* MB_ENABLE_LAMBDA */
	mb_unrefvar(instance);
#endif /* MB_ENABLE_LAMBDA */

	if(lose) {
		if(running->var_dict)
			_HT_FOREACH(running->var_dict, _do_nothing_on_object, _lose_object, running);
	}
}

/* Find a scope in a scope chain */
_running_context_t* _find_scope(mb_interpreter_t* s, _running_context_t* p) {
	_running_context_t* running = 0;

	mb_assert(s);

	running = s->running_context;
	while(running) {
		if(running == p)
			return running;

		if(running->ref == p)
			return running->ref;

		running = running->prev;
	}

	return running;
}

/* Get the root scope in a scope chain */
_running_context_t* _get_root_scope(_running_context_t* scope) {
	_running_context_t* result = 0;

	while(scope) {
		result = scope;
		scope = scope->prev;
	}

	return result;
}

#ifdef MB_ENABLE_LAMBDA
/* Get the root referenced scope in a referenced scope chain */
_running_context_ref_t* _get_root_ref_scope(_running_context_ref_t* scope) {
	_running_context_ref_t* result = 0;

	while(scope) {
		result = scope;
		scope = scope->prev;
	}

	return result;
}
#endif /* MB_ENABLE_LAMBDA */

/* Get a proper scope to define a routine */
_running_context_t* _get_scope_to_add_routine(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;
	_running_context_t* running = 0;
	unsigned short class_state = _CLASS_STATE_NONE;

	mb_assert(s);

	context = s->parsing_context;
	running = s->running_context;
#ifdef MB_ENABLE_CLASS
	class_state = context->class_state;
#endif /* MB_ENABLE_CLASS */
	if(class_state != _CLASS_STATE_NONE) {
		if(running)
			running = running->prev;
	} else {
		while(running) {
			if(running->meta == _SCOPE_META_ROOT)
				break;

			running = running->prev;
		}
	}

	return running;
}

/* Try to search an identifier in a scope chain */
_ls_node_t* _search_identifier_in_scope_chain(mb_interpreter_t* s, _running_context_t* scope, const char* n, int fp, _ht_node_t** ht, _running_context_t** sp) {
	_ls_node_t* result = 0;
	_running_context_t* running = 0;
	_ht_node_t* fn = 0;
	_running_context_t* fs = 0;

	mb_assert(s && n);

#ifdef MB_ENABLE_CLASS
	if(fp) {
		result = _search_identifier_accessor(s, scope, n, &fn, &fs, fp == _PATHING_UNKNOWN_FOR_NOT_FOUND);
		if(result)
			goto _exit;
	}

	if(s->last_routine && s->last_routine->instance) {
		_class_t* lastinst = s->last_routine->instance;
		s->last_routine->instance = 0;
		result = _search_identifier_in_class(s, lastinst, n, &fn, &fs);
		s->last_routine->instance = lastinst;
		if(result)
			goto _exit;
	}
#else /* MB_ENABLE_CLASS */
	mb_unrefvar(fp);
#endif /* MB_ENABLE_CLASS */

	if(scope)
		running = scope;
	else
		running = s->running_context;
	while(running && !result) {
		if(running->var_dict) {
			result = _ht_find(running->var_dict, (void*)n);
			fn = running->var_dict;
			fs = running;
			if(!result && running->meta == _SCOPE_META_REF) {
				result = _ht_find(running->ref->var_dict, (void*)n);
				fn = running->ref->var_dict;
				fs = running->ref;
			}
			if(result)
				break;
		}

		running = running->prev;
	}

#ifdef MB_ENABLE_CLASS
_exit:
#endif /* MB_ENABLE_CLASS */
	if(ht) *ht = fn;
	if(sp) *sp = fs;

	return result;
}

/* Try to search an array in a scope chain */
_array_t* _search_array_in_scope_chain(mb_interpreter_t* s, _array_t* i, _object_t** o) {
	_object_t* obj = 0;
	_ls_node_t* scp = 0;
	_array_t* result = 0;

	mb_assert(s && i);

	result = i;
	if(result->name)
		scp = _search_identifier_in_scope_chain(s, 0, result->name, _PATHING_NONE, 0, 0);
	if(scp) {
		obj = (_object_t*)scp->data;
		if(obj && obj->type == _DT_ARRAY) {
			result = obj->data.array;
			if(o) *o = obj;
		}
	}

	return result;
}

/* Try to search a variable in a scope chain */
_var_t* _search_var_in_scope_chain(mb_interpreter_t* s, _var_t* i, _object_t** o) {
	_object_t* obj = 0;
	_ls_node_t* scp = 0;
	_var_t* result = 0;

	mb_assert(s && i);

	if(o) *o = 0;
	result = i;
	scp = _search_identifier_in_scope_chain(s, 0, result->name, _PATHING_NORMAL, 0, 0);
	if(scp) {
		obj = (_object_t*)scp->data;
		if(_IS_VAR(obj)) {
			if(o) *o = obj;
			result = obj->data.variable;
		}
	}

	return result;
}

/* Try to search an identifier accessor in a scope */
_ls_node_t* _search_identifier_accessor(mb_interpreter_t* s, _running_context_t* scope, const char* n, _ht_node_t** ht, _running_context_t** sp, bool_t unknown_for_not_found) {
	_ls_node_t* result = 0;
	_object_t* obj = 0;
	char acc[_SINGLE_SYMBOL_MAX_LENGTH];
	int i = 0;
	int j = 0;
	int nc = 0;
#ifdef MB_ENABLE_CLASS
	_class_t* instance = 0;
#else /* MB_ENABLE_CLASS */
	mb_unrefvar(unknown_for_not_found);
#endif /* MB_ENABLE_CLASS */

	mb_assert(s && n);

	while((i == 0) || (i > 0 && n[i - 1])) {
		acc[j] = n[i];
		if(_is_accessor_char(acc[j]) || acc[j] == _ZERO_CHAR) {
			acc[j] = _ZERO_CHAR;
			++nc;
			do {
#ifdef MB_ENABLE_CLASS
				if(instance) {
					result = _search_identifier_in_class(s, instance, acc, ht, sp);
					if(!result && unknown_for_not_found) {
						result = (_ls_node_t*)&_LS_NODE_UNKNOWN;

						return result;
					}

					break;
				} else if(nc > 1) {
					return 0;
				}
#endif /* MB_ENABLE_CLASS */

				result = _search_identifier_in_scope_chain(s, scope, acc, _PATHING_NONE, ht, sp);
			} while(0);

			if(!result)
				return 0;
			obj = (_object_t*)result->data;
			if(!obj)
				return 0;
			switch(obj->type) {
			case _DT_VAR:
#ifdef MB_ENABLE_USERTYPE_REF
				if(obj->data.variable->data->type == _DT_USERTYPE_REF)
					return result;
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_CLASS
				if(obj->data.variable->data->type == _DT_CLASS)
					instance = obj->data.variable->data->data.instance;
#endif /* MB_ENABLE_CLASS */

				break;
#ifdef MB_ENABLE_CLASS
			case _DT_CLASS:
				instance = obj->data.instance;

				break;
#endif /* MB_ENABLE_CLASS */
			case _DT_ARRAY: /* Fall through */
			case _DT_ROUTINE: /* Do nothing */
				break;
			default:
				mb_assert(0 && "Unsupported.");

				return 0;
			}

			j = 0;
			i++;

			continue;
		}
		j++;
		i++;
	}

	return result;
}

/* Create a variable object */
_var_t* _create_var(_object_t** oobj, const char* n, size_t ns, bool_t dup_name) {
	_object_t* obj = 0;
	_var_t* var = 0;

	var = (_var_t*)mb_malloc(sizeof(_var_t));
	memset(var, 0, sizeof(_var_t));
	if(dup_name)
		var->name = mb_strdup(n, ns);
	else
		var->name = (char*)n;
	var->data = _create_object();

	if(!oobj || !(*oobj))
		obj = _create_object();
	else
		obj = *oobj;
	_MAKE_NIL(obj);
	obj->type = _DT_VAR;
	obj->data.variable = var;
	obj->is_ref = false;

	if(oobj) *oobj = obj;

	return var;
}

/* Retrieve a variable's name and value */
int _retrieve_var(void* data, void* extra, void* t) {
	int result = _OP_RESULT_NORMAL;
	_tuple3_t* tuple = 0;
	mb_interpreter_t* s = 0;
	mb_var_retrieving_func_t retrieved = 0;
	int* count = 0;
	_object_t* obj = 0;
	_var_t* var = 0;
	_array_t* arr = 0;
	mb_value_t val;
	mb_unrefvar(extra);

	mb_assert(data && t);

	mb_make_nil(val);

	tuple = (_tuple3_t*)t;
	s = (mb_interpreter_t*)tuple->e1;
	retrieved = (mb_var_retrieving_func_t)(uintptr_t)tuple->e2;
	count = (int*)tuple->e3;

	obj = (_object_t*)data;
	switch(obj->type) {
	case _DT_VAR:
		if(retrieved) {
			var = (_var_t*)obj->data.variable;
			_internal_object_to_public_value(var->data, &val);
			retrieved(s, var->name, val);
		}
		++*count;

		break;
	case _DT_ARRAY:
		if(retrieved) {
			arr = (_array_t*)obj->data.array;
			_internal_object_to_public_value(obj, &val);
			retrieved(s, arr->name, val);
		}
		++*count;

		break;
	default: /* Do nothing */
		break;
	}

	return result;
}

/* Create an _object_t struct */
_object_t* _create_object(void) {
	_object_t* result = 0;

	result = (_object_t*)mb_malloc(sizeof(_object_t));
	_MAKE_NIL(result);

	return result;
}

/* Clone the data of an object */
int _clone_object(mb_interpreter_t* s, _object_t* obj, _object_t* tgt, bool_t toupval, bool_t deep) {
	int result = 0;

	mb_assert(obj && tgt);

	_MAKE_NIL(tgt);
	if(_is_internal_object(obj))
		goto _exit;
	tgt->type = obj->type;
	switch(obj->type) {
	case _DT_VAR:
		_clone_object(s, obj->data.variable->data, tgt, toupval, deep);

		break;
	case _DT_STRING:
		tgt->data.string = mb_strdup(obj->data.string, 0);

		break;
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		_clone_usertype_ref(obj->data.usertype_ref, tgt);

		break;
#endif /* MB_ENABLE_USERTYPE_REF */
	case _DT_FUNC:
		tgt->data.func->name = mb_strdup(obj->data.func->name, strlen(obj->data.func->name) + 1);
		tgt->data.func->pointer = obj->data.func->pointer;

		break;
	case _DT_ARRAY:
		if(deep) {
			tgt->data.array = _clone_array(s, obj->data.array);
		} else {
			tgt->data.array = obj->data.array;
#ifdef MB_ENABLE_ARRAY_REF
			_ref(&obj->data.array->ref, obj->data.array);
#endif /* MB_ENABLE_ARRAY_REF */
		}

		break;
#ifdef MB_ENABLE_COLLECTION_LIB
	case _DT_LIST:
		if(deep) {
			tgt->data.list = _create_list(obj->data.list->ref.s);
			_ref(&tgt->data.list->ref, tgt->data.list);
			_LS_FOREACH(obj->data.list->list, _do_nothing_on_object, _clone_to_list, tgt->data.list);
		} else {
			tgt->data.list = obj->data.list;
			_ref(&obj->data.list->ref, obj->data.list);
		}

		break;
	case _DT_LIST_IT:
		tgt->data.list_it = _create_list_it(obj->data.list_it->list, true);

		break;
	case _DT_DICT:
		if(deep) {
			tgt->data.dict = _create_dict(obj->data.dict->ref.s);
			_ref(&tgt->data.dict->ref, tgt->data.dict);
			_HT_FOREACH(obj->data.dict->dict, _do_nothing_on_object, _clone_to_dict, tgt->data.dict);
		} else {
			tgt->data.dict = obj->data.dict;
			_ref(&obj->data.dict->ref, obj->data.dict);
		}

		break;
	case _DT_DICT_IT:
		tgt->data.dict_it = _create_dict_it(obj->data.dict_it->dict, true);

		break;
#endif /* MB_ENABLE_COLLECTION_LIB */
	case _DT_LABEL:
		tgt->data.label->name = mb_strdup(obj->data.label->name, 0);
		tgt->data.label->node = obj->data.label->node;

		break;
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		if(deep) {
			tgt->data.instance = (_class_t*)mb_malloc(sizeof(_class_t));
			_init_class(s, tgt->data.instance, mb_strdup(obj->data.instance->name, 0));
			tgt->data.instance->created_from = obj->data.instance->created_from;
			_push_scope_by_class(s, tgt->data.instance->scope);
			_traverse_class(obj->data.instance, _clone_clsss_field, _clone_class_meta_link, _META_LIST_MAX_DEPTH, false, tgt->data.instance, 0);
			if(_search_class_hash_and_compare_functions(s, tgt->data.instance) != MB_FUNC_OK) {
				mb_assert(0 && "Impossible.");
			}
			tgt->data.instance->userdata = obj->data.instance->userdata;
			_pop_scope(s, false);
		} else {
			tgt->data.instance = obj->data.instance;
			_ref(&obj->data.instance->ref, obj->data.instance);
		}

		break;
#endif /* MB_ENABLE_CLASS */
	case _DT_ROUTINE:
		tgt->data.routine = _clone_routine(
			obj->data.routine,
#ifdef MB_ENABLE_CLASS
			obj->data.routine->instance,
#else /* MB_ENABLE_CLASS */
			0,
#endif /* MB_ENABLE_CLASS */
			toupval
		);

		break;
	case _DT_NIL: /* Fall through */
	case _DT_UNKNOWN: /* Fall through */
	case _DT_INT: /* Fall through */
	case _DT_REAL: /* Fall through */
	case _DT_TYPE: /* Fall through */
	case _DT_SEP: /* Fall through */
	case _DT_EOS: /* Fall through */
	case _DT_USERTYPE:
		tgt->data = obj->data;

		break;
	default:
		mb_assert(0 && "Invalid type.");

		break;
	}
	tgt->is_ref = false;
#ifdef MB_ENABLE_SOURCE_TRACE
	tgt->source_pos = 0;
	tgt->source_row = 0;
	tgt->source_col = 0;
#else /* MB_ENABLE_SOURCE_TRACE */
	tgt->source_pos = 0;
#endif /* MB_ENABLE_SOURCE_TRACE */
	++result;

_exit:
	return result;
}

/* Dispose the data of an object */
int _dispose_object(_object_t* obj) {
	int result = 0;
	_var_t* var = 0;

	mb_assert(obj);

	if(_is_internal_object(obj))
		goto _exit;
	switch(obj->type) {
	case _DT_VAR:
		if(!obj->is_ref) {
			var = (_var_t*)obj->data.variable;
			safe_free(var->name);
			mb_assert(var->data->type != _DT_VAR);
			if(_IS_ME(var))
				_destroy_object_capsule_only(var->data, 0);
			else
				_destroy_object(var->data, 0);
			safe_free(var);
		}

		break;
	case _DT_STRING:
		if(!obj->is_ref) {
			if(obj->data.string) {
				safe_free(obj->data.string);
			}
		}

		break;
	case _DT_FUNC:
		safe_free(obj->data.func->name);
		safe_free(obj->data.func);

		break;
	_UNREF_USERTYPE_REF(obj)
#ifdef MB_ENABLE_ARRAY_REF
	_UNREF_ARRAY(obj)
#else /* MB_ENABLE_ARRAY_REF */
	_DESTROY_ARRAY(obj)
#endif /* MB_ENABLE_ARRAY_REF */
	_UNREF_COLL(obj)
	_UNREF_COLL_IT(obj)
	_UNREF_CLASS(obj)
	_UNREF_ROUTINE(obj)
	case _DT_LABEL:
		if(!obj->is_ref) {
			safe_free(obj->data.label->name);
			safe_free(obj->data.label);
		}

		break;
#ifdef MB_ENABLE_SOURCE_TRACE
	case _DT_PREV_IMPORT: /* Fall through */
	case _DT_POST_IMPORT:
		if(!obj->is_ref) {
			if(obj->data.import_info) {
				if(obj->data.import_info->source_file) {
					safe_free(obj->data.import_info->source_file);
				}
				safe_free(obj->data.import_info);
			}
		}

		break;
#endif /* MB_ENABLE_SOURCE_TRACE */
	case _DT_NIL: /* Fall through */
	case _DT_UNKNOWN: /* Fall through */
	case _DT_INT: /* Fall through */
	case _DT_REAL: /* Fall through */
	case _DT_TYPE: /* Fall through */
	case _DT_SEP: /* Fall through */
	case _DT_EOS: /* Fall through */
	case _DT_USERTYPE: /* Do nothing */
		break;
	default:
		mb_assert(0 && "Invalid type.");

		break;
	}
	obj->is_ref = false;
	obj->type = _DT_NIL;
	memset(&obj->data, 0, sizeof(obj->data));
#ifdef MB_ENABLE_SOURCE_TRACE
	obj->source_pos = 0;
	obj->source_row = 0;
	obj->source_col = 0;
#else /* MB_ENABLE_SOURCE_TRACE */
	obj->source_pos = 0;
#endif /* MB_ENABLE_SOURCE_TRACE */
	++result;

_exit:
	return result;
}

/* Destroy an object and its data */
int _destroy_object(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	obj = (_object_t*)data;
	if(!_dispose_object(obj))
		goto _exit;
	safe_free(obj);

_exit:
	return result;
}

/* Destroy an object, including its data and extra data */
int _destroy_object_with_extra(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;

	mb_assert(data);

	obj = (_object_t*)data;
	if(!_dispose_object(obj))
		goto _exit;
	safe_free(obj);
	obj = (_object_t*)extra;
	if(!_dispose_object(obj))
		goto _exit;
	safe_free(obj);

_exit:
	return result;
}

/* Destroy an object which is not come from compile time */
int _destroy_object_not_compile_time(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	obj = (_object_t*)data;
	if(!obj->source_pos) {
		if(!_dispose_object(obj))
			goto _exit;
		safe_free(obj);
	}

_exit:
	return result;
}

/* Destroy only the capsule (wrapper) of an object, leave the data behind */
int _destroy_object_capsule_only(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	_object_t* obj = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	obj = (_object_t*)data;
	safe_free(obj);

	return result;
}

/* Do nothing with an object, this is a helper function */
int _do_nothing_on_object(void* data, void* extra) {
	int result = _OP_RESULT_NORMAL;
	mb_unrefvar(data);
	mb_unrefvar(extra);

	return result;
}

/* Lose an object in a scope */
int _lose_object(void* data, void* extra, _running_context_t* running) {
	int result = _OP_RESULT_NORMAL;
	_object_t* obj = 0;
	bool_t make_nil = true;

	mb_assert(data && extra);

	obj = (_object_t*)data;
#ifdef MB_ENABLE_LAMBDA
	if(obj->type == _DT_ROUTINE && obj->data.routine->type == MB_RT_LAMBDA)
		obj->is_ref = false;
#endif /* MB_ENABLE_LAMBDA */
#ifdef MB_ENABLE_COLLECTION_LIB
	if(obj->type == _DT_LIST_IT) {
		if((!obj->is_ref || !obj->data.list_it->locking) && running->intermediate_value.value.list_it != obj->data.list_it) {
			_destroy_list_it(obj->data.list_it); /* Process dangling value */
		}

		goto _exit;
	} else if(obj->type == _DT_DICT_IT) {
		if((!obj->is_ref || !obj->data.dict_it->locking) && running->intermediate_value.value.dict_it != obj->data.dict_it) {
			_destroy_dict_it(obj->data.dict_it); /* Process dangling value */
		}

		goto _exit;
	}
#endif /* MB_ENABLE_COLLECTION_LIB */
	switch(obj->type) {
	case _DT_VAR:
		_lose_object(obj->data.variable->data, extra, running);
		make_nil = false;

		break;
	_UNREF_USERTYPE_REF(obj)
	_UNREF_ARRAY(obj)
	_UNREF_COLL(obj)
	_UNREF_CLASS(obj)
	_UNREF_ROUTINE(obj)
	default:
		make_nil = false;

		break;
	}

#ifdef MB_ENABLE_COLLECTION_LIB
_exit:
#endif /* MB_ENABLE_COLLECTION_LIB */
	if(make_nil) {
		_MAKE_NIL(obj);
	}

	return result;
}

/* Remove an object referenced to source code */
int _remove_source_object(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	mb_unrefvar(extra);

	mb_assert(data);

	return result;
}

/* Destroy a chunk of memory */
int _destroy_memory(void* data, void* extra) {
	int result = _OP_RESULT_NORMAL;
	mb_unrefvar(extra);

	mb_assert(data);

	safe_free(data);

	return result;
}

/* Compare two numbers from two objects */
int _compare_numbers(const _object_t* first, const _object_t* second) {
	int result = 0;

	mb_assert(first && second);
	mb_assert((first->type == _DT_INT || first->type == _DT_REAL) && (second->type == _DT_INT || second->type == _DT_REAL));

	if(first->type == _DT_INT && second->type == _DT_INT) {
		if(first->data.integer > second->data.integer)
			result = 1;
		else if(first->data.integer < second->data.integer)
			result = -1;
	} else if(first->type == _DT_REAL && second->type == _DT_REAL) {
		if(first->data.float_point > second->data.float_point)
			result = 1;
		else if(first->data.float_point < second->data.float_point)
			result = -1;
	} else {
		if((first->type == _DT_INT ? (real_t)first->data.integer : first->data.float_point) > (second->type == _DT_INT ? (real_t)second->data.integer : second->data.float_point))
			result = 1;
		else if((first->type == _DT_INT ? (real_t)first->data.integer : first->data.float_point) < (second->type == _DT_INT ? (real_t)second->data.integer : second->data.float_point))
			result = -1;
	}

	return result;
}

/* Determine whether an object is internal */
bool_t _is_internal_object(_object_t* obj) {
	bool_t result = false;

	mb_assert(obj);

	result = (
		(_exp_assign == obj) ||
		(_OBJ_BOOL_TRUE == obj) || (_OBJ_BOOL_FALSE == obj)
	);

	return result;
}

/* Convert a public mb_data_e type to an internal _data_e */
_data_e _public_type_to_internal_type(mb_data_e t) {
	switch(t) {
	case MB_DT_NIL:
		return _DT_NIL;
	case MB_DT_INT:
		return _DT_INT;
	case MB_DT_REAL:
		return _DT_REAL;
	case MB_DT_STRING:
		return _DT_STRING;
	case MB_DT_TYPE:
		return _DT_TYPE;
	case MB_DT_USERTYPE:
		return _DT_USERTYPE;
#ifdef MB_ENABLE_USERTYPE_REF
	case MB_DT_USERTYPE_REF:
		return _DT_USERTYPE_REF;
#endif /* MB_ENABLE_USERTYPE_REF */
	case MB_DT_ARRAY:
		return _DT_ARRAY;
#ifdef MB_ENABLE_COLLECTION_LIB
	case MB_DT_LIST:
		return _DT_LIST;
	case MB_DT_LIST_IT:
		return _DT_LIST_IT;
	case MB_DT_DICT:
		return _DT_DICT;
	case MB_DT_DICT_IT:
		return _DT_DICT_IT;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	case MB_DT_CLASS:
		return _DT_CLASS;
#endif /* MB_ENABLE_CLASS */
	case MB_DT_ROUTINE:
		return _DT_ROUTINE;
	default:
		return _DT_UNKNOWN;
	}
}

/* Convert an internal mb_data_e type to a public _data_e */
mb_data_e _internal_type_to_public_type(_data_e t) {
	switch(t) {
	case _DT_NIL:
		return MB_DT_NIL;
	case _DT_INT:
		return MB_DT_INT;
	case _DT_REAL:
		return MB_DT_REAL;
	case _DT_STRING:
		return MB_DT_STRING;
	case _DT_TYPE:
		return MB_DT_TYPE;
	case _DT_USERTYPE:
		return MB_DT_USERTYPE;
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		return MB_DT_USERTYPE_REF;
#endif /* MB_ENABLE_USERTYPE_REF */
	case _DT_ARRAY:
		return MB_DT_ARRAY;
#ifdef MB_ENABLE_COLLECTION_LIB
	case _DT_LIST:
		return MB_DT_LIST;
	case _DT_LIST_IT:
		return MB_DT_LIST_IT;
	case _DT_DICT:
		return MB_DT_DICT;
	case _DT_DICT_IT:
		return MB_DT_DICT_IT;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		return MB_DT_CLASS;
#endif /* MB_ENABLE_CLASS */
	case _DT_ROUTINE:
		return MB_DT_ROUTINE;
	default:
		return MB_DT_UNKNOWN;
	}
}

/* Assign a public mb_value_t to an internal _object_t */
int _public_value_to_internal_object(mb_value_t* pbl, _object_t* itn) {
	int result = MB_FUNC_OK;

	mb_assert(pbl && itn);

	_UNREF(itn)

	switch(pbl->type) {
	case MB_DT_NIL:
		itn->type = _DT_NIL;
		itn->data.integer = false;

		break;
	case MB_DT_UNKNOWN:
		itn->type = _DT_UNKNOWN;
		itn->data.integer = false;

		break;
	case MB_DT_INT:
		itn->type = _DT_INT;
		itn->data.integer = pbl->value.integer;

		break;
	case MB_DT_REAL:
		itn->type = _DT_REAL;
		itn->data.float_point = pbl->value.float_point;

		break;
	case MB_DT_STRING:
		itn->type = _DT_STRING;
		itn->data.string = pbl->value.string;
		itn->is_ref = true;

		break;
	case MB_DT_TYPE:
		itn->type = _DT_TYPE;
		itn->data.type = pbl->value.type;

		break;
	case MB_DT_USERTYPE:
		itn->type = _DT_USERTYPE;
		memcpy(itn->data.bytes, pbl->value.bytes, sizeof(mb_val_bytes_t));

		break;
#ifdef MB_ENABLE_USERTYPE_REF
	case MB_DT_USERTYPE_REF:
		itn->type = _DT_USERTYPE_REF;
		itn->data.usertype_ref = (_usertype_ref_t*)pbl->value.usertype_ref;

		break;
#endif /* MB_ENABLE_USERTYPE_REF */
	case MB_DT_ARRAY:
		itn->type = _DT_ARRAY;
		itn->data.array = (_array_t*)pbl->value.array;
		itn->is_ref = false;

		break;
#ifdef MB_ENABLE_COLLECTION_LIB
	case MB_DT_LIST:
		itn->type = _DT_LIST;
		itn->data.list = (_list_t*)pbl->value.list;

		break;
	case MB_DT_LIST_IT:
		itn->type = _DT_LIST_IT;
		itn->data.list_it = (_list_it_t*)pbl->value.list_it;

		break;
	case MB_DT_DICT:
		itn->type = _DT_DICT;
		itn->data.dict = (_dict_t*)pbl->value.dict;

		break;
	case MB_DT_DICT_IT:
		itn->type = _DT_DICT_IT;
		itn->data.dict_it = (_dict_it_t*)pbl->value.dict_it;

		break;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	case MB_DT_CLASS:
		itn->type = _DT_CLASS;
		itn->data.instance = (_class_t*)pbl->value.instance;

		break;
#endif /* MB_ENABLE_CLASS */
	case MB_DT_ROUTINE:
		itn->type = _DT_ROUTINE;
		itn->data.routine = (_routine_t*)pbl->value.routine;

		break;
	default:
		result = MB_FUNC_ERR;

		break;
	}

	return result;
}

/* Assign an internal _object_t to a public mb_value_t */
int _internal_object_to_public_value(_object_t* itn, mb_value_t* pbl) {
	int result = MB_FUNC_OK;

	mb_assert(pbl && itn);

	switch(itn->type) {
	case _DT_VAR:
		if(itn->data.variable)
			result = _internal_object_to_public_value(itn->data.variable->data, pbl);

		break;
	case _DT_NIL:
		mb_make_nil(*pbl);

		break;
	case _DT_UNKNOWN:
		pbl->type = MB_DT_UNKNOWN;
		pbl->value.integer = false;

		break;
	case _DT_INT:
		pbl->type = MB_DT_INT;
		pbl->value.integer = itn->data.integer;

		break;
	case _DT_REAL:
		pbl->type = MB_DT_REAL;
		pbl->value.float_point = itn->data.float_point;

		break;
	case _DT_STRING:
		pbl->type = MB_DT_STRING;
		pbl->value.string = itn->data.string;

		break;
	case _DT_TYPE:
		pbl->type = MB_DT_TYPE;
		pbl->value.type = itn->data.type;

		break;
	case _DT_USERTYPE:
		pbl->type = MB_DT_USERTYPE;
		memcpy(pbl->value.bytes, itn->data.bytes, sizeof(mb_val_bytes_t));

		break;
#ifdef MB_ENABLE_USERTYPE_REF
	case _DT_USERTYPE_REF:
		pbl->type = MB_DT_USERTYPE_REF;
		pbl->value.usertype_ref = itn->data.usertype_ref;

		break;
#endif /* MB_ENABLE_USERTYPE_REF */
	case _DT_ARRAY:
		pbl->type = MB_DT_ARRAY;
		pbl->value.array = itn->data.array;

		break;
#ifdef MB_ENABLE_COLLECTION_LIB
	case _DT_LIST:
		pbl->type = MB_DT_LIST;
		pbl->value.list = itn->data.list;

		break;
	case _DT_LIST_IT:
		pbl->type = MB_DT_LIST_IT;
		pbl->value.list_it = itn->data.list;

		break;
	case _DT_DICT:
		pbl->type = MB_DT_DICT;
		pbl->value.dict = itn->data.dict;

		break;
	case _DT_DICT_IT:
		pbl->type = MB_DT_DICT_IT;
		pbl->value.dict_it = itn->data.dict_it;

		break;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		pbl->type = MB_DT_CLASS;
		pbl->value.instance = itn->data.instance;

		break;
#endif /* MB_ENABLE_CLASS */
	case _DT_ROUTINE:
		pbl->type = MB_DT_ROUTINE;
		pbl->value.routine = itn->data.routine;

		break;
	default:
		result = MB_FUNC_ERR;

		break;
	}

	return result;
}

/* Create an internal object from a public value */
int _create_internal_object_from_public_value(mb_value_t* pbl, _object_t** itn) {
	int result = MB_FUNC_OK;

	mb_assert(pbl && itn);

	*itn = _create_object();
	_public_value_to_internal_object(pbl, *itn);
	if((*itn)->type == _DT_STRING) {
		(*itn)->is_ref = false;
		(*itn)->data.string = mb_strdup((*itn)->data.string, strlen((*itn)->data.string) + 1);
	}

	return result;
}

/* Compare a public value and an internal object */
int _compare_public_value_and_internal_object(mb_value_t* pbl, _object_t* itn) {
	int result = 0;
	mb_value_t tmp;

	mb_make_nil(tmp);
	_internal_object_to_public_value(itn, &tmp);
	if(pbl->type != tmp.type) {
		result = pbl->type - tmp.type;
	} else {
		switch(pbl->type) {
		case MB_DT_NIL:
			result = 0;

			break;
		case MB_DT_INT:
			result = mb_memcmp(&pbl->value.integer, &tmp.value.integer, sizeof(int_t));

			break;
		case MB_DT_REAL:
			result = mb_memcmp(&pbl->value.float_point, &tmp.value.float_point, sizeof(real_t));

			break;
		case MB_DT_STRING:
			result = mb_memcmp(&pbl->value.string, &tmp.value.string, sizeof(char*));

			break;
		case MB_DT_TYPE:
			result = mb_memcmp(&pbl->value.type, &tmp.value.type, sizeof(mb_data_e));

			break;
		case MB_DT_USERTYPE:
			result = mb_memcmp(&pbl->value.bytes, &tmp.value.bytes, sizeof(mb_val_bytes_t));

			break;
#ifdef MB_ENABLE_USERTYPE_REF
		case MB_DT_USERTYPE_REF:
			result = mb_memcmp(&pbl->value.usertype_ref, &tmp.value.usertype_ref, sizeof(void*));

			break;
#endif /* MB_ENABLE_USERTYPE_REF */
		case MB_DT_ARRAY:
			result = mb_memcmp(&pbl->value.array, &tmp.value.array, sizeof(void*));

			break;
#ifdef MB_ENABLE_COLLECTION_LIB
		case MB_DT_LIST:
			result = mb_memcmp(&pbl->value.list, &tmp.value.list, sizeof(void*));

			break;
		case MB_DT_LIST_IT:
			result = mb_memcmp(&pbl->value.list_it, &tmp.value.list_it, sizeof(void*));

			break;
		case MB_DT_DICT:
			result = mb_memcmp(&pbl->value.dict, &tmp.value.dict, sizeof(void*));

			break;
		case MB_DT_DICT_IT:
			result = mb_memcmp(&pbl->value.dict_it, &tmp.value.dict_it, sizeof(void*));

			break;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
		case MB_DT_CLASS:
			result = mb_memcmp(&pbl->value.instance, &tmp.value.instance, sizeof(void*));

			break;
#endif /* MB_ENABLE_CLASS */
		case MB_DT_ROUTINE:
			result = mb_memcmp(&pbl->value.routine, &tmp.value.routine, sizeof(void*));

			break;
		default:
			result = mb_memcmp(pbl->value.bytes, tmp.value.bytes, sizeof(mb_val_bytes_t));

			break;
		}
	}

	return result;
}

/* Try clear the intermediate value */
void _try_clear_intermediate_value(void* data, void* extra, mb_interpreter_t* s) {
	_object_t* obj = 0;
	_running_context_t* running = 0;
	mb_unrefvar(extra);

	mb_assert(s);

	if(data == 0)
		return;

	obj = (_object_t*)data;
	running = s->running_context;

	if(!_compare_public_value_and_internal_object(&running->intermediate_value, obj)) {
		mb_make_nil(running->intermediate_value);
	}
}

/* Remove from another list if exists */
void _remove_if_exists(void* data, void* extra, _ls_node_t* ls) {
	_object_t* obj = 0;
	mb_unrefvar(extra);

	obj = (_object_t*)data;
	_ls_try_remove(ls, obj, _ls_cmp_data, 0);
}

/* Destroy an object in variable argument list */
void _destroy_var_arg(void* data, void* extra, _gc_t* gc) {
	_object_t* obj = 0;
	mb_unrefvar(extra);
	mb_unrefvar(gc);

	mb_assert(data);

	obj = (_object_t*)data;
	_UNREF(obj)
	safe_free(obj);
}

/* Destroy edge destroying objects */
void _destroy_edge_objects(mb_interpreter_t* s) {
	if(!s) return;

	_LS_FOREACH(s->edge_destroy_objects, _destroy_object, _try_clear_intermediate_value, s);
	_ls_clear(s->edge_destroy_objects);
}

/* Mark a string as an edge destroying object */
void _mark_edge_destroy_string(mb_interpreter_t* s, char* ch) {
	_object_t* temp_obj = 0;

	mb_assert(s && ch);

	temp_obj = _create_object();
	temp_obj->type = _DT_STRING;
	temp_obj->is_ref = false;
	temp_obj->data.string = ch;
	_ls_pushback(s->edge_destroy_objects, temp_obj);
}

/* Destroy lazy destroying objects */
void _destroy_lazy_objects(mb_interpreter_t* s) {
	mb_assert(s);

	_LS_FOREACH(s->lazy_destroy_objects, _destroy_object, _try_clear_intermediate_value, s);
	_ls_clear(s->lazy_destroy_objects);
}

/* Mark a string as a lazy destroying object */
void _mark_lazy_destroy_string(mb_interpreter_t* s, char* ch) {
	_object_t* temp_obj = 0;

	mb_assert(s && ch);

	temp_obj = _create_object();
	temp_obj->type = _DT_STRING;
	temp_obj->is_ref = false;
	temp_obj->data.string = ch;
	_ls_pushback(s->lazy_destroy_objects, temp_obj);
}

/* Assign a value with another */
void _assign_public_value(mb_interpreter_t* s, mb_value_t* tgt, mb_value_t* src, bool_t pit) {
	_object_t obj;
	mb_value_t nil;

	mb_assert(tgt);

#ifdef MB_ENABLE_COLLECTION_LIB
	if(pit && _try_purge_it(s, tgt, 0))
		return;
#else /* MB_ENABLE_COLLECTION_LIB */
	mb_unrefvar(s);
	mb_unrefvar(pit);
#endif /* MB_ENABLE_COLLECTION_LIB */

	_MAKE_NIL(&obj);
	_public_value_to_internal_object(tgt, &obj);
	_UNREF(&obj)

	mb_make_nil(nil);
	if(!src)
		src = &nil;
	memcpy(tgt, src, sizeof(mb_value_t));
	*src = nil;
}

/* Swap two public values */
void _swap_public_value(mb_value_t* tgt, mb_value_t* src) {
	mb_value_t tmp;

	mb_assert(tgt && src);

	tmp = *tgt;
	*tgt = *src;
	*src = tmp;
}

/* Clear the scope chain */
int _clear_scope_chain(mb_interpreter_t* s) {
	int result = 0;
	_running_context_t* running = 0;
	_running_context_t* prev = 0;

	mb_assert(s);

	running = s->running_context;
	while(running) {
		prev = running->prev;

		_ht_foreach(running->var_dict, _destroy_object);
		_ht_clear(running->var_dict);
#ifdef MB_ENABLE_LAMBDA
		if(running->refered_lambdas)
			_ls_clear(running->refered_lambdas);
#endif /* MB_ENABLE_LAMBDA */

		result++;
		running = prev;
	}

	return result;
}

/* Dispose the scope chain */
int _dispose_scope_chain(mb_interpreter_t* s) {
	int result = 0;
	_running_context_t* running = 0;
	_running_context_t* prev = 0;

	mb_assert(s);

	running = s->running_context;
	while(running) {
		prev = running->prev;

		_ht_foreach(running->var_dict, _destroy_object);
		_ht_clear(running->var_dict);
		_ht_destroy(running->var_dict);
#ifdef MB_ENABLE_LAMBDA
		if(running->refered_lambdas) {
			_ls_clear(running->refered_lambdas);
			_ls_destroy(running->refered_lambdas);
			running->refered_lambdas = 0;
		}
#endif /* MB_ENABLE_LAMBDA */
		running->var_dict = 0;
		mb_dispose_value(s, running->intermediate_value);
		safe_free(running);

		result++;
		running = prev;
	}
	s->running_context = 0;

	return result;
}

/* Tidy the scope chain */
void _tidy_scope_chain(mb_interpreter_t* s) {
	_parsing_context_t* context = 0;

	mb_assert(s);

	context = s->parsing_context;
	if(!context) {
		while(s->running_context->prev)
			_pop_scope(s, false);

		return;
	}

	while(context->routine_state && s->running_context->meta != _SCOPE_META_ROOT) {
		if(_end_routine(s)) {
			if(!s->running_context->prev)
				break;
			_pop_scope(s, false);
		}
	}
#ifdef MB_ENABLE_CLASS
	while(context->class_state != _CLASS_STATE_NONE) {
		if(_end_class(s)) {
			if(!s->running_context->prev)
				break;
			_pop_scope(s, false);
		}
	}
#endif /* MB_ENABLE_CLASS */
}

/* Collect the intermediate value in a scope */
void _collect_intermediate_value_in_scope(_running_context_t* running, void* data) {
	_object_t tmp;

	mb_assert(running && data);

	if(!running)
		return;

	_MAKE_NIL(&tmp);
	_public_value_to_internal_object(&running->intermediate_value, &tmp);
	if(tmp.data.pointer == data) {
		switch(tmp.type) {
#ifdef MB_ENABLE_USERTYPE_REF
		case _DT_USERTYPE_REF: /* Fall through */
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_COLLECTION_LIB
		case _DT_LIST: /* Fall through */
		case _DT_DICT: /* Fall through */
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
		case _DT_CLASS: /* Fall through */
#endif /* MB_ENABLE_CLASS */
		case _DT_ARRAY:
			mb_make_nil(running->intermediate_value);

			break;
		default: /* Do nothing */
			break;
		}
	}
}

#ifdef MB_ENABLE_FORK
/* Collect all intermediate values in scope chain */
void _collect_intermediate_value_in_scope_chain(void* data, void* extra, void* d) {
	mb_interpreter_t* s = 0;
	_running_context_t* running = 0;
	mb_unrefvar(extra);

	s = (mb_interpreter_t*)data;
	running = s->running_context;
	while(running) {
		_collect_intermediate_value_in_scope(running, d);
		running = running->prev;
	}
}
#endif /* MB_ENABLE_FORK */

/* Collect the intermediate value */
void _collect_intermediate_value(_ref_t* ref, void* data) {
	mb_interpreter_t* s = 0;

	mb_assert(ref && data);

	s = ref->s;
	if(!s) return;
	if(s->running_context)
		_collect_intermediate_value_in_scope(s->running_context, data);
#ifdef MB_ENABLE_FORK
	if(s->all_forked) {
		while(mb_get_forked_from(s, &s) == MB_FUNC_OK) {
			/* Do nothing */
		}
		_LS_FOREACH(s->all_forked, _do_nothing_on_object, _collect_intermediate_value_in_scope_chain, data);
	}
#endif /* MB_ENABLE_FORK */
}

/* Mark the intermediate value to be collected if it's dangling */
void _mark_dangling_intermediate_value(mb_interpreter_t* s, _running_context_t* running) {
	mb_assert(s && running);

	switch(running->intermediate_value.type) {
#ifdef MB_ENABLE_USERTYPE_REF
	case MB_DT_USERTYPE_REF: /* Fall through */
#endif /* MB_ENABLE_USERTYPE_REF */
#ifdef MB_ENABLE_COLLECTION_LIB
	case MB_DT_LIST: /* Fall through */
	case MB_DT_DICT: /* Fall through */
	case MB_DT_LIST_IT: /* Fall through */
	case MB_DT_DICT_IT: /* Fall through */
#endif /* MB_ENABLE_COLLECTION_LIB */
	case MB_DT_ROUTINE: {
			_object_t tmp;
			_MAKE_NIL(&tmp);
			_public_value_to_internal_object(&running->intermediate_value, &tmp);
#ifdef MB_ENABLE_COLLECTION_LIB
			if(tmp.type == _DT_LIST_IT && tmp.data.list_it->locking)
				break;
			else if(tmp.type == _DT_DICT_IT && tmp.data.dict_it->locking)
				break;
#endif /* MB_ENABLE_COLLECTION_LIB */
			_ADDGC(&tmp, &s->gc, false) /* Process dangling value */
		}

		break;
	default: /* Do nothing */
		break;
	}
}

/* Evaluate a variable, this is a helper function for the PRINT statement */
_object_t* _eval_var_in_print(mb_interpreter_t* s, _object_t** val_ptr, _ls_node_t** ast, _object_t* obj) {
	_object_t tmp;

	mb_assert(s);

	switch(obj->type) {
	case _DT_ROUTINE:
		_execute_statement(s, ast, true);
		_MAKE_NIL(&tmp);
		_public_value_to_internal_object(&s->running_context->intermediate_value, &tmp);
		if(tmp.type == _DT_STRING) {
			tmp.data.string = mb_strdup(tmp.data.string, strlen(tmp.data.string) + 1);
			tmp.is_ref = false;
			mb_make_nil(s->running_context->intermediate_value);
		}
		**val_ptr = tmp;
		if(obj->data.routine->type != MB_RT_NATIVE) {
			if(*ast)
				*ast = (*ast)->prev;
		}

		break;
	case _DT_VAR:
		*val_ptr = obj->data.variable->data;
		if(*ast) *ast = (*ast)->next;

		break;
	default:
		*val_ptr = obj;
		if(*ast) *ast = (*ast)->next;

		break;
	}

	return *val_ptr;
}

/** Interpretation */

/* Callback a stepped debug handler, this function is called before each step */
int _prev_stepped(mb_interpreter_t* s, _ls_node_t* ast) {
	int result = MB_FUNC_OK;
	_object_t* obj = 0;

	mb_assert(s);

	if(s->debug_prev_stepped_handler) {
		if(ast && ast->data) {
			obj = (_object_t*)ast->data;
#ifdef MB_ENABLE_SOURCE_TRACE
			result = s->debug_prev_stepped_handler(s, (void**)&ast, s->source_file, obj->source_pos, obj->source_row, obj->source_col);
#else /* MB_ENABLE_SOURCE_TRACE */
			result = s->debug_prev_stepped_handler(s, (void**)&ast, s->source_file, obj->source_pos, 0, 0);
#endif /* MB_ENABLE_SOURCE_TRACE */
		} else {
			result = s->debug_prev_stepped_handler(s, (void**)&ast, s->source_file, -1, 0, 0);
		}
	}

	return result;
}

/* Callback a stepped debug handler, this function is called after each step */
int _post_stepped(mb_interpreter_t* s, _ls_node_t* ast) {
	int result = MB_FUNC_OK;
	_object_t* obj = 0;

	mb_assert(s);

	if(s->debug_post_stepped_handler) {
		if(ast && ast->data) {
			obj = (_object_t*)ast->data;
#ifdef MB_ENABLE_SOURCE_TRACE
			result = s->debug_post_stepped_handler(s, (void**)&ast, s->source_file, obj->source_pos, obj->source_row, obj->source_col);
#else /* MB_ENABLE_SOURCE_TRACE */
			result = s->debug_post_stepped_handler(s, (void**)&ast, s->source_file, obj->source_pos, 0, 0);
#endif /* MB_ENABLE_SOURCE_TRACE */
		} else {
			result = s->debug_post_stepped_handler(s, (void**)&ast, s->source_file, -1, 0, 0);
		}
	}

	return result;
}

/* Execute the ast, this is the core execution function */
int _execute_statement(mb_interpreter_t* s, _ls_node_t** l, bool_t force_next) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_running_context_t* running = 0;
	_ls_node_t* sub_stack = 0;
	bool_t skip_to_eoi = true;
	bool_t end_of_ast = false;

	mb_assert(s && l);

	running = s->running_context;
	sub_stack = s->sub_stack;

	ast = *l;

	obj = (_object_t*)ast->data;

	result = _prev_stepped(s, ast);
	if(result != MB_FUNC_OK)
		goto _exit;

_retry:
	switch(obj->type) {
	case _DT_FUNC:
		if(_is_binary(obj->data.func->pointer)) {
			_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		if(_is_flow(obj->data.func->pointer)) {
			result = (obj->data.func->pointer)(s, (void**)&ast);
		} else {
#ifdef MB_ENABLE_STACK_TRACE
			_ls_pushback(s->stack_frames, obj->data.func->name);
#endif /* MB_ENABLE_STACK_TRACE */
			result = (obj->data.func->pointer)(s, (void**)&ast);
#ifdef MB_ENABLE_STACK_TRACE
			_ls_popback(s->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */
			_mark_dangling_intermediate_value(s, running);
		}
		if(result == MB_FUNC_IGNORE) {
			result = MB_FUNC_OK;
			obj = (_object_t*)ast->data;

			goto _retry;
		}

		break;
	case _DT_VAR:
#ifdef MB_ENABLE_CLASS
		if(obj->data.variable->data->type == _DT_ROUTINE) {
			if(ast && ast->next && _IS_FUNC(ast->next->data, _core_open_bracket)) {
				obj = obj->data.variable->data;

				goto _retry;
			} else {
				result = _core_let(s, (void**)&ast);
			}
		} else if(s->last_instance || obj->data.variable->pathing) {
			/* Need to path */
			_ls_node_t* pathed = _search_identifier_in_scope_chain(s, 0, obj->data.variable->name, _PU(obj->data.variable->pathing), 0, 0);
			if(pathed && pathed->data) {
				if(obj != (_object_t*)pathed->data) {
					/* Found another node */
#ifdef MB_ENABLE_USERTYPE_REF
					if(_try_call_func_on_usertype_ref(s, &ast, obj, pathed, &result))
						break;
#endif /* MB_ENABLE_USERTYPE_REF */

					obj = (_object_t*)pathed->data;

					goto _retry;
				} else {
					/* Final node */
					result = _core_let(s, (void**)&ast);
				}
			} else {
				/* Normal node */
				result = _core_let(s, (void**)&ast);
			}
		} else {
			/* Do not need to path */
			result = _core_let(s, (void**)&ast);
		}
#else /* MB_ENABLE_CLASS */
		result = _core_let(s, (void**)&ast);
#endif /* MB_ENABLE_CLASS */

		break;
	case _DT_ARRAY:
		result = _core_let(s, (void**)&ast);

		break;
	case _DT_INT: /* Fall through */
	case _DT_REAL: /* Fall through */
	case _DT_STRING:
		_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);

		break;
#ifdef MB_ENABLE_CLASS
	case _DT_CLASS:
		_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);

		break;
#endif /* MB_ENABLE_CLASS */
	case _DT_ROUTINE:
		ast = ast->prev;
		result = _core_call(s, (void**)&ast);
		_mark_dangling_intermediate_value(s, running);

		break;
#ifdef MB_ENABLE_SOURCE_TRACE
	case _DT_PREV_IMPORT:
		s->source_file = obj->data.import_info->source_file;

		break;
	case _DT_POST_IMPORT:
		s->source_file = obj->data.import_info->source_file;

		break;
#endif /* MB_ENABLE_SOURCE_TRACE */
	default: /* Do nothing */
		break;
	}

	if(s->schedule_suspend_tag) {
		if(s->schedule_suspend_tag == MB_FUNC_SUSPEND)
			mb_suspend(s, (void**)&ast);
		result = s->schedule_suspend_tag;
		s->schedule_suspend_tag = 0;
	}

	if(result != MB_FUNC_OK && result != MB_FUNC_SUSPEND && result != MB_SUB_RETURN)
		goto _exit;

	if(ast && ast != s->ast) {
		obj = DON(ast);
		if(!obj) {
			/* Do nothing */
		} else if(_IS_EOS(obj)) {
			if(force_next || result != MB_SUB_RETURN)
				ast = ast->next;
		} else if(_IS_SEP(obj, ':')) {
			skip_to_eoi = false;
			ast = ast->next;
		} else if(_IS_VAR(obj)) {
#ifdef MB_ENABLE_CLASS
			_ls_node_t* fn = 0;
			if(_is_valid_class_accessor_following_routine(s, obj->data.variable, ast, &fn)) {
				if(fn) {
					if(s->calling)
						result = _core_let(s, (void**)&ast);
				} else {
					_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
				}
			} else {
				_handle_error_on_obj(s, SE_RN_COLON_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
#else /* MB_ENABLE_CLASS */
			_handle_error_on_obj(s, SE_RN_COLON_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
#endif /* MB_ENABLE_CLASS */
		} else if(_IS_FUNC(obj, _core_enddef) && result != MB_SUB_RETURN) {
			ast = (_ls_node_t*)_ls_popback(sub_stack);
#ifdef MB_ENABLE_LAMBDA
		} else if(obj && _IS_FUNC(obj, _core_close_bracket) && s->last_routine && s->last_routine->type == MB_RT_LAMBDA) {
			/* Do nothing */
#endif /* MB_ENABLE_LAMBDA */
		} else if(obj && obj->type == _DT_FUNC && (_is_operator(obj->data.func->pointer) || _is_flow(obj->data.func->pointer))) {
			ast = ast->next;
		} else if(obj && obj->type == _DT_FUNC) {
			/* Do nothing */
		} else if(obj && obj->type != _DT_FUNC) {
			ast = ast->next;
		} else {
			_handle_error_on_obj(s, SE_RN_COLON_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
	}

	if(skip_to_eoi && s->skip_to_eoi && s->skip_to_eoi == _ls_back(s->sub_stack)) {
		s->skip_to_eoi = 0;
		obj = (_object_t*)ast->data;
		if(!_IS_EOS(obj)) {
			result = _skip_to(s, &ast, 0, _DT_EOS);
			if(result != MB_FUNC_OK)
				goto _exit;
		}
	}

_exit:
	_destroy_lazy_objects(s);

	*l = ast;

	if(!ast) {
		ast = _ls_back(s->ast);
		end_of_ast = true;
	}

	if(ast == s->ast) {
		*l = ast->next;
	} else {
		int ret = _post_stepped(s, ast);
		if(result == MB_FUNC_OK)
			result = ret;

		if(end_of_ast && ast && ast->next) /* May be changed when stepping */
			*l = ast->next;
	}

	return result;
}

/* Common function to end current looping */
int _common_end_looping(mb_interpreter_t* s, _ls_node_t** l) {
	int result = MB_FUNC_OK;

	mb_assert(s && l);

	result = _skip_struct(s, l, _core_for, 0, _core_next);
	if(result == MB_FUNC_OK)
		result = _skip_to(s, l, 0, _DT_EOS);

	return result;
}

/* Common function to keep current looping */
int _common_keep_looping(mb_interpreter_t* s, _ls_node_t** l, _var_t* var_loop) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_running_context_t* running = 0;

	mb_assert(s && l);

	running = s->running_context;
	ast = *l;

	obj = (_object_t*)ast->data;
	while(!_IS_FUNC(obj, _core_next)) {
		result = _execute_statement(s, &ast, false);
		if(result == MB_LOOP_CONTINUE) { /* NEXT */
			if(!running->next_loop_var || running->next_loop_var == var_loop) { /* This loop */
				running->next_loop_var = 0;
				result = MB_FUNC_OK;

				break;
			} else { /* Not this loop */
				if(_skip_struct(s, &ast, _core_for, 0, _core_next) != MB_FUNC_OK)
					goto _exit;
				_skip_to(s, &ast, 0, _DT_EOS);

				goto _exit;
			}
		} else if(result == MB_LOOP_BREAK) { /* EXIT */
			if(_skip_struct(s, &ast, _core_for, 0, _core_next) != MB_FUNC_OK)
				goto _exit;
			_skip_to(s, &ast, 0, _DT_EOS);

			goto _exit;
		} else if(result == MB_SUB_RETURN) { /* RETURN */
			goto _exit;
		} else if(result != MB_FUNC_OK) { /* Normally */
			goto _exit;
		}

		if(!ast) {
			_handle_error_on_obj(s, SE_RN_NEXT_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		obj = (_object_t*)ast->data;
	}

_exit:
	*l = ast;

	return result;
}

/* Execute normal FOR-TO-STEP-NEXT-routine */
int _execute_normal_for_loop(mb_interpreter_t* s, _ls_node_t** l, _var_t* var_loop) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_ls_node_t* to_node = 0;
	_object_t* obj = 0;
	_object_t to_val;
	_object_t step_val;
	_object_t* to_val_ptr = 0;
	_object_t* step_val_ptr = 0;
	_tuple3_t ass_tuple3;
	_tuple3_t* ass_tuple3_ptr = 0;

	mb_assert(s && l && var_loop);

	ast = *l;

	to_val_ptr = &to_val;
	_MAKE_NIL(to_val_ptr);
	step_val_ptr = &step_val;
	_MAKE_NIL(step_val_ptr);
	ass_tuple3_ptr = &ass_tuple3;

	/* Get begin value */
	result = _execute_statement(s, &ast, true);
	if(result != MB_FUNC_OK)
		goto _exit;
	if(!ast)
		goto _exit;
	ast = ast->prev;

	obj = (_object_t*)ast->data;
	if(!_IS_FUNC(obj, _core_to)) {
		_handle_error_on_obj(s, SE_RN_TO_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

	ast = ast->next;
	if(!ast) {
		_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	to_node = ast;

_to:
	ast = to_node;

	/* Get end value */
	result = _calc_expression(s, &ast, &to_val_ptr);
	if(result != MB_FUNC_OK)
		goto _exit;

	if(ast) {
		obj = (_object_t*)ast->data;
		if(!_IS_FUNC(obj, _core_step)) {
			step_val = _OBJ_INT_UNIT;
		} else {
			ast = ast->next;
			if(!ast) {
				_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}

			/* Get step value */
			result = _calc_expression(s, &ast, &step_val_ptr);
			if(result != MB_FUNC_OK)
				goto _exit;
		}
	}

	if((_compare_numbers(step_val_ptr, &_OBJ_INT_ZERO) > 0 && _compare_numbers(var_loop->data, to_val_ptr) > 0) ||
		(_compare_numbers(step_val_ptr, &_OBJ_INT_ZERO) < 0 && _compare_numbers(var_loop->data, to_val_ptr) < 0)) {
		/* End looping */
		result = _common_end_looping(s, &ast);

		goto _exit;
	} else {
		/* Keep looping */
		if(!ast)
			goto _exit;
		result = _common_keep_looping(s, &ast, var_loop);
		if(result == MB_LOOP_BREAK) {
			result = MB_FUNC_OK;

			goto _exit;
		} else if(result != MB_FUNC_OK || result == MB_SUB_RETURN) {
			goto _exit;
		}

		ass_tuple3.e1 = var_loop->data;
		ass_tuple3.e2 = step_val_ptr;
		ass_tuple3.e3 = var_loop->data;
		_instruct_num_op_num(+, &ass_tuple3_ptr);

		goto _to;
	}

_exit:
	*l = ast;

	return result;
}


/* Skip current execution flow to a specific function */
int _skip_to(mb_interpreter_t* s, _ls_node_t** l, mb_func_t f, _data_e t) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_ls_node_t* tmp = 0;
	_object_t* obj = 0;

	mb_assert(s && l);

	ast = *l;
	do {
		if(!ast) {
			_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(tmp), MB_FUNC_ERR, _exit, result);
		}
		tmp = ast;
		obj = (_object_t*)ast->data;
		*l = ast;
		ast = ast->next;
	} while(!(_IS_FUNC(obj, f)) && obj->type != t);

_exit:
	return result;
}

/* Skip single line structure */
bool_t _skip_single_line_struct(_ls_node_t** ast, mb_func_t func) {
	_ls_node_t* post = *ast;
	while(post && !_IS_EOS(post->data))
		post = post->next;
	if(post && post->prev && !_IS_FUNC(post->prev->data, func)) {
		*ast = post;

		return true;
	}

	return false;
}

/* Skip current IF execution flow to next chunk */
int _skip_if_chunk(mb_interpreter_t* s, _ls_node_t** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_ls_node_t* tmp = 0;
	_object_t* obj = 0;
	int nested = 0;
	unsigned mask = 0;

	mb_assert(s && l);

	ast = *l;
	mb_assert(ast && ast->prev);
	do {
		if(!ast) {
			_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
		tmp = ast;
		obj = (_object_t*)ast->data;
		*l = ast;
		ast = ast->next;
		if(ast && _IS_FUNC((_object_t*)ast->data, _core_if)) {
			if(_skip_single_line_struct(&ast, _core_then))
				continue;
			if(++nested > sizeof(mask) * 8) {
				_handle_error_on_obj(s, SE_RN_TOO_MANY_NESTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}
		} else if(ast && nested && _IS_FUNC((_object_t*)ast->data, _core_then)) {
			if(!(ast && ast->next && _IS_EOS(ast->next->data)))
				mask |= 1 << (nested - 1);
		} else if(ast && nested &&
			(((mask & (1 << (nested - 1))) && _IS_EOS(ast->data)) ||
			(!(mask & (1 << (nested - 1))) && _IS_FUNC((_object_t*)ast->data, _core_endif)))
		) {
			if(--nested < 0) {
				_handle_error_on_obj(s, SE_RN_INCOMPLETE_STRUCTURE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}
			ast = ast->next;
		}
	} while(nested || (!_IS_FUNC(obj, _core_elseif) && !_IS_FUNC(obj, _core_else) && !_IS_FUNC(obj, _core_endif)));

_exit:
	return result;
}

/* Skip current structure */
int _skip_struct(mb_interpreter_t* s, _ls_node_t** l, mb_func_t open_func, mb_func_t post_open_func, mb_func_t close_func) {
	int result = MB_FUNC_OK;
	int count = 0;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_object_t* obj_prev = 0;

	mb_assert(s && l && open_func && close_func);

	ast = *l;

	count = 1;
	do {
		if(!ast || !ast->next) {
			_handle_error_on_obj(s, SE_RN_INCOMPLETE_STRUCTURE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		obj_prev = (_object_t*)ast->data;
		ast = ast->next;
		obj = (_object_t*)ast->data;
		if(_IS_FUNC(obj, open_func)) {
			if(post_open_func) {
				if(_skip_single_line_struct(&ast, post_open_func))
					continue;
			}
			++count;
		} else if(_IS_FUNC(obj, close_func) && _IS_EOS(obj_prev)) {
			--count;
		}
	} while(count);

_exit:
	*l = ast;

	return result;
}

/* Check whether multiline statement is allowed */
bool_t _multiline_statement(mb_interpreter_t* s) {
#if _MULTILINE_STATEMENT
	if(_ls_empty(s->multiline_enabled))
		return false;

	return (bool_t)(intptr_t)_ls_back(s->multiline_enabled)->data;
#else /* _MULTILINE_STATEMENT */
	mb_unrefvar(s);

	return false;
#endif /* _MULTILINE_STATEMENT */
}

/* Create a running context */
_running_context_t* _create_running_context(bool_t create_var_dict) {
	_running_context_t* result = 0;

	result = (_running_context_t*)mb_malloc(sizeof(_running_context_t));
	memset(result, 0, sizeof(_running_context_t));
	result->calc_depth = _INFINITY_CALC_DEPTH;
	if(create_var_dict)
		result->var_dict = _ht_create(0, _ht_cmp_string, _ht_hash_string, 0);

	return result;
}

/* Reset the parsing context of a MY-BASIC environment */
_parsing_context_t* _reset_parsing_context(_parsing_context_t* context) {
	_ls_node_t* imp = 0;

	if(!context)
		context = (_parsing_context_t*)mb_malloc(sizeof(_parsing_context_t));
	else
		imp = context->imported;
	memset(context, 0, sizeof(_parsing_context_t));
	context->parsing_row = 1;
	if(!imp) {
		imp = _ls_create();
	} else {
		_ls_foreach(imp, _destroy_memory);
		_ls_clear(imp);
	}
	context->imported = imp;
	_end_of_file(context);

	return context;
}

/* Destroy the parsing context of a MY-BASIC environment */
void _destroy_parsing_context(_parsing_context_t* _UNALIGNED_ARG * context) {
	if(!context || !(*context))
		return;

	if(*context) {
		if((*context)->imported) {
			_ls_foreach((*context)->imported, _destroy_memory);
			_ls_destroy((*context)->imported);
		}
		safe_free(*context);
	}
}

/** Interface processors */

#ifdef MB_ENABLE_MODULE
/* Create a module function structure */
_module_func_t* _create_module_func(mb_interpreter_t* s, mb_func_t f) {
	_module_func_t* result = 0;

	mb_assert(s);

	if(!s->with_module)
		return result;

	result = (_module_func_t*)mb_malloc(sizeof(_module_func_t));
	result->module = mb_strdup(s->with_module, 0);
	result->func = f;

	return result;
}

/* Destroy a module function structure */
int _ls_destroy_module_func(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	_module_func_t* mod = 0;
	mb_unrefvar(extra);

	mb_assert(data);

	mod = (_module_func_t*)data;
	safe_free(mod->module);
	safe_free(mod);

	return result;
}

/* Destroy all module function structures */
int _ht_destroy_module_func_list(void* data, void* extra) {
	int result = _OP_RESULT_DEL_NODE;
	_ls_node_t* lst = 0;
	char* n = 0;

	mb_assert(data);

	lst = (_ls_node_t*)data;
	n = (char*)extra;
	_ls_foreach(lst, _ls_destroy_module_func);
	_ls_destroy(lst);
	safe_free(n);

	return result;
}
#endif /* MB_ENABLE_MODULE */

/* Generate a function name to be registered according to module information */
char* _generate_func_name(mb_interpreter_t* s, char* n, bool_t with_mod) {
	char* name = 0;
	size_t _sl = 0;

	mb_assert(s && n);

	_sl = strlen(n);
#ifdef MB_ENABLE_MODULE
	if(with_mod && s->with_module) {
		size_t _ml = strlen(s->with_module);
		name = (char*)mb_malloc(_ml + 1 + _sl + 1);
		memcpy(name, s->with_module, _ml);
		name[_ml] = '.';
		memcpy(name + _ml + 1, n, _sl + 1);
	} else {
		name = (char*)mb_malloc(_sl + 1);
		memcpy(name, n, _sl + 1);
	}
#else /* MB_ENABLE_MODULE */
	mb_unrefvar(with_mod);
	name = (char*)mb_malloc(_sl + 1);
	memcpy(name, n, _sl + 1);
#endif /* MB_ENABLE_MODULE */
	mb_strupr(name);

	return name;
}

/* Register a function to a MY-BASIC environment */
int _register_func(mb_interpreter_t* s, char* n, mb_func_t f, bool_t local) {
	int result = 0;
	_ht_node_t* scope = 0;
	_ls_node_t* exists = 0;
	char* name = 0;

	mb_assert(s);

	if(!n)
		return result;

	n = mb_strdup(n, strlen(n) + 1);
	mb_strupr(n);

	scope = local ? s->local_func_dict : s->global_func_dict;
#ifdef MB_ENABLE_MODULE
	if(s->with_module)
		name = _generate_func_name(s, n, true);
#endif /* MB_ENABLE_MODULE */
	if(!name)
		name = mb_strdup(n, strlen(n) + 1);
	exists = _ht_find(scope, (void*)name);
	if(!exists) {
		result += _ht_set_or_insert(scope, (void*)name, (void*)(intptr_t)f);
	} else {
		_set_current_error(s, SE_CM_FUNC_EXISTS, 0);
		safe_free(name);
	}

#ifdef MB_ENABLE_MODULE
	if(s->with_module) {
		_ls_node_t* tmp = 0;
		exists = _ht_find(s->module_func_dict, (void*)n);
		if(!exists) {
			name = _generate_func_name(s, n, false);
			result += _ht_set_or_insert(s->module_func_dict, (void*)name, _ls_create());
		}
		exists = _ht_find(s->module_func_dict, (void*)n);
		exists = (_ls_node_t*)exists->data;
		tmp = _ls_find(exists, s, _ls_cmp_module_func, 0);
		if(!tmp)
			_ls_pushback(exists, _create_module_func(s, f));
		else
			_set_current_error(s, SE_CM_FUNC_EXISTS, 0);
	}
#endif /* MB_ENABLE_MODULE */

	safe_free(n);

	return result;
}

/* Remove a function from a MY-BASIC environment */
int _remove_func(mb_interpreter_t* s, char* n, bool_t local) {
	int result = 0;
	_ht_node_t* scope = 0;
	_ls_node_t* exists = 0;
	char* name = 0;

	mb_assert(s);

	if(!n)
		return result;

	n = mb_strdup(n, strlen(n) + 1);
	mb_strupr(n);

	scope = local ? s->local_func_dict : s->global_func_dict;
#ifdef MB_ENABLE_MODULE
	if(s->with_module)
		name = _generate_func_name(s, n, true);
#endif /* MB_ENABLE_MODULE */
	if(!name)
		name = mb_strdup(n, strlen(n) + 1);
	exists = _ht_find(scope, (void*)name);
	if(exists)
		result += _ht_remove(scope, (void*)name, _ls_cmp_extra_string);
	else
		_set_current_error(s, SE_CM_FUNC_DOES_NOT_EXIST, 0);
	safe_free(name);

#ifdef MB_ENABLE_MODULE
	if(s->with_module) {
		_ls_node_t* tmp = 0;
		exists = _ht_find(s->module_func_dict, (void*)n);
		if(exists) {
			exists = (_ls_node_t*)exists->data;
			tmp = _ls_find(exists, s, _ls_cmp_module_func, 0);
			if(tmp)
				_ls_remove(exists, tmp, _ls_destroy_module_func);
		}
	}
#endif /* MB_ENABLE_MODULE */

	safe_free(n);

	return result;
}

/* Find function interface in the function dictionaries */
_ls_node_t* _find_func(mb_interpreter_t* s, char* n, bool_t* mod) {
	_ls_node_t* result = 0;
	mb_unrefvar(mod);

	mb_assert(s && n);

	n = mb_strdup(n, strlen(n) + 1);
	mb_strupr(n);

	result = _ht_find(s->local_func_dict, (void*)n);
	if(!result)
		result = _ht_find(s->global_func_dict, (void*)n);

#ifdef MB_ENABLE_MODULE
	if(!result) {
		result = _ht_find(s->module_func_dict, (void*)n);
		if(result && result->data) {
			_module_func_t* mp = 0;
			result = (_ls_node_t*)result->data;
			result = result->next;
			while(result) {
				mp = (_module_func_t*)result->data;
				if(_ls_find(s->using_modules, mp->module, (_ls_compare_t)_ht_cmp_string, 0))
					break;
				result = result->next;
			}
			*mod = true;
		}
	}
#endif /* MB_ENABLE_MODULE */

	safe_free(n);

	return result;
}

/* Open global constants */
static int _open_constant(mb_interpreter_t* s) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	unsigned long ul = 0;

	mb_assert(s);

	running = s->running_context;

	ul = _ht_set_or_insert(running->var_dict, (void*)MB_TRUE, _OBJ_BOOL_TRUE);
	mb_assert(ul);
	ul = _ht_set_or_insert(running->var_dict, (void*)MB_FALSE, _OBJ_BOOL_FALSE);
	mb_assert(ul);

	return result;
}

/* Close global constants */
static int _close_constant(mb_interpreter_t* s) {
	int result = MB_FUNC_OK;

	mb_assert(s);

	return result;
}


/* ========================================================} */

/*
** {========================================================
** Public functions definitions
*/

/* Get the version number of this MY-BASIC system */
unsigned long mb_ver(void) {
	return MB_VERSION;
}

/* Get the version text of this MY-BASIC system */
const char* mb_ver_string(void) {
	return MB_VERSION_STRING;
}

/* Initialize the MY-BASIC system */
int mb_init(void) {
	if(_exp_assign)
		return MB_FUNC_ERR;

	_exp_assign = _create_object();
	_exp_assign->type = _DT_FUNC;
	_exp_assign->data.func = (_func_t*)mb_malloc(sizeof(_func_t));
	memset(_exp_assign->data.func, 0, sizeof(_func_t));
	_exp_assign->data.func->name = (char*)mb_malloc(strlen(_DUMMY_ASSIGN_CHAR) + 1);
	memcpy(_exp_assign->data.func->name, _DUMMY_ASSIGN_CHAR, strlen(_DUMMY_ASSIGN_CHAR) + 1);
	_exp_assign->data.func->pointer = _core_dummy_assign;

	mb_assert(!_OBJ_BOOL_TRUE);
	if(!_OBJ_BOOL_TRUE) {
		_var_t* bvar = _create_var(&_OBJ_BOOL_TRUE, MB_TRUE, strlen(MB_TRUE) + 1, true);
		bvar->data->type = _DT_INT;
		bvar->data->data.integer = 1;
	}
	mb_assert(!_OBJ_BOOL_FALSE);
	if(!_OBJ_BOOL_FALSE) {
		_var_t* bvar = _create_var(&_OBJ_BOOL_FALSE, MB_FALSE, strlen(MB_FALSE) + 1, true);
		bvar->data->type = _DT_INT;
		bvar->data->data.integer = 0;
	}

	return MB_FUNC_OK;
}

/* Close the MY-BASIC system */
int mb_dispose(void) {
	if(!_exp_assign)
		return MB_FUNC_ERR;

	safe_free(_exp_assign->data.func->name);
	safe_free(_exp_assign->data.func);
	safe_free(_exp_assign);
	_exp_assign = 0;

	mb_assert(_OBJ_BOOL_TRUE);
	if(_OBJ_BOOL_TRUE) {
		safe_free(_OBJ_BOOL_TRUE->data.variable->data);
		safe_free(_OBJ_BOOL_TRUE->data.variable->name);
		safe_free(_OBJ_BOOL_TRUE->data.variable);
		safe_free(_OBJ_BOOL_TRUE);
		_OBJ_BOOL_TRUE = 0;
	}
	mb_assert(_OBJ_BOOL_FALSE);
	if(_OBJ_BOOL_FALSE) {
		safe_free(_OBJ_BOOL_FALSE->data.variable->data);
		safe_free(_OBJ_BOOL_FALSE->data.variable->name);
		safe_free(_OBJ_BOOL_FALSE->data.variable);
		safe_free(_OBJ_BOOL_FALSE);
		_OBJ_BOOL_FALSE = 0;
	}

	return MB_FUNC_OK;
}

/* Open a MY-BASIC environment */
int mb_open(struct mb_interpreter_t** s) {
	int result = MB_FUNC_OK;
	_ht_node_t* local_scope = 0;
	_ht_node_t* global_scope = 0;
	_running_context_t* running = 0;

	if(!s)
		return MB_FUNC_ERR;

	*s = (mb_interpreter_t*)mb_malloc(sizeof(mb_interpreter_t));
	memset(*s, 0, sizeof(mb_interpreter_t));
	(*s)->valid = true;

	local_scope = _ht_create(0, _ht_cmp_string, _ht_hash_string, _ls_free_extra);
	(*s)->local_func_dict = local_scope;

	global_scope = _ht_create(0, _ht_cmp_string, _ht_hash_string, _ls_free_extra);
	(*s)->global_func_dict = global_scope;

#ifdef MB_ENABLE_MODULE
	global_scope = _ht_create(0, _ht_cmp_string, _ht_hash_string, _ht_destroy_module_func_list);
	(*s)->module_func_dict = global_scope;
	(*s)->using_modules = _ls_create();
#endif /* MB_ENABLE_MODULE */

	(*s)->parsing_context = _reset_parsing_context((*s)->parsing_context);

	(*s)->edge_destroy_objects = _ls_create();
	(*s)->lazy_destroy_objects = _ls_create();

	(*s)->gc.table = _ht_create(0, _ht_cmp_ref, _ht_hash_ref, _do_nothing_on_object);
	(*s)->gc.recursive_table = _ht_create(0, _ht_cmp_ref, _ht_hash_ref, _do_nothing_on_object);
	(*s)->gc.collected_table = _ht_create(0, _ht_cmp_ref, _ht_hash_ref, _do_nothing_on_object);
	(*s)->gc.valid_table = 0;
	(*s)->gc.collecting = 0;

	running = _create_running_context(true);
	running->meta = _SCOPE_META_ROOT;
	(*s)->running_context = running;

	(*s)->sub_stack = _ls_create();

	(*s)->in_neg_expr = _ls_create();

#ifdef MB_ENABLE_STACK_TRACE
	(*s)->stack_frames = _ls_create();
#endif /* MB_ENABLE_STACK_TRACE */
#if _MULTILINE_STATEMENT
	(*s)->multiline_enabled = _ls_create();
#endif /* _MULTILINE_STATEMENT */

	(*s)->ast = _ls_create();

	_MB_OPEN_LIBRARY(core, *s);
	_MB_OPEN_LIBRARY(std, *s);
#ifdef MB_ENABLE_COLLECTION_LIB
	_MB_OPEN_LIBRARY(coll, *s);
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_DIRECT_MAMORY_MANIPULATION_LIB
	_MB_OPEN_LIBRARY(dmm, *s);
#endif /* MB_ENABLE_DIRECT_MAMORY_MANIPULATION_LIB */

	result = _open_constant(*s);
	mb_assert(MB_FUNC_OK == result);

	return result;
}

/* Close a MY-BASIC environment */
int mb_close(struct mb_interpreter_t** s) {
	_ht_node_t* local_scope = 0;
	_ht_node_t* global_scope = 0;
	_ls_node_t* ast = 0;

	if(!s || !(*s))
		return MB_FUNC_ERR;

#ifdef MB_ENABLE_FORK
	if((*s)->forked_from)
		return mb_join(s);
#endif /* MB_ENABLE_FORK */

	(*s)->valid = false;

#ifdef MB_ENABLE_DIRECT_MAMORY_MANIPULATION_LIB
	_MB_CLOSE_LIBRARY(dmm, *s);
#endif /* MB_ENABLE_DIRECT_MAMORY_MANIPULATION_LIB */
#ifdef MB_ENABLE_COLLECTION_LIB
	_MB_CLOSE_LIBRARY(coll, *s);
#endif /* MB_ENABLE_COLLECTION_LIB */
	_MB_CLOSE_LIBRARY(std, *s);
	_MB_CLOSE_LIBRARY(core, *s);

	ast = (*s)->ast;
	_LS_FOREACH(ast, _destroy_object, _try_clear_intermediate_value, *s);
	_ls_destroy(ast);

	_ls_destroy((*s)->sub_stack);

	_ls_destroy((*s)->in_neg_expr);

#ifdef MB_ENABLE_STACK_TRACE
	_ls_destroy((*s)->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */
#if _MULTILINE_STATEMENT
	_ls_destroy((*s)->multiline_enabled);
#endif /* _MULTILINE_STATEMENT */

	_tidy_scope_chain(*s);
	_dispose_scope_chain(*s);

	(*s)->gc.disabled = false;
	_gc_collect_garbage(*s, -1);
	_ht_destroy((*s)->gc.table);
	_ht_destroy((*s)->gc.recursive_table);
	_ht_destroy((*s)->gc.collected_table);
	(*s)->gc.table = 0;
	(*s)->gc.recursive_table = 0;
	(*s)->gc.collected_table = 0;

#ifdef MB_ENABLE_FORK
	if((*s)->all_forked) {
		mb_assert(_ls_count((*s)->all_forked) == 0);
		_ls_destroy((*s)->all_forked);
	}
#endif /* MB_ENABLE_FORK */

	_ls_foreach((*s)->edge_destroy_objects, _destroy_object);
	_ls_destroy((*s)->edge_destroy_objects);
	_ls_foreach((*s)->lazy_destroy_objects, _destroy_object);
	_ls_destroy((*s)->lazy_destroy_objects);

	_destroy_parsing_context(&(*s)->parsing_context);

#ifdef MB_ENABLE_MODULE
	global_scope = (*s)->module_func_dict;
	_ht_foreach(global_scope, _ht_destroy_module_func_list);
	_ht_destroy(global_scope);
	_ls_foreach((*s)->using_modules, _destroy_memory);
	_ls_destroy((*s)->using_modules);
#endif /* MB_ENABLE_MODULE */

	global_scope = (*s)->global_func_dict;
	_ht_foreach(global_scope, _ls_free_extra);
	_ht_destroy(global_scope);

	local_scope = (*s)->local_func_dict;
	_ht_foreach(local_scope, _ls_free_extra);
	_ht_destroy(local_scope);

	_close_constant(*s);

	safe_free(*s);

	return MB_FUNC_OK;
}

/* Reset a MY-BASIC environment */
int mb_reset(struct mb_interpreter_t** s, bool_t clear_funcs, bool_t clear_vars) {
	int result = MB_FUNC_OK;
	_ht_node_t* global_scope = 0;
	_ls_node_t* ast;
	_running_context_t* running = 0;

	if(!s || !(*s))
		return MB_FUNC_ERR;

	(*s)->valid = false;

	(*s)->run_count = 0;
	(*s)->has_run = false;
	(*s)->jump_set = _JMP_NIL;
#ifdef MB_ENABLE_CLASS
	(*s)->last_instance = 0;
	(*s)->calling = false;
#endif /* MB_ENABLE_CLASS */
	(*s)->last_routine = 0;
	(*s)->no_eat_comma_mark = 0;
	(*s)->handled_error = false;
	(*s)->last_error = SE_NO_ERR;
	(*s)->last_error_file = 0;

	running = (*s)->running_context;
	(*s)->suspent_point = 0;
	running->next_loop_var = 0;
	memset(&(running->intermediate_value), 0, sizeof(mb_value_t));

	ast = (*s)->ast;
	_LS_FOREACH(ast, _destroy_object, _try_clear_intermediate_value, *s);
	_ls_clear(ast);

	_ls_clear((*s)->sub_stack);

#ifdef MB_ENABLE_STACK_TRACE
	_ls_clear((*s)->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */
#if _MULTILINE_STATEMENT
	_ls_clear((*s)->multiline_enabled);
#endif /* _MULTILINE_STATEMENT */

	if(clear_vars) {
		_tidy_scope_chain(*s);
		_clear_scope_chain(*s);
	}

#ifdef MB_ENABLE_FORK
	if((*s)->all_forked) {
		mb_assert(_ls_count((*s)->all_forked) == 0);
		_ls_clear((*s)->all_forked);
	}
#endif /* MB_ENABLE_FORK */

	(*s)->parsing_context = _reset_parsing_context((*s)->parsing_context);

	if(clear_funcs) {
#ifdef MB_ENABLE_MODULE
		global_scope = (*s)->module_func_dict;
		_ht_foreach(global_scope, _ht_destroy_module_func_list);
		_ht_clear(global_scope);
		_ls_foreach((*s)->using_modules, _destroy_memory);
		_ls_clear((*s)->using_modules);
#endif /* MB_ENABLE_MODULE */

		global_scope = (*s)->global_func_dict;
		_ht_foreach(global_scope, _ls_free_extra);
		_ht_clear(global_scope);
	}

	result = _open_constant(*s);
	mb_assert(MB_FUNC_OK == result);

	(*s)->valid = true;

	return result;
}

/* Fork a new MY-BASIC environment */
int mb_fork(struct mb_interpreter_t** s, struct mb_interpreter_t* r, bool_t clear_forked) {
#ifdef MB_ENABLE_FORK
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;

	if(!s || !r)
		return MB_FUNC_ERR;

	*s = (mb_interpreter_t*)mb_malloc(sizeof(mb_interpreter_t));
	memcpy(*s, r, sizeof(mb_interpreter_t));

	(*s)->edge_destroy_objects = _ls_create();
	(*s)->lazy_destroy_objects = _ls_create();

	running = _create_running_context(true);
	running->meta = _SCOPE_META_ROOT;
	(*s)->forked_context = (*s)->running_context = running;
	running->prev = _get_root_scope(r->running_context);

	(*s)->var_args = 0;

	(*s)->sub_stack = _ls_create();

	(*s)->in_neg_expr = _ls_create();

#ifdef MB_ENABLE_STACK_TRACE
	(*s)->stack_frames = _ls_create();
#endif /* MB_ENABLE_STACK_TRACE */
#if _MULTILINE_STATEMENT
	(*s)->multiline_enabled = _ls_create();
#endif /* _MULTILINE_STATEMENT */

	(*s)->forked_from = r;

	if(clear_forked) {
		if(!r->all_forked)
			r->all_forked = _ls_create();
		_ls_pushback(r->all_forked, *s);
	}

	mb_assert(MB_FUNC_OK == result);

	return result;
#else /* MB_ENABLE_FORK */
	mb_unrefvar(s);
	mb_unrefvar(r);
	mb_unrefvar(clear_forked);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_FORK */
}

/* Join a forked MY-BASIC environment */
int mb_join(struct mb_interpreter_t** s) {
#ifdef MB_ENABLE_FORK
	int result = MB_FUNC_OK;
	mb_interpreter_t* src = 0;

	if(!s || !(*s) || !(*s)->forked_from)
		return MB_FUNC_ERR;

	src = *s;
	while(mb_get_forked_from(src, &src) == MB_FUNC_OK) {
		/* Do nothing */
	}

	(*s)->valid = false;

	_ls_destroy((*s)->sub_stack);

	_ls_destroy((*s)->in_neg_expr);

#ifdef MB_ENABLE_STACK_TRACE
	_ls_destroy((*s)->stack_frames);
#endif /* MB_ENABLE_STACK_TRACE */
#if _MULTILINE_STATEMENT
	_ls_destroy((*s)->multiline_enabled);
#endif /* _MULTILINE_STATEMENT */

	(*s)->forked_context->prev = 0;
	(*s)->running_context = (*s)->forked_context;
	_dispose_scope_chain(*s);

	_ls_foreach((*s)->edge_destroy_objects, _destroy_object);
	_ls_destroy((*s)->edge_destroy_objects);
	_ls_foreach((*s)->lazy_destroy_objects, _destroy_object);
	_ls_destroy((*s)->lazy_destroy_objects);

	if(src->all_forked)
		_ls_try_remove(src->all_forked, *s, _ls_cmp_data, 0);

	safe_free(*s);

	return result;
#else /* MB_ENABLE_FORK */
	mb_unrefvar(s);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_FORK */
}

/* Get the source MY-BASIC environment of a forked one */
int mb_get_forked_from(struct mb_interpreter_t* s, struct mb_interpreter_t** src) {
#ifdef MB_ENABLE_FORK
	int result = MB_FUNC_OK;

	if(!s || !src)
		result = MB_FUNC_ERR;
	else if(s->forked_from == 0)
		result = MB_FUNC_ERR;
	else
		*src = s->forked_from;

	return result;
#else /* MB_ENABLE_FORK */
	mb_unrefvar(s);
	mb_unrefvar(src);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_FORK */
}

/* Register an API function to a MY-BASIC environment */
int mb_register_func(struct mb_interpreter_t* s, const char* n, mb_func_t f) {
	if(!s || !n || !f) return 0;

	return _register_func(s, (char*)n, f, false);
}

/* Remove an API function from a MY-BASIC environment */
int mb_remove_func(struct mb_interpreter_t* s, const char* n) {
	if(!s || !n) return 0;

	return _remove_func(s, (char*)n, false);
}

/* Remove a reserved API from a MY-BASIC environment */
int mb_remove_reserved_func(struct mb_interpreter_t* s, const char* n) {
	if(!s || !n) return 0;

	return _remove_func(s, (char*)n, true);
}

/* Begin a module, all functions registered within a module will put inside it */
int mb_begin_module(struct mb_interpreter_t* s, const char* n) {
	int result = MB_FUNC_OK;

	if(!s || !n) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

#ifdef MB_ENABLE_MODULE
	if(s->with_module) {
		_handle_error_on_obj(s, SE_PS_INVALID_MODULE, s->source_file, (_object_t*)0, MB_FUNC_ERR, _exit, result);
	} else {
		s->with_module = mb_strdup(n, strlen(n) + 1);
	}
#else /* MB_ENABLE_MODULE */
	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, (_object_t*)0, MB_FUNC_WARNING, _exit, result);
#endif /* MB_ENABLE_MODULE */

_exit:
	return result;
}

/* End a module */
int mb_end_module(struct mb_interpreter_t* s) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

#ifdef MB_ENABLE_MODULE
	if(s->with_module) {
		safe_free(s->with_module);
	} else {
		_handle_error_on_obj(s, SE_PS_INVALID_MODULE, s->source_file, (_object_t*)0, MB_FUNC_ERR, _exit, result);
	}

#else /* MB_ENABLE_MODULE */
	_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, (_object_t*)0, MB_FUNC_WARNING, _exit, result);
#endif /* MB_ENABLE_MODULE */

_exit:
	return result;
}

/* Try attempting to begin an API function */
int mb_attempt_func_begin(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

#if _MULTILINE_STATEMENT
	_ls_pushback(s->multiline_enabled, (void*)(intptr_t)false);
#endif /* _MULTILINE_STATEMENT */
	ast = (_ls_node_t*)*l;
	if(!ast) {
		result = MB_FUNC_ERR;

		goto _exit;
	}
	obj = (_object_t*)ast->data;
	if(!(obj->type == _DT_FUNC)) {
#if _MULTILINE_STATEMENT
		_ls_popback(s->multiline_enabled);
#endif /* _MULTILINE_STATEMENT */
		_handle_error_on_obj(s, SE_RN_INCOMPLETE_STRUCTURE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;

	++s->no_eat_comma_mark;

_exit:
	*l = ast;

	return result;
}

/* Try attempting to end an API function */
int mb_attempt_func_end(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;

	if(!s || !l) {
		result = MB_FUNC_ERR;
	} else {
#if _MULTILINE_STATEMENT
		_ls_popback(s->multiline_enabled);
#endif /* _MULTILINE_STATEMENT */
		--s->no_eat_comma_mark;
	}

	return result;
}

/* Try attempting an open bracket */
int mb_attempt_open_bracket(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	ast = (_ls_node_t*)*l;
#if _MULTILINE_STATEMENT
	_ls_pushback(s->multiline_enabled, (void*)(intptr_t)true);
	do {
		ast = ast->next;
		obj = ast ? (_object_t*)ast->data : 0;
	} while(_IS_EOS(obj));
#else /* _MULTILINE_STATEMENT */
	ast = ast->next;
	obj = ast ? (_object_t*)ast->data : 0;
#endif /* _MULTILINE_STATEMENT */
	if(!obj || !_IS_FUNC(obj, _core_open_bracket)) {
#if _MULTILINE_STATEMENT
		_ls_popback(s->multiline_enabled);
#endif /* _MULTILINE_STATEMENT */
		_handle_error_on_obj(s, SE_RN_OPEN_BRACKET_EXPECTED, s->source_file, ast ? DON(ast) : DON2(l), MB_FUNC_ERR, _exit, result);
	}
	if(ast) ast = ast->next;

_exit:
	*l = ast;

	return result;
}

/* Try attempting a close bracket */
int mb_attempt_close_bracket(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	ast = (_ls_node_t*)*l;
	if(!ast) {
		_handle_error_on_obj(s, SE_RN_CLOSE_BRACKET_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
#if _MULTILINE_STATEMENT
	_ls_popback(s->multiline_enabled);
	obj = (_object_t*)ast->data;
	while(_IS_EOS(obj)) {
		ast = ast->next;
		obj = ast ? (_object_t*)ast->data : 0;
	}
#else /* _MULTILINE_STATEMENT */
	obj = ast ? (_object_t*)ast->data : 0;
#endif /* _MULTILINE_STATEMENT */
	if(!obj || !_IS_FUNC(obj, _core_close_bracket)) {
		_handle_error_on_obj(s, SE_RN_CLOSE_BRACKET_EXPECTED, s->source_file, ast ? DON(ast) : DON2(l), MB_FUNC_ERR, _exit, result);
	}
	if(ast) ast = ast->next;

_exit:
	*l = ast;

	return result;
}

/* Detect if there is any more argument */
int mb_has_arg(struct mb_interpreter_t* s, void** l) {
	int result = 0;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;

	if(!s || !l)
		goto _exit;

	ast = (_ls_node_t*)*l;
	if(ast) {
#if _MULTILINE_STATEMENT
		if(_multiline_statement(s)) {
			obj = (_object_t*)ast->data;
			while(_IS_EOS(obj)) {
				ast = ast->next;
				obj = ast ? (_object_t*)ast->data : 0;
			}
		} else {
			obj = (_object_t*)ast->data;
		}
#else /* _MULTILINE_STATEMENT */
		obj = ast ? (_object_t*)ast->data : 0;
#endif /* _MULTILINE_STATEMENT */
		if(obj && !_IS_FUNC(obj, _core_close_bracket) && !_IS_EOS(obj))
			result = obj->type != _DT_SEP && obj->type != _DT_EOS;
	}

_exit:
	return result;
}

/* Pop an integer argument */
int mb_pop_int(struct mb_interpreter_t* s, void** l, int_t* val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(val) *val = 0;

	if(!s || !l || !val) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(arg);

	mb_check(mb_pop_value(s, l, &arg));

	switch(arg.type) {
	case MB_DT_INT:
		*val = arg.value.integer;

		break;
	case MB_DT_REAL:
		*val = (int_t)(arg.value.float_point);

		break;
	default:
		_assign_public_value(s, &arg, 0, false);
#if _SIMPLE_ARG_ERROR
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#else /* _SIMPLE_ARG_ERROR */
		result = MB_FUNC_ERR;
#endif /* _SIMPLE_ARG_ERROR */

		goto _exit;
	}

_exit:
	return result;
}

/* Pop a float point argument */
int mb_pop_real(struct mb_interpreter_t* s, void** l, real_t* val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(val) *val = 0;

	if(!s || !l || !val) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(arg);

	mb_check(mb_pop_value(s, l, &arg));

	switch(arg.type) {
	case MB_DT_INT:
		*val = (real_t)(arg.value.integer);

		break;
	case MB_DT_REAL:
		*val = arg.value.float_point;

		break;
	default:
		_assign_public_value(s, &arg, 0, false);
#if _SIMPLE_ARG_ERROR
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#else /* _SIMPLE_ARG_ERROR */
		result = MB_FUNC_ERR;
#endif /* _SIMPLE_ARG_ERROR */

		goto _exit;
	}

_exit:
	return result;
}

/* Pop a string argument */
int mb_pop_string(struct mb_interpreter_t* s, void** l, char** val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(val) *val = 0;

	if(!s || !l || !val) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(arg);

	mb_check(mb_pop_value(s, l, &arg));

	switch(arg.type) {
	case MB_DT_STRING:
		*val = arg.value.string;

		break;
	default:
		_assign_public_value(s, &arg, 0, false);
#if _SIMPLE_ARG_ERROR
		_handle_error_on_obj(s, SE_RN_STRING_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
#else /* _SIMPLE_ARG_ERROR */
		result = MB_FUNC_ERR;
#endif /* _SIMPLE_ARG_ERROR */

		goto _exit;
	}

_exit:
	return result;
}

/* Pop a usertype argument */
int mb_pop_usertype(struct mb_interpreter_t* s, void** l, void** val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(val) *val = 0;

	if(!s || !l || !val) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(arg);

	mb_check(mb_pop_value(s, l, &arg));

	switch(arg.type) {
	case MB_DT_USERTYPE:
		*val = arg.value.usertype;

		break;
	default:
		_assign_public_value(s, &arg, 0, false);
		result = MB_FUNC_ERR;

		goto _exit;
	}

_exit:
	return result;
}

/* Pop an argument value */
int mb_pop_value(struct mb_interpreter_t* s, void** l, mb_value_t* val) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t val_obj;
	_object_t* val_ptr = 0;
	_running_context_t* running = 0;
	int* inep = 0;

	if(!s || !l || !val) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;

	if(!_ls_empty(s->in_neg_expr))
		inep = (int*)_ls_back(s->in_neg_expr)->data;

	val_ptr = &val_obj;
	_MAKE_NIL(val_ptr);

#ifdef MB_ENABLE_USERTYPE_REF
	if(s->usertype_ref_ahead) {
		ast = (_ls_node_t*)*l;
		memcpy(val_ptr, s->usertype_ref_ahead, sizeof(_object_t));
		s->usertype_ref_ahead = 0;

		goto _got;
	}
#endif /* MB_ENABLE_USERTYPE_REF */
	ast = (_ls_node_t*)*l;
	if(!ast)
		goto _exit;
#if _MULTILINE_STATEMENT
	if(_multiline_statement(s)) {
		_object_t* obj = 0;
		obj = (_object_t*)ast->data;
		while(_IS_EOS(obj)) {
			ast = ast->next;
			obj = ast ? (_object_t*)ast->data : 0;
		}
	}
#endif /* _MULTILINE_STATEMENT */
	result = _calc_expression(s, &ast, &val_ptr);
	if(result != MB_FUNC_OK)
		goto _exit;

#ifdef MB_ENABLE_USERTYPE_REF
_got:
#endif /* MB_ENABLE_USERTYPE_REF */
	if(val_ptr->type == _DT_STRING && !val_ptr->is_ref) {
		_destroy_edge_objects(s);
		_mark_edge_destroy_string(s, val_ptr->data.string);
	}
	_REF(val_ptr)

	if(s->no_eat_comma_mark < _NO_EAT_COMMA && (!inep || (inep && !(*inep)))) {
		if(ast && _IS_SEP(ast->data, ','))
			ast = ast->next;
	}

	result = _internal_object_to_public_value(val_ptr, val);
	if(result != MB_FUNC_OK)
		goto _exit;

_exit:
	*l = ast;

	return result;
}

/* Push an integer argument */
int mb_push_int(struct mb_interpreter_t* s, void** l, int_t val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_int(arg, val);
	mb_check(mb_push_value(s, l, arg));

_exit:
	return result;
}

/* Push a float point argument */
int mb_push_real(struct mb_interpreter_t* s, void** l, real_t val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_real(arg, val);
	mb_convert_to_int_if_posible(arg);
	mb_check(mb_push_value(s, l, arg));

_exit:
	return result;
}

/* Push a string argument */
int mb_push_string(struct mb_interpreter_t* s, void** l, char* val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_string(arg, val);
	mb_check(mb_push_value(s, l, arg));
	_mark_lazy_destroy_string(s, val);

_exit:
	return result;
}

/* Push a usertype argument */
int mb_push_usertype(struct mb_interpreter_t* s, void** l, void* val) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_usertype(arg, val);
	mb_check(mb_push_value(s, l, arg));

_exit:
	return result;
}

/* Push an argument value */
int mb_push_value(struct mb_interpreter_t* s, void** l, mb_value_t val) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	_object_t obj;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;
	_assign_public_value(s, &running->intermediate_value, &val, false);

	_MAKE_NIL(&obj);
	_public_value_to_internal_object(&running->intermediate_value, &obj);
	_REF(&obj)

	_gc_try_trigger(s);

_exit:
	return result;
}

/* Begin a class */
int mb_begin_class(struct mb_interpreter_t* s, void** l, const char* n, mb_value_t** meta, int c, mb_value_t* out) {
#ifdef MB_ENABLE_CLASS
	int result = MB_FUNC_OK;
	_class_t* instance = 0;
	_object_t* obj = 0;
	_running_context_t* running = 0;
	_ls_node_t* tmp = 0;
	_var_t* var = 0;
	int i = 0;
	_object_t mo;
	_class_t* mi = 0;

	if(!s || !l || !n || !out) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;

	tmp = (_ls_node_t*)*l;

	_using_jump_set_of_structured(s, tmp, _exit, result);

	tmp = _search_identifier_in_scope_chain(s, 0, n, _PATHING_NONE, 0, 0);
	if(tmp && tmp->data) {
		obj = (_object_t*)tmp->data;
		if(_IS_VAR(obj))
			var = obj->data.variable;
	}
	if(s->last_instance || (obj && !var)) {
		_handle_error_on_obj(s, SE_RN_DUPLICATE_CLASS, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	obj = _create_object();
	obj->type = _DT_CLASS;

	instance = (_class_t*)mb_malloc(sizeof(_class_t));
	_init_class(s, instance, mb_strdup(n, strlen(n) + 1));

	for(i = 0; i < c; i++) {
		if(meta[i]->type != MB_DT_CLASS) {
			_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
		_MAKE_NIL(&mo);
		_public_value_to_internal_object(meta[i], &mo);
		mi = mo.data.instance;
		_link_meta_class(s, instance, mi);
	}

	_push_scope_by_class(s, instance->scope);
	obj->data.instance = instance;

	if(var) {
		_destroy_object(var->data, 0);
		var->data = obj;
	} else {
		_ht_set_or_insert(running->var_dict, (void*)n, obj);
	}

	s->last_instance = instance;

	if(out) {
		out->type = MB_DT_CLASS;
		out->value.instance = instance;
	}

_exit:
	return result;
#else /* MB_ENABLE_CLASS */
	mb_unrefvar(s);
	mb_unrefvar(l);
	mb_unrefvar(n);
	mb_unrefvar(meta);
	mb_unrefvar(c);
	mb_unrefvar(out);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_CLASS */
}

/* End a class */
int mb_end_class(struct mb_interpreter_t* s, void** l) {
#ifdef MB_ENABLE_CLASS
	int result = MB_FUNC_OK;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	_pop_scope(s, false);

	s->last_instance = 0;

_exit:
	return result;
#else /* MB_ENABLE_CLASS */
	mb_unrefvar(s);
	mb_unrefvar(l);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_CLASS */
}

/* Get the userdata of a class instance */
int mb_get_class_userdata(struct mb_interpreter_t* s, void** l, void** d) {
#ifdef MB_ENABLE_CLASS
	int result = MB_FUNC_OK;

	if(!s || !d) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(s && s->last_instance) {
		if(d)
			*d = s->last_instance->userdata;
	} else if(s && s->last_routine && s->last_routine->instance) {
		if(d)
			*d = s->last_routine->instance->userdata;
	} else {
		if(d) *d = 0;

		_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

_exit:
	return result;
#else /* MB_ENABLE_CLASS */
	mb_unrefvar(s);
	mb_unrefvar(l);
	mb_unrefvar(d);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_CLASS */
}

/* Set the userdata of a class instance */
int mb_set_class_userdata(struct mb_interpreter_t* s, void** l, void* d) {
#ifdef MB_ENABLE_CLASS
	int result = MB_FUNC_OK;

	if(!s || !d) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(s && s->last_instance) {
		s->last_instance->userdata = d;
	} else {
		_handle_error_on_obj(s, SE_RN_CLASS_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

_exit:
	return result;
#else /* MB_ENABLE_CLASS */
	mb_unrefvar(s);
	mb_unrefvar(l);
	mb_unrefvar(d);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_CLASS */
}

/* Get a value by its identifier name */
int mb_get_value_by_name(struct mb_interpreter_t* s, void** l, const char* n, mb_value_t* val) {
	int result = MB_FUNC_OK;
	_ls_node_t* tmp = 0;
	_object_t* obj = 0;
	mb_unrefvar(l);

	if(!s || !n) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(*val);

	tmp = _search_identifier_in_scope_chain(s, 0, n, _PATHING_NORMAL, 0, 0);
	if(tmp && tmp->data) {
		obj = (_object_t*)tmp->data;
		_internal_object_to_public_value(obj, val);
	}

_exit:
	return result;
}

/* Retrieve all variables at the current frame */
int mb_get_vars(struct mb_interpreter_t* s, void** l, mb_var_retrieving_func_t r, int stack_offset) {
	int result = 0;
	_running_context_t* running = 0;
	_tuple3_t tuple;
	mb_unrefvar(l);

	if(!s)
		goto _exit;

	running = s->running_context;
	if(stack_offset == -1) {
		running = _get_root_scope(running);
	} else {
		while(stack_offset > 0 && running) {
			running = running->prev;
			--stack_offset;
		}
	}
	if(!running)
		goto _exit;
	tuple.e1 = s;
	tuple.e2 = (void*)(uintptr_t)r;
	tuple.e3 = &result;
	_HT_FOREACH(running->var_dict, _do_nothing_on_object, _retrieve_var, &tuple);

_exit:
	return result;
}

/* Add a variable with a specific name */
int mb_add_var(struct mb_interpreter_t* s, void** l, const char* n, mb_value_t val, bool_t force) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	_object_t* obj = 0;
	_var_t* var = 0;
	_ls_node_t* tmp = 0;

	if(!s || !n) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;

	tmp = _ht_find(running->var_dict, (void*)n);

	if(tmp) {
		if(force) {
			result = mb_set_var_value(s, tmp->data, val);

			goto _exit;
		} else {
			_handle_error_on_obj(s, SE_RN_DUPLICATE_ID, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
	}

	var = _create_var(&obj, n, strlen(n) + 1, true);
	_public_value_to_internal_object(&val, var->data);

	_ht_set_or_insert(running->var_dict, var->name, obj);

_exit:
	return result;
}

/* Get a token literally, store it in an argument if it's a variable */
int mb_get_var(struct mb_interpreter_t* s, void** l, void** v, bool_t redir) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(v) *v = 0;

	ast = (_ls_node_t*)*l;
	if(ast) {
		obj = (_object_t*)ast->data;
		if(_IS_SEP(obj, ',')) {
			ast = ast->next;
			obj = ast ? (_object_t*)ast->data : 0;
		}
		if(ast) ast = ast->next;
	}

	if(_IS_VAR(obj)) {
#ifdef MB_ENABLE_CLASS
		if(redir && obj->data.variable->pathing)
			_search_var_in_scope_chain(s, obj->data.variable, &obj);
#else /* MB_ENABLE_CLASS */
		mb_unrefvar(redir);
#endif /* MB_ENABLE_CLASS */
		if(v)
			*v = obj;
	}

	if(ast && _IS_SEP(ast->data, ','))
		ast = ast->next;

	*l = ast;

_exit:
	return result;
}

/* Get the name of a variable */
int mb_get_var_name(struct mb_interpreter_t* s, void* v, char** n) {
	int result = MB_FUNC_OK;
	_object_t* obj = 0;

	if(n) *n = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(!n || !v)
		goto _exit;

	obj = (_object_t*)v;
	if(!_IS_VAR(obj))
		goto _exit;

	if(n) *n = obj->data.variable->name;

_exit:
	return result;
}

/* Get the value of a variable */
int mb_get_var_value(struct mb_interpreter_t* s, void* v, mb_value_t* val) {
	int result = MB_FUNC_OK;
	_object_t* obj = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(!val || !v)
		goto _exit;

	obj = (_object_t*)v;
	if(!_IS_VAR(obj))
		goto _exit;

	_internal_object_to_public_value(obj->data.variable->data, val);

_exit:
	return result;
}

/* Set the value of a variable */
int mb_set_var_value(struct mb_interpreter_t* s, void* v, mb_value_t val) {
	int result = MB_FUNC_OK;
	_object_t* obj = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(!v)
		goto _exit;
	obj = (_object_t*)v;
	if(!_IS_VAR(obj))
		goto _exit;

	_public_value_to_internal_object(&val, obj->data.variable->data);

_exit:
	return result;
}

/* Create an array */
int mb_init_array(struct mb_interpreter_t* s, void** l, mb_data_e t, int* d, int c, void** a) {
	int result = MB_FUNC_OK;
	_array_t* arr = 0;
	_data_e type = _DT_NIL;
	int j = 0;
	int n = 0;

	if(!s || !l || !d || !a) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	*a = 0;
	if(c > MB_MAX_DIMENSION_COUNT) {
		_handle_error_on_obj(s, SE_RN_TOO_MANY_DIMENSIONS, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
	for(j = 0; j < c; j++) {
		n = d[j];
		if(n <= 0) {
			_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
	}

#ifdef MB_SIMPLE_ARRAY
	if(t == MB_DT_REAL) {
		type = _DT_REAL;
	} else if(t == MB_DT_STRING) {
		type = _DT_STRING;
	} else {
		_handle_error_on_obj(s, SE_RN_UNEXPECTED_TYPE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
#else /* MB_SIMPLE_ARRAY */
	mb_unrefvar(t);
	type = _DT_REAL;
#endif /* MB_SIMPLE_ARRAY */

	arr = _create_array(s, 0, type);
	for(j = 0; j < c; j++) {
		n = d[j];
		arr->dimensions[arr->dimension_count++] = n;
		if(arr->count)
			arr->count *= (unsigned)n;
		else
			arr->count += (unsigned)n;
	}
	_init_array(arr);
	if(!arr->raw) {
		arr->dimension_count = 0;
		arr->dimensions[0] = 0;
		arr->count = 0;
	}
	*a = arr;

_exit:
	return result;
}

/* Get the length of an array */
int mb_get_array_len(struct mb_interpreter_t* s, void** l, void* a, int r, int* i) {
	int result = 0;
	_array_t* arr = 0;

	if(!s || !l)
		goto _exit;

	arr = (_array_t*)a;
	if(r < 0 || r >= arr->dimension_count) {
		_handle_error_on_obj(s, SE_RN_RANK_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
	if(i)
		*i = arr->dimensions[r];

_exit:
	return result;
}

/* Get an element of an array with a specific index */
int mb_get_array_elem(struct mb_interpreter_t* s, void** l, void* a, int* d, int c, mb_value_t* val) {
	int result = MB_FUNC_OK;
	_array_t* arr = 0;
	int index = 0;
	_data_e type = _DT_NIL;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	arr = (_array_t*)a;
	if(c < 0 || c > arr->dimension_count) {
		_handle_error_on_obj(s, SE_RN_TOO_MANY_DIMENSIONS, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
	if(!val)
		goto _exit;

	index = _get_array_pos(s, arr, d, c);
	if(index < 0) {
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	_get_array_elem(s, arr, index, &val->value, &type);
	val->type = _internal_type_to_public_type(type);

_exit:
	return result;
}

/* Set an element of an array with a specific index */
int mb_set_array_elem(struct mb_interpreter_t* s, void** l, void* a, int* d, int c, mb_value_t val) {
	int result = MB_FUNC_OK;
	_array_t* arr = 0;
	int index = 0;
	_data_e type = _DT_NIL;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	arr = (_array_t*)a;
	if(c < 0 || c > arr->dimension_count) {
		_handle_error_on_obj(s, SE_RN_TOO_MANY_DIMENSIONS, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	index = _get_array_pos(s, arr, d, c);
	if(index < 0) {
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	type = _public_type_to_internal_type(val.type);
	_set_array_elem(s, 0, arr, (unsigned)index, &val.value, &type);

_exit:
	return result;
}

/* Create a referenced usertype value */
int mb_make_ref_value(struct mb_interpreter_t* s, void* val, mb_value_t* out, mb_dtor_func_t un, mb_clone_func_t cl, mb_hash_func_t hs, mb_cmp_func_t cp, mb_fmt_func_t ft) {
#ifdef MB_ENABLE_USERTYPE_REF
	int result = MB_FUNC_OK;
	_usertype_ref_t* ref = 0;

	if(!s || !out) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(out) {
		ref = _create_usertype_ref(s, val, un, cl, hs, cp, ft);
		out->type = MB_DT_USERTYPE_REF;
		out->value.usertype_ref = ref;
	}

_exit:
	return result;
#else /* MB_ENABLE_USERTYPE_REF */
	mb_unrefvar(s);
	mb_unrefvar(val);
	mb_unrefvar(out);
	mb_unrefvar(un);
	mb_unrefvar(cl);
	mb_unrefvar(hs);
	mb_unrefvar(cp);
	mb_unrefvar(ft);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_USERTYPE_REF */
}

/* Get the data of a referenced usertype value */
int mb_get_ref_value(struct mb_interpreter_t* s, void** l, mb_value_t val, void** out) {
#ifdef MB_ENABLE_USERTYPE_REF
	int result = MB_FUNC_OK;
	_usertype_ref_t* ref = 0;

	if(!s || !out) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(val.type != MB_DT_USERTYPE_REF) {
		_handle_error_on_obj(s, SE_RN_UNEXPECTED_TYPE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	if(out) {
		ref = (_usertype_ref_t*)val.value.usertype_ref;
		*out = ref->usertype;
	}

_exit:
	return result;
#else /* MB_ENABLE_USERTYPE_REF */
	mb_unrefvar(s);
	mb_unrefvar(l);
	mb_unrefvar(val);
	mb_unrefvar(out);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_USERTYPE_REF */
}

/* Increase the reference of a value by 1 */
int mb_ref_value(struct mb_interpreter_t* s, void** l, mb_value_t val) {
	int result = MB_FUNC_OK;
	_object_t obj;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

#ifdef MB_ENABLE_COLLECTION_LIB
	if(val.type == MB_DT_LIST_IT || val.type == MB_DT_DICT_IT)
		goto _exit;
#endif /* MB_ENABLE_COLLECTION_LIB */

	_MAKE_NIL(&obj);
	_public_value_to_internal_object(&val, &obj);
	if(
#ifdef MB_ENABLE_USERTYPE_REF
		obj.type != _DT_USERTYPE_REF &&
#endif /* MB_ENABLE_USERTYPE_REF */
		obj.type != _DT_ARRAY &&
#ifdef MB_ENABLE_COLLECTION_LIB
		obj.type != _DT_LIST && obj.type != _DT_DICT &&
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
		obj.type != _DT_CLASS &&
#endif /* MB_ENABLE_CLASS */
		obj.type != _DT_ROUTINE
	) {
		_handle_error_on_obj(s, SE_RN_REFERENCED_TYPE_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
	_REF(&obj)

_exit:
	return result;
}

/* Decrease the reference of a value by 1 */
int mb_unref_value(struct mb_interpreter_t* s, void** l, mb_value_t val) {
	int result = MB_FUNC_OK;
	_object_t obj;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

#ifdef MB_ENABLE_COLLECTION_LIB
	if(_try_purge_it(s, &val, 0))
		goto _exit;
#endif /* MB_ENABLE_COLLECTION_LIB */

	_MAKE_NIL(&obj);
	_public_value_to_internal_object(&val, &obj);
	if(
#ifdef MB_ENABLE_USERTYPE_REF
		obj.type != _DT_USERTYPE_REF &&
#endif /* MB_ENABLE_USERTYPE_REF */
		obj.type != _DT_ARRAY &&
#ifdef MB_ENABLE_COLLECTION_LIB
		obj.type != _DT_LIST && obj.type != _DT_DICT &&
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
		obj.type != _DT_CLASS &&
#endif /* MB_ENABLE_CLASS */
		obj.type != _DT_ROUTINE
	) {
		_handle_error_on_obj(s, SE_RN_REFERENCED_TYPE_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
	_UNREF(&obj)

_exit:
	return result;
}

/* Set the global alive checker */
int mb_set_alive_checker(struct mb_interpreter_t* s, mb_alive_checker_t f) {
	int result = MB_FUNC_OK;

	if(!s)
		result = MB_FUNC_ERR;
	else
		s->alive_check_handler = f;

	return result;
}

/* Set the alive checker of a value */
int mb_set_alive_checker_of_value(struct mb_interpreter_t* s, void** l, mb_value_t val, mb_alive_value_checker_t f) {
#ifdef MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF
	int result = MB_FUNC_OK;
	_object_t obj;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(val.type != MB_DT_USERTYPE_REF) {
		_handle_error_on_obj(s, SE_RN_REFERENCED_TYPE_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	_MAKE_NIL(&obj);
	_public_value_to_internal_object(&val, &obj);
	obj.data.usertype_ref->alive_checker = f;

_exit:
	return result;
#else /* MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF */
	mb_unrefvar(s);
	mb_unrefvar(l);
	mb_unrefvar(val);
	mb_unrefvar(f);

	return MB_FUNC_ERR;
#endif /* MB_ENABLE_ALIVE_CHECKING_ON_USERTYPE_REF */
}

/* Override a meta function of a value */
int mb_override_value(struct mb_interpreter_t* s, void** l, mb_value_t val, mb_meta_func_e m, void* f) {
	int result = MB_FUNC_OK;
	_object_t obj;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

#ifdef MB_ENABLE_USERTYPE_REF
	_MAKE_NIL(&obj);
	if(val.type == MB_DT_USERTYPE_REF) {
		_usertype_ref_t* user = 0;
		_public_value_to_internal_object(&val, &obj);
		user = obj.data.usertype_ref;
		if(m & MB_MF_CALC) {
			if(!user->calc_operators) {
				user->calc_operators = (_calculation_operator_info_t*)mb_malloc(sizeof(_calculation_operator_info_t));
				memset(user->calc_operators, 0, sizeof(_calculation_operator_info_t));
			}
		}
		switch(m) {
		case MB_MF_IS:
			user->calc_operators->is = (mb_meta_operator_t)(intptr_t)f;

			break;
		case MB_MF_ADD:
			user->calc_operators->add = (mb_meta_operator_t)(intptr_t)f;

			break;
		case MB_MF_SUB:
			user->calc_operators->sub = (mb_meta_operator_t)(intptr_t)f;

			break;
		case MB_MF_MUL:
			user->calc_operators->mul = (mb_meta_operator_t)(intptr_t)f;

			break;
		case MB_MF_DIV:
			user->calc_operators->div = (mb_meta_operator_t)(intptr_t)f;

			break;
		case MB_MF_NEG:
			user->calc_operators->neg = (mb_meta_operator_t)(intptr_t)f;

			break;
		case MB_MF_COLL:
			user->coll_func = (mb_meta_func_t)(intptr_t)f;

			break;
		case MB_MF_FUNC:
			user->generic_func = (mb_meta_func_t)(intptr_t)f;

			break;
		default: /* Do nothing */
			break;
		}
	} else {
		result = MB_FUNC_ERR;
	}
#else /* MB_ENABLE_USERTYPE_REF */
	mb_unrefvar(obj);
	mb_unrefvar(val);
	mb_unrefvar(m);
	mb_unrefvar(f);
#endif /* MB_ENABLE_USERTYPE_REF */

_exit:
	return result;
}

/* Dispose a value */
int mb_dispose_value(struct mb_interpreter_t* s, mb_value_t val) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(val.type == MB_DT_STRING) {
		safe_free(val.value.string);
	}

	_assign_public_value(s, &val, 0, false);

_exit:
	return result;
}

/* Get a sub routine with a specific name */
int mb_get_routine(struct mb_interpreter_t* s, void** l, const char* n, mb_value_t* val) {
	int result = MB_FUNC_OK;
	_object_t* obj = 0;
	_ls_node_t* scp = 0;

	if(!s || !n || !val) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	mb_make_nil(*val);

	scp = _search_identifier_in_scope_chain(s, 0, n, _PATHING_NONE, 0, 0);
	if(scp) {
		obj = (_object_t*)scp->data;
		if(obj) {
			if(obj->type == _DT_ROUTINE) {
				_internal_object_to_public_value(obj, val);
			} else {
				_handle_error_on_obj(s, SE_RN_ROUTINE_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}
		}
	}

_exit:
	return result;
}

/* Set a sub routine with a specific name and native function pointer */
int mb_set_routine(struct mb_interpreter_t* s, void** l, const char* n, mb_routine_func_t f, bool_t force) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	_object_t* obj = 0;
	_routine_t* routine = 0;
	_ls_node_t* tmp = 0;
	_var_t* var = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;

	tmp = _ht_find(running->var_dict, (void*)n);

	if(tmp) {
		if(force) {
			obj = (_object_t*)tmp->data;
			if(_IS_VAR(obj))
				var = obj->data.variable;
		} else {
			_handle_error_on_obj(s, SE_RN_DUPLICATE_ROUTINE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}
	}

	routine = (_routine_t*)mb_malloc(sizeof(_routine_t));
	_init_routine(s, routine, mb_strdup(n, strlen(n) + 1), f);

	obj = _create_object();
	obj->type = _DT_ROUTINE;
	obj->data.routine = routine;
	obj->is_ref = false;

#ifdef MB_ENABLE_CLASS
	routine->instance = s->last_instance;
#endif /* MB_ENABLE_CLASS */

	if(var && force) {
		_destroy_object(var->data, 0);
		var->data = obj;
	} else {
		_ht_set_or_insert(running->var_dict, routine->name, obj);
	}

_exit:
	return result;
}

/* Evaluate a sub routine */
int mb_eval_routine(struct mb_interpreter_t* s, void** l, mb_value_t val, mb_value_t* args, unsigned argc, mb_value_t* ret) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	_object_t obj;
	_ls_node_t* ast = 0;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;

	if(val.type != MB_DT_ROUTINE) {
		_handle_error_on_obj(s, SE_RN_ROUTINE_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

	_MAKE_NIL(&obj);
	_public_value_to_internal_object(&val, &obj);
	ast = (_ls_node_t*)(*l);
	result = _eval_routine(s, &ast, args, argc, obj.data.routine, _has_routine_fun_arg, _pop_routine_fun_arg);

	if(ret) {
		_assign_public_value(s, ret, &running->intermediate_value, false);
		_MAKE_NIL(&obj);
		_public_value_to_internal_object(ret, &obj);
		_ADDGC(&obj, &s->gc, false)
	}

_exit:
	return result;
}

/* Get the type of a routine */
int mb_get_routine_type(struct mb_interpreter_t* s, mb_value_t val, mb_routine_type_e* y) {
	int result = MB_FUNC_OK;
	_object_t obj;
	mb_unrefvar(s);

	if(!y) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(val.type != MB_DT_ROUTINE) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	_MAKE_NIL(&obj);
	_public_value_to_internal_object(&val, &obj);
	*y = obj.data.routine->type;

_exit:
	return result;
}

/* Load and parse a script string */
int mb_load_string(struct mb_interpreter_t* s, const char* l, bool_t reset) {
	int result = MB_FUNC_OK;
	unsigned short _row = 0;
	unsigned short _col = 0;
	char wrapped = _ZERO_CHAR;
	_parsing_context_t* context = 0;

	if(!s || !l) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	s->run_count = 0;
	s->last_error = SE_NO_ERR;
	s->last_error_file = 0;

	if(!s->parsing_context)
		s->parsing_context = _reset_parsing_context(s->parsing_context);

	context = s->parsing_context;

	mb_uu_getbom(&l);
	while(*l) {
		int n = 1;
#ifdef MB_ENABLE_UNICODE_ID
		if(context->parsing_state == _PS_NORMAL)
			n = mb_uu_ischar(l);
#endif /* MB_ENABLE_UNICODE_ID */
		do {
			if(n == 1) {
				char ch = *l;
				if((ch == _NEWLINE_CHAR || ch == _RETURN_CHAR) && (!wrapped || wrapped == ch)) {
					unsigned short before = 0;
					wrapped = ch;
					before = context->parsing_row++;
					context->parsing_col = 0;
					if(before > context->parsing_row) {
						context->parsing_col = 1;
						_handle_error_now(s, SE_RN_PROGRAM_TOO_LONG, s->last_error_file, MB_FUNC_ERR);

						goto _exit;
					}

					break;
				}
			}
			wrapped = _ZERO_CHAR;
			++context->parsing_col;
		} while(0);
		result = _parse_char(s, l, n, context->parsing_pos, _row, _col);
		if(result != MB_FUNC_OK) {
			_set_error_pos(s, context->parsing_pos, _row, _col);
			_handle_error_now(s, s->last_error, s->last_error_file, result);

			goto _exit;
		}
		_row = context->parsing_row;
		_col = context->parsing_col;
		++context->parsing_pos;
		l += n;
	};
	result = _parse_char(s, 0, 1, context->parsing_pos, context->parsing_row, context->parsing_col);

_exit:
	if(reset)
		_end_of_file(context);

	return result;
}

/* Load and parse a script file */
int mb_load_file(struct mb_interpreter_t* s, const char* f) {
	int result = MB_FUNC_OK;
	char* buf = 0;
	_parsing_context_t* context = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	s->parsing_context = context = _reset_parsing_context(s->parsing_context);

	buf = _load_file(s, f, 0, false);
	if(buf) {
		result = mb_load_string(s, buf, true);
		safe_free(buf);

		if(result)
			goto _exit;
	} else {
		_set_current_error(s, SE_PS_FAILED_TO_OPEN_FILE, 0);

		result = MB_FUNC_ERR;
	}

_exit:
	if(context)
		context->parsing_state = _PS_NORMAL;

	return result;
}

/* Run the current AST */
int mb_run(struct mb_interpreter_t* s, bool_t clear_parser) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	++s->run_count;

	if(s->parsing_context) {
		if(s->parsing_context->routine_state) {
			s->parsing_context->routine_state = 0;
			result = MB_FUNC_ERR;
			_handle_error_now(s, SE_RN_INCOMPLETE_STRUCTURE, s->source_file, result);
			_tidy_scope_chain(s);

			goto _exit;
		}
#ifdef MB_ENABLE_CLASS
		if(s->parsing_context->class_state != _CLASS_STATE_NONE) {
			s->parsing_context->class_state = _CLASS_STATE_NONE;
			result = MB_FUNC_ERR;
			_handle_error_now(s, SE_RN_INCOMPLETE_STRUCTURE, s->source_file, result);
			_tidy_scope_chain(s);

			goto _exit;
		}
#endif /* MB_ENABLE_CLASS */
	}

	if(clear_parser)
		_destroy_parsing_context(&s->parsing_context);

	s->handled_error = false;

	if(s->suspent_point) {
		ast = s->suspent_point;
		ast = ast->next;
		s->suspent_point = 0;
	} else {
		s->source_file = 0;
#ifdef MB_ENABLE_CLASS
		s->last_instance = 0;
		s->calling = false;
#endif /* MB_ENABLE_CLASS */
		s->last_routine = 0;
		s->last_error = SE_NO_ERR;
		s->last_error_file = 0;

#if _MULTILINE_STATEMENT
		_ls_clear(s->multiline_enabled);
#endif /* _MULTILINE_STATEMENT */

		mb_assert(!s->no_eat_comma_mark);
		while(s->running_context->prev)
			s->running_context = s->running_context->prev;
		ast = s->ast;
		ast = ast->next;
		if(!ast) {
			result = MB_FUNC_ERR;
			_set_error_pos(s, 0, 0, 0);
			_handle_error_now(s, SE_RN_EMPTY_PROGRAM, s->source_file, result);

			goto _exit;
		}
	}

	do {
		_ls_node_t* p = ast;
		result = _execute_statement(s, &ast, true);
		if(ast == p) {
			_handle_error_now(s, SE_RN_INVALID_EXPRESSION, s->last_error_file, result);

			goto _exit;
		}
		if(result != MB_FUNC_OK && result != MB_SUB_RETURN) {
			if(result != MB_FUNC_SUSPEND) {
				if(result >= MB_EXTENDED_ABORT)
					s->last_error = SE_EA_EXTENDED_ABORT;
				_handle_error_now(s, s->last_error, s->last_error_file, result);
			}

			goto _exit;
		}
	} while(ast);

_exit:
	if(s) {
		if(!s->suspent_point)
			s->source_file = 0;
		if(clear_parser)
			_destroy_parsing_context(&s->parsing_context);

		_destroy_edge_objects(s);

		s->has_run = true;
	}

	return result;
}

/* Suspend current execution and save the context */
int mb_suspend(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;

	if(!s || !l || !(*l)) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	ast = (_ls_node_t*)*l;
	s->suspent_point = ast;

_exit:
	return result;
}

/* Schedule to suspend current execution */
int mb_schedule_suspend(struct mb_interpreter_t* s, int t) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(t == MB_FUNC_OK)
		t = MB_FUNC_SUSPEND;
	s->schedule_suspend_tag = t;

_exit:
	return result;
}

/* Get the value of an identifier */
int mb_debug_get(struct mb_interpreter_t* s, const char* n, mb_value_t* val) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	_ls_node_t* v = 0;
	_object_t* obj = 0;
	mb_value_t tmp;

	if(!s || !n) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;

	v = _search_identifier_in_scope_chain(s, 0, n, _PATHING_NONE, 0, 0);
	if(v) {
		obj = (_object_t*)v->data;
		mb_assert(_IS_VAR(obj));
		if(val)
			result = _internal_object_to_public_value(obj->data.variable->data, val);
		else
			result = _internal_object_to_public_value(obj->data.variable->data, &tmp);
	} else {
		if(val) {
			mb_make_nil(*val);
		}
		_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, (_object_t*)0, MB_FUNC_ERR, _exit, result);
	}

_exit:
	return result;
}

/* Set the value of an identifier */
int mb_debug_set(struct mb_interpreter_t* s, const char* n, mb_value_t val) {
	int result = MB_FUNC_OK;
	_running_context_t* running = 0;
	_ls_node_t* v = 0;
	_object_t* obj = 0;

	if(!s || !n) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	running = s->running_context;

	v = _search_identifier_in_scope_chain(s, 0, n, _PATHING_NONE, 0, 0);
	if(v) {
		obj = (_object_t*)v->data;
		mb_assert(_IS_VAR(obj));
		result = _public_value_to_internal_object(&val, obj->data.variable->data);
	} else {
		_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, (_object_t*)0, MB_FUNC_ERR, _exit, result);
	}

_exit:
	return result;
}

/* Get stack frame count of a MY-BASIC environment */
int mb_debug_count_stack_frames(struct mb_interpreter_t* s) {
#ifdef MB_ENABLE_STACK_TRACE
	int result = 0;

	if(!s) {
		goto _exit;
	}

	result = _ls_count(s->stack_frames);

_exit:
	return result;
#else /* MB_ENABLE_STACK_TRACE */
	int result = 0;
	mb_unrefvar(s);
	mb_unrefvar(l);

	return result;
#endif /* MB_ENABLE_STACK_TRACE */
}

/* Get stack frame names of a MY-BASIC environment */
int mb_debug_get_stack_trace(struct mb_interpreter_t* s, char** fs, unsigned fc) {
#ifdef MB_ENABLE_STACK_TRACE
	int result = MB_FUNC_OK;
	_ls_node_t* f = 0;
	unsigned i = 0;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(fs && fc) {
		if(_ls_count(s->stack_frames) > fc)
			fs[--fc] = "...";
		f = s->stack_frames->prev;
		while(f != s->stack_frames && f && f->data && i < fc) {
			fs[i++] = (char*)f->data;
			f = f->prev;
		}
	}
	while(i < fc)
		fs[i++] = 0;

_exit:
	return result;
#else /* MB_ENABLE_STACK_TRACE */
	int result = MB_FUNC_ERR;
	mb_unrefvar(s);
	mb_unrefvar(fs);
	mb_unrefvar(fc);

	return result;
#endif /* MB_ENABLE_STACK_TRACE */
}

/* Set a stepped handler to a MY-BASIC environment */
int mb_debug_set_stepped_handler(struct mb_interpreter_t* s, mb_debug_stepped_handler_t prev, mb_debug_stepped_handler_t post) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	s->debug_prev_stepped_handler = prev;
	s->debug_post_stepped_handler = post;

_exit:
	return result;
}

/* Get type description text */
const char* mb_get_type_string(mb_data_e t) {
	switch(t) {
	case MB_DT_NIL:
		return "NIL";
	case MB_DT_UNKNOWN:
		return "UNKNOWN";
	case MB_DT_INT:
		return "INTEGER";
	case MB_DT_REAL:
		return "REAL";
	case MB_DT_NUM:
		return "NUMBER";
	case MB_DT_STRING:
		return "STRING";
	case MB_DT_TYPE:
		return "TYPE";
	case MB_DT_USERTYPE:
		return "USERTYPE";
#ifdef MB_ENABLE_USERTYPE_REF
	case MB_DT_USERTYPE_REF:
		return "USERTYPE_REF";
#endif /* MB_ENABLE_USERTYPE_REF */
	case MB_DT_ARRAY:
		return "ARRAY";
#ifdef MB_ENABLE_COLLECTION_LIB
	case MB_DT_LIST:
		return "LIST";
	case MB_DT_LIST_IT:
		return "LIST_ITERATOR";
	case MB_DT_DICT:
		return "DICT";
	case MB_DT_DICT_IT:
		return "DICT_ITERATOR";
	case MB_DT_COLLECTION:
		return "COLLECTION";
	case MB_DT_ITERATOR:
		return "ITERATOR";
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	case MB_DT_CLASS:
		return "CLASS";
#endif /* MB_ENABLE_CLASS */
	case MB_DT_ROUTINE:
		return "ROUTINE";
	default: /* Return a not existing string */
		return "";
	}
}

/* Raise an error */
int mb_raise_error(struct mb_interpreter_t* s, void** l, mb_error_e err, int ret) {
	int result = MB_FUNC_ERR;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	_handle_error_on_obj(s, err, s->source_file, DON2(l), ret, _exit, result);

_exit:
	return result;
}

/* Get the last error information */
mb_error_e mb_get_last_error(struct mb_interpreter_t* s, const char** file, int* pos, unsigned short* row, unsigned short* col) {
	mb_error_e result = SE_NO_ERR;

	if(!s)
		goto _exit;

	result = s->last_error;
	s->last_error = SE_NO_ERR; /* Clear error state */
	if(file) *file = s->last_error_file;
	if(pos) *pos = s->last_error_pos;
	if(row) *row = s->last_error_row;
	if(col) *col = s->last_error_col;
	s->last_error_file = 0;

_exit:
	return result;
}

/* Get the error description text */
const char* mb_get_error_desc(mb_error_e err) {
#ifdef MB_ENABLE_FULL_ERROR
	if(err < countof(_ERR_DESC))
		return _ERR_DESC[err];

	return "Unknown error";
#else /* MB_ENABLE_FULL_ERROR */
	mb_unrefvar(err);

	return "Error occurred";
#endif /* MB_ENABLE_FULL_ERROR */
}

/* Set an error handler to a MY-BASIC environment */
int mb_set_error_handler(struct mb_interpreter_t* s, mb_error_handler_t h) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	s->error_handler = h;

_exit:
	return result;
}

/* Set a print functor to a MY-BASIC environment */
int mb_set_printer(struct mb_interpreter_t* s, mb_print_func_t p) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	s->printer = p;

_exit:
	return result;
}

/* Set an input functor to a MY-BASIC environment */
int mb_set_inputer(struct mb_interpreter_t* s, mb_input_func_t p) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	s->inputer = p;

_exit:
	return result;
}

/* Set an import handler to a MY-BASIC environment */
int mb_set_import_handler(struct mb_interpreter_t* s, mb_import_handler_t h) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	s->import_handler = h;

_exit:
	return result;
}

/* Register an allocator and a freer globally */
int mb_set_memory_manager(mb_memory_allocate_func_t a, mb_memory_free_func_t f) {
	_mb_allocate_func = a;
	_mb_free_func = f;

	return MB_FUNC_OK;
}

/* Get whether GC is enabled */
bool_t mb_get_gc_enabled(struct mb_interpreter_t* s) {
	if(!s) return false;

	return !s->gc.disabled;
}

/* Sets whether GC is enabled */
int mb_set_gc_enabled(struct mb_interpreter_t* s, bool_t gc) {
	if(!s) return MB_FUNC_ERR;

	s->gc.disabled = !gc;

	return MB_FUNC_OK;
}

/* Trigger GC */
int mb_gc(struct mb_interpreter_t* s, int_t* collected) {
	int_t diff = 0;

	if(!s)
		return MB_FUNC_ERR;

	diff = (int_t)_mb_allocated;
	_gc_collect_garbage(s, 1);
	diff = (int_t)(_mb_allocated - diff);
	if(collected)
		*collected = diff;

	return MB_FUNC_OK;
}

/* Get the userdata of a MY-BASIC environment */
int mb_get_userdata(struct mb_interpreter_t* s, void** d) {
	int result = MB_FUNC_OK;

	if(!s || !d) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(s && d)
		*d = s->userdata;

_exit:
	return result;
}

/* Set the userdata of a MY-BASIC environment */
int mb_set_userdata(struct mb_interpreter_t* s, void* d) {
	int result = MB_FUNC_OK;

	if(!s) {
		result = MB_FUNC_ERR;

		goto _exit;
	}

	if(s)
		s->userdata = d;

_exit:
	return result;
}

/* Safe stdin reader function */
int mb_gets(struct mb_interpreter_t* s, const char* pmt, char* buf, int n) {
	int result = 0;
	mb_unrefvar(s);
	mb_unrefvar(pmt);

	if(buf && n) {
		if(fgets(buf, n, stdin) == 0) {
			fprintf(stderr, "Error reading.\n");

			exit(1);
		}
		result = (int)strlen(buf);
		if(buf[result - 1] == _NEWLINE_CHAR) {
			buf[result - 1] = _ZERO_CHAR;
			result--;
		}
	}

	return result;
}

/* Duplicate a string for internal use */
char* mb_memdup(const char* val, unsigned size) {
	char* result = 0;

	if(val != 0) {
		result = (char*)mb_malloc(size);
		if(result)
			memcpy(result, val, size);
	}

	return result;
}

/* ========================================================} */

/*
** {========================================================
** Lib definitions
*/

/* ========================================================} */
