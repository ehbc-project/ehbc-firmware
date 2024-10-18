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

#include "mb_lib_std.h"

#include "mb_lib_core.h"
#include "mb_lib_coll.h"

/** Standard lib */

static int _std_abs(mb_interpreter_t* s, void** l);
static int _std_sgn(mb_interpreter_t* s, void** l);
static int _std_sqr(mb_interpreter_t* s, void** l);
static int _std_floor(mb_interpreter_t* s, void** l);
static int _std_ceil(mb_interpreter_t* s, void** l);
static int _std_fix(mb_interpreter_t* s, void** l);
static int _std_round(mb_interpreter_t* s, void** l);
static int _std_srnd(mb_interpreter_t* s, void** l);
static int _std_rnd(mb_interpreter_t* s, void** l);
static int _std_sin(mb_interpreter_t* s, void** l);
static int _std_cos(mb_interpreter_t* s, void** l);
static int _std_tan(mb_interpreter_t* s, void** l);
static int _std_asin(mb_interpreter_t* s, void** l);
static int _std_acos(mb_interpreter_t* s, void** l);
static int _std_atan(mb_interpreter_t* s, void** l);
static int _std_exp(mb_interpreter_t* s, void** l);
static int _std_log(mb_interpreter_t* s, void** l);
static int _std_asc(mb_interpreter_t* s, void** l);
static int _std_chr(mb_interpreter_t* s, void** l);
static int _std_left(mb_interpreter_t* s, void** l);
static int _std_mid(mb_interpreter_t* s, void** l);
static int _std_right(mb_interpreter_t* s, void** l);
static int _std_str(mb_interpreter_t* s, void** l);
static int _std_val(mb_interpreter_t* s, void** l);
static int _std_len(mb_interpreter_t* s, void** l);
static int _std_get(mb_interpreter_t* s, void** l);
static int _std_set(mb_interpreter_t* s, void** l);
static int _std_print(mb_interpreter_t* s, void** l);
static int _std_input(mb_interpreter_t* s, void** l);

MBCONST static const _func_t _std_libs[] = {
	{ _STD_ID_ABS,   _std_abs },
	{ _STD_ID_SGN,   _std_sgn },
	{ _STD_ID_SQR,   _std_sqr },
	{ _STD_ID_FLOOR, _std_floor },
	{ _STD_ID_CEIL,  _std_ceil },
	{ _STD_ID_FIX,   _std_fix },
	{ _STD_ID_ROUND, _std_round },
	{ _STD_ID_SRND,  _std_srnd },
	{ _STD_ID_RND,   _std_rnd },
	{ _STD_ID_SIN,   _std_sin },
	{ _STD_ID_COS,   _std_cos },
	{ _STD_ID_TAN,   _std_tan },
	{ _STD_ID_ASIN,  _std_asin },
	{ _STD_ID_ACOS,  _std_acos },
	{ _STD_ID_ATAN,  _std_atan },
	{ _STD_ID_EXP,   _std_exp },
	{ _STD_ID_LOG,   _std_log },

	{ _STD_ID_ASC,   _std_asc },
	{ _STD_ID_CHR,   _std_chr },
	{ _STD_ID_LEFT,  _std_left },
	{ _STD_ID_MID,   _std_mid },
	{ _STD_ID_RIGHT, _std_right },
	{ _STD_ID_STR,   _std_str },
	{ _STD_ID_VAL,   _std_val },

	{ _STD_ID_LEN,   _std_len },
	{ _STD_ID_GET,   _std_get },
	{ _STD_ID_SET,   _std_set },

	{ _STD_ID_PRINT, _std_print },
	{ _STD_ID_INPUT, _std_input }
};

_MB_DEFINE_LIBRARY(std)

/* Get the absolute value of a number */
static int _std_abs(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	switch(arg.type) {
	case MB_DT_INT:
		arg.value.integer = (int_t)abs(arg.value.integer);

		break;
	case MB_DT_REAL:
		arg.value.float_point = (real_t)fabs(arg.value.float_point);

		break;
	default:
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);

		break;
	}

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the sign of a number */
static int _std_sgn(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	switch(arg.type) {
	case MB_DT_INT:
		arg.value.integer = sgn(arg.value.integer);

		break;
	case MB_DT_REAL:
		arg.value.integer = sgn(arg.value.float_point);
		arg.type = MB_DT_INT;

		break;
	default:
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);

		break;
	}
	_mb_check_mark_exit(mb_push_int(s, l, arg.value.integer), result, _exit);

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* Get the square root of a number */
static int _std_sqr(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, sqrt, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the greatest integer not greater than a number */
static int _std_floor(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	switch(arg.type) {
	case MB_DT_INT: /* Do nothing */
		break;
	case MB_DT_REAL:
		arg.value.integer = (int_t)floor(arg.value.float_point);
		arg.type = MB_DT_INT;

		break;
	default:
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);

		break;
	}
	_mb_check_mark_exit(mb_push_int(s, l, arg.value.integer), result, _exit);

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* Get the least integer not less than a number */
static int _std_ceil(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	switch(arg.type) {
	case MB_DT_INT: /* Do nothing */
		break;
	case MB_DT_REAL:
		arg.value.integer = (int_t)ceil(arg.value.float_point);
		arg.type = MB_DT_INT;

		break;
	default:
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);

		break;
	}
	_mb_check_mark_exit(mb_push_int(s, l, arg.value.integer), result, _exit);

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* Get the integer format of a number */
static int _std_fix(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	switch(arg.type) {
	case MB_DT_INT: /* Do nothing */
		break;
	case MB_DT_REAL:
		arg.value.integer = (int_t)(arg.value.float_point);
		arg.type = MB_DT_INT;

		break;
	default:
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);

		break;
	}
	_mb_check_mark_exit(mb_push_int(s, l, arg.value.integer), result, _exit);

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* Get the rounded integer of a number */
static int _std_round(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	switch(arg.type) {
	case MB_DT_INT: /* Do nothing */
		break;
	case MB_DT_REAL:
		arg.value.integer = (int_t)(arg.value.float_point + (real_t)0.5f);
		arg.type = MB_DT_INT;

		break;
	default:
		_handle_error_on_obj(s, SE_RN_NUMBER_EXPECTED, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);

		break;
	}
	_mb_check_mark_exit(mb_push_int(s, l, arg.value.integer), result, _exit);

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* Set a random seed */
static int _std_srnd(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	int_t seed = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_int(s, l, &seed));

	mb_check(mb_attempt_close_bracket(s, l));

	srand((unsigned)seed);

	return result;
}

/* Get a random value among 0 ~ 1 or among given bounds */
static int _std_rnd(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	real_t rnd = (real_t)0.0f;

	mb_assert(s && l);

	ast = (_ls_node_t*)*l;

	if(ast && ast->next && _IS_FUNC(ast->next->data, _core_open_bracket)) {
		int_t lw = 0;
		int_t hg = 0;

		mb_check(mb_attempt_open_bracket(s, l));

		if(mb_has_arg(s, l)) {
			mb_check(mb_pop_int(s, l, &hg));
		}
		if(mb_has_arg(s, l)) {
			lw = hg;
			mb_check(mb_pop_int(s, l, &hg));
		}

		mb_check(mb_attempt_close_bracket(s, l));

		if(lw >= hg || hg > RAND_MAX) {
			_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
		}

		rnd = (real_t)(rand() % (hg - lw + 1) + lw); /* [LOW, HIGH] */

		mb_check(mb_push_int(s, l, (int_t)rnd));
	} else {
		mb_check(mb_attempt_func_begin(s, l));

		mb_check(mb_attempt_func_end(s, l));

		rnd = (real_t)(((real_t)(rand() % 101)) / 100.0f); /* [0.0, 1.0] */

		mb_check(mb_push_real(s, l, rnd));
	}

_exit:
	return result;
}

/* Get the sin value of a number */
static int _std_sin(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, sin, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the cos value of a number */
static int _std_cos(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, cos, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the tan value of a number */
static int _std_tan(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, tan, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the asin value of a number */
static int _std_asin(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, asin, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the acos value of a number */
static int _std_acos(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, acos, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the atan value of a number */
static int _std_atan(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, atan, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the exp value of a number */
static int _std_exp(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, exp, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the log value of a number */
static int _std_log(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	_math_calculate_fun_real(s, l, arg, log, _exit, result);

_exit:
	mb_check(mb_push_value(s, l, arg));

	return result;
}

/* Get the ASCII code of a character */
static int _std_asc(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* arg = 0;
	int_t val = 0;
#ifdef MB_ENABLE_UNICODE
	size_t sz = 0;
#endif /* MB_ENABLE_UNICODE */

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_string(s, l, &arg));

	mb_check(mb_attempt_close_bracket(s, l));

	if(arg[0] == _ZERO_CHAR) {
		result = MB_FUNC_ERR;

		goto _exit;
	}
#ifdef MB_ENABLE_UNICODE
	sz = (size_t)mb_uu_ischar(arg);
	if(sizeof(int_t) < sz) {
		sz = sizeof(int_t);
		_handle_error_on_obj(s, SE_RN_OVERFLOW, s->source_file, DON2(l), MB_FUNC_WARNING, _exit, result);
	}
	memcpy(&val, arg, sz);
#else /* MB_ENABLE_UNICODE */
	val = (int_t)arg[0];
#endif /* MB_ENABLE_UNICODE */
	mb_check(mb_push_int(s, l, val));

_exit:
	return result;
}

/* Get the character of an ASCII code */
static int _std_chr(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	int_t arg = 0;
	char* chr = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_int(s, l, &arg));

	mb_check(mb_attempt_close_bracket(s, l));

	chr = (char*)mb_malloc(sizeof(arg) + 1);
	memset(chr, 0, sizeof(arg) + 1);
	memcpy(chr, &arg, sizeof(arg));
	mb_check(mb_push_string(s, l, chr));

	return result;
}

/* Get a number of characters from the left of a string */
static int _std_left(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* arg = 0;
	int_t count = 0;
	char* sub = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_string(s, l, &arg));
	mb_check(mb_pop_int(s, l, &count));

	mb_check(mb_attempt_close_bracket(s, l));

	if(count <= 0) {
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

#ifdef MB_ENABLE_UNICODE
	switch(mb_uu_substr(arg, 0, (int)count, &sub)) {
	case 0:
		_handle_error_on_obj(s, SE_RN_INVALID_STRING, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	case -1:
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_UNICODE */
	sub = (char*)mb_malloc(count + 1);
	memcpy(sub, arg, count);
	sub[count] = _ZERO_CHAR;
#endif /* MB_ENABLE_UNICODE */
	mb_check(mb_push_string(s, l, sub));

_exit:
	return result;
}

/* Get a number of characters from a specific position of a string */
static int _std_mid(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* arg = 0;
	int_t start = 0;
	int_t count = 0;
	char* sub = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_string(s, l, &arg));
	mb_check(mb_pop_int(s, l, &start));
	mb_check(mb_pop_int(s, l, &count));

	mb_check(mb_attempt_close_bracket(s, l));

	if(count <= 0 || start < 0 || start >= (int_t)strlen(arg)) {
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

#ifdef MB_ENABLE_UNICODE
	switch(mb_uu_substr(arg, start, (int)count, &sub)) {
	case 0:
		_handle_error_on_obj(s, SE_RN_INVALID_STRING, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	case -1:
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_UNICODE */
	sub = (char*)mb_malloc(count + 1);
	memcpy(sub, arg + start, count);
	sub[count] = _ZERO_CHAR;
#endif /* MB_ENABLE_UNICODE */
	mb_check(mb_push_string(s, l, sub));

_exit:
	return result;
}

/* Get a number of characters from the right of a string */
static int _std_right(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* arg = 0;
	int_t count = 0;
	char* sub = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_string(s, l, &arg));
	mb_check(mb_pop_int(s, l, &count));

	mb_check(mb_attempt_close_bracket(s, l));

	if(count <= 0) {
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}

#ifdef MB_ENABLE_UNICODE
	switch(mb_uu_substr(arg, (int)(mb_uu_strlen(arg) - count), (int)count, &sub)) {
	case 0:
		_handle_error_on_obj(s, SE_RN_INVALID_STRING, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	case -1:
		_handle_error_on_obj(s, SE_RN_INDEX_OUT_OF_BOUND, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

		break;
	}
#else /* MB_ENABLE_UNICODE */
	sub = (char*)mb_malloc(count + 1);
	memcpy(sub, arg + (strlen(arg) - count), count);
	sub[count] = _ZERO_CHAR;
#endif /* MB_ENABLE_UNICODE */
	mb_check(mb_push_string(s, l, sub));

_exit:
	return result;
}

/* Get the string format of a number */
static int _std_str(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t arg;
	_dynamic_buffer_t buf;
	size_t lbuf = 32;

	mb_assert(s && l);

	mb_make_nil(arg);

	_INIT_BUF(buf);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	switch(arg.type) {
	case MB_DT_INT:
		lbuf = 32; /* Enough for even 64bit integer */
		_RESIZE_CHAR_BUF(buf, lbuf);
		if((size_t)sprintf(_CHAR_BUF_PTR(buf), MB_INT_FMT, arg.value.integer) >= lbuf) {
			mb_assert(0 && "Buffer overflow.");
		}

		break;
	case MB_DT_REAL:
		lbuf = 1 /* - */ + (DBL_MAX_10_EXP + 1) /* 308 + 1 digits */ + 1 /* . */ + 6 /* precision */ + 1 /* \0 */;
		_RESIZE_CHAR_BUF(buf, lbuf);
#ifdef MB_MANUAL_REAL_FORMATTING
		_real_to_str(arg.value.float_point, _CHAR_BUF_PTR(buf), lbuf, 5);
#else /* MB_MANUAL_REAL_FORMATTING */
		mb_realtostr(arg.value.float_point, _CHAR_BUF_PTR(buf), lbuf);
#endif /* MB_MANUAL_REAL_FORMATTING */

		break;
	case MB_DT_STRING: {
			char* ret = mb_strdup(arg.value.string, strlen(arg.value.string) + 1);
			mb_check(mb_push_string(s, l, ret));

			goto _exit;
		}
	case MB_DT_TYPE: {
			const char* sp = mb_get_type_string(arg.value.type);
			char* ret = mb_strdup(sp, strlen(sp) + 1);
			mb_check(mb_push_string(s, l, ret));

			goto _exit;
		}
#ifdef MB_ENABLE_CLASS
	case MB_DT_CLASS: {
			bool_t got_tostr = false;
			_class_t* instance = (_class_t*)arg.value.instance;
			_object_t val_obj;
			_MAKE_NIL(&val_obj);
			if((result = _format_class_to_string(s, l, instance, &val_obj, &got_tostr)) == MB_FUNC_OK) {
				if(got_tostr) {
					mb_check(mb_push_string(s, l, val_obj.data.string));
				} else {
					const char* sp = mb_get_type_string(_internal_type_to_public_type(_DT_CLASS));
					char* ret = mb_strdup(sp, strlen(sp) + 1);
					mb_check(mb_push_string(s, l, ret));
				}

				goto _exit;
			} else {
				goto _exit;
			}
		}
#endif /* MB_ENABLE_CLASS */
	default:
		result = MB_FUNC_ERR;

		goto _exit;
	}
	mb_check(mb_push_string(s, l, _HEAP_CHAR_BUF(buf)));

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* Get the number format of a string, or get the value of a dictionary iterator */
static int _std_val(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	char* conv_suc = 0;
	mb_value_t arg;
#ifdef MB_ENABLE_COLLECTION_LIB
	_object_t ocoi;
#endif /* MB_ENABLE_COLLECTION_LIB */
	mb_value_t ret;
	mb_meta_status_e os = MB_MS_NONE;
#ifdef MB_ENABLE_COLLECTION_LIB
	_ls_node_t* ast = 0;
#endif /* MB_ENABLE_COLLECTION_LIB */

	mb_assert(s && l);

	mb_make_nil(arg);
	mb_make_nil(ret);

	mb_check(mb_attempt_open_bracket(s, l));

#ifdef MB_ENABLE_COLLECTION_LIB
	ast = (_ls_node_t*)*l;
	if(ast && _IS_FUNC(ast->data, _coll_iterator)) {
		_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
	}
#endif /* MB_ENABLE_COLLECTION_LIB */
	mb_check(mb_pop_value(s, l, &arg));
	os = _try_overridden(s, l, &arg, _STD_ID_VAL, MB_MF_FUNC);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		switch(arg.type) {
		case MB_DT_STRING:
			ret.value.integer = (int_t)mb_strtol(arg.value.string, &conv_suc, 0);
			if(*conv_suc == _ZERO_CHAR) {
				ret.type = MB_DT_INT;
				mb_check(mb_push_value(s, l, ret));

				goto _exit;
			}
			ret.value.float_point = (real_t)mb_strtod(arg.value.string, &conv_suc);
			if(*conv_suc == _ZERO_CHAR) {
				ret.type = MB_DT_REAL;
				mb_check(mb_push_value(s, l, ret));

				goto _exit;
			}
			result = MB_FUNC_ERR;

			break;
#ifdef MB_ENABLE_COLLECTION_LIB
		case MB_DT_LIST_IT:
			_MAKE_NIL(&ocoi);
			_public_value_to_internal_object(&arg, &ocoi);
			_handle_error_on_obj(s, SE_RN_UNEXPECTED_TYPE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		case MB_DT_DICT_IT:
			_MAKE_NIL(&ocoi);
			_public_value_to_internal_object(&arg, &ocoi);
			if(ocoi.data.dict_it && ocoi.data.dict_it->curr_node && ocoi.data.dict_it->curr_node != _INVALID_DICT_IT && ocoi.data.dict_it->curr_node->data) {
				_internal_object_to_public_value((_object_t*)ocoi.data.dict_it->curr_node->data, &ret);
				mb_check(mb_push_value(s, l, ret));
			} else {
				_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}

			break;
#endif /* MB_ENABLE_COLLECTION_LIB */
		default:
			_assign_public_value(s, &arg, 0, true);
			_handle_error_on_obj(s, SE_RN_UNEXPECTED_TYPE, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	} else {
		if((os & MB_MS_RETURNED) == MB_MS_NONE) {
			mb_check(mb_push_value(s, l, ret));
		}
	}

_exit:
	mb_check(mb_attempt_close_bracket(s, l));

	return result;
}

/* Get the length of a string or an array, or element count of a collection or a variable argument list */
static int _std_len(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	mb_value_t arg;
	_array_t* arr = 0;
#ifdef MB_ENABLE_COLLECTION_LIB
	_list_t* lst = 0;
	_dict_t* dct = 0;
#endif /* MB_ENABLE_COLLECTION_LIB */
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(arg);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	ast = (_ls_node_t*)*l;
	if(ast) obj = (_object_t*)ast->data;
	if(obj && _IS_FUNC(obj, _core_args)) {
		ast = ast->next;
		*l = ast;
		_mb_check_mark_exit(mb_push_int(s, l, s->var_args ? (int_t)_ls_count(s->var_args) : 0), result, _exit);

		_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

		goto _exit;
	}
	_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);
	os = _try_overridden(s, l, &arg, _STD_ID_LEN, MB_MF_FUNC);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		switch(arg.type) {
		case MB_DT_STRING:
#ifdef MB_ENABLE_UNICODE
			_mb_check_mark_exit(mb_push_int(s, l, (int_t)mb_uu_strlen(arg.value.string)), result, _exit);
#else /* MB_ENABLE_UNICODE */
			_mb_check_mark_exit(mb_push_int(s, l, (int_t)strlen(arg.value.string)), result, _exit);
#endif /* MB_ENABLE_UNICODE */

			break;
		case MB_DT_ARRAY:
			arr = (_array_t*)arg.value.array;
			_mb_check_mark_exit(mb_push_int(s, l, (int_t)arr->count), result, _exit);

			break;
#ifdef MB_ENABLE_COLLECTION_LIB
		case MB_DT_LIST:
			lst = (_list_t*)arg.value.list;
			_mb_check_mark_exit(mb_push_int(s, l, (int_t)lst->count), result, _exit);
			_assign_public_value(s, &arg, 0, true);

			break;
		case MB_DT_DICT:
			dct = (_dict_t*)arg.value.dict;
			_mb_check_mark_exit(mb_push_int(s, l, (int_t)_ht_count(dct->dict)), result, _exit);
			_assign_public_value(s, &arg, 0, true);

			break;
#endif /* MB_ENABLE_COLLECTION_LIB */
		default:
			_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);

			break;
		}
	} else {
		if((os & MB_MS_RETURNED) == MB_MS_NONE) {
			_mb_check_mark_exit(mb_push_int(s, l, 0), result, _exit);
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

_exit:
	_assign_public_value(s, &arg, 0, true);

	return result;
}

/* GET statement */
static int _std_get(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t ov;
	mb_value_t arg;
	_object_t obj;
#ifdef MB_ENABLE_COLLECTION_LIB
	int_t index = 0;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	char* field = 0;
	_ls_node_t* fnode = 0;
	_object_t* fobj = 0;
#endif /* MB_ENABLE_CLASS */
	mb_value_t ret;
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(ov);
	mb_make_nil(arg);
	mb_make_nil(ret);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &ov), result, _exit);
	os = _try_overridden(s, l, &ov, _STD_ID_GET, MB_MF_FUNC);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_MAKE_NIL(&obj);
		switch(ov.type) {
#ifdef MB_ENABLE_COLLECTION_LIB
		case MB_DT_LIST:
			_public_value_to_internal_object(&ov, &obj);
			_mb_check_mark_exit(mb_pop_int(s, l, &index), result, _exit);
			if(!_at_list(obj.data.list, index, &ret)) {
				_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}

			break;
		case MB_DT_DICT:
			_public_value_to_internal_object(&ov, &obj);
			_mb_check_mark_exit(mb_pop_value(s, l, &arg), result, _exit);
			if(!_find_dict(obj.data.dict, &arg, &ret)) {
				_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}

			break;
		case MB_DT_LIST_IT:
			_public_value_to_internal_object(&ov, &obj);
			if(obj.data.list_it && !obj.data.list_it->list->range_begin && obj.data.list_it->curr.node && obj.data.list_it->curr.node->data) {
				_internal_object_to_public_value((_object_t*)obj.data.list_it->curr.node->data, &ret);
			} else if(obj.data.list_it && obj.data.list_it->list->range_begin) {
				mb_make_int(ret, obj.data.list_it->curr.ranging);
			} else {
				_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}

			break;
		case MB_DT_DICT_IT:
			_public_value_to_internal_object(&ov, &obj);
			if(obj.data.dict_it && obj.data.dict_it->curr_node && obj.data.dict_it->curr_node != _INVALID_DICT_IT && obj.data.dict_it->curr_node->extra) {
				_internal_object_to_public_value((_object_t*)obj.data.dict_it->curr_node->extra, &ret);
			} else {
				_handle_error_on_obj(s, SE_RN_INVALID_ITERATOR, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}

			break;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
		case MB_DT_CLASS:
			_public_value_to_internal_object(&ov, &obj);
			_mb_check_mark_exit(mb_pop_string(s, l, &field), result, _exit);
			field = mb_strupr(field);
			fnode = _search_identifier_in_class(s, obj.data.instance, field, 0, 0);
			if(fnode && fnode->data) {
				fobj = (_object_t*)fnode->data;
				_internal_object_to_public_value(fobj, &ret);
			}

			break;
#endif /* MB_ENABLE_CLASS */
		default:
			_handle_error_on_obj(s, SE_RN_COLLECTION_OR_ITERATOR_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, ret), result, _exit);
	}

_exit:
	_assign_public_value(s, &ov, 0, true);

	return result;
}

/* SET statement */
static int _std_set(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	mb_value_t ov;
	mb_value_t key;
	mb_value_t val;
	_object_t obj;
#ifdef MB_ENABLE_COLLECTION_LIB
	_object_t* oval = 0;
	int_t idx = 0;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
	char* field = 0;
	_ls_node_t* fnode = 0;
	_object_t* fobj = 0;
	mb_value_t nv;
#endif /* MB_ENABLE_CLASS */
	mb_meta_status_e os = MB_MS_NONE;

	mb_assert(s && l);

	mb_make_nil(ov);
	mb_make_nil(key);
	mb_make_nil(val);

	_mb_check_mark_exit(mb_attempt_open_bracket(s, l), result, _exit);

	_mb_check_mark_exit(mb_pop_value(s, l, &ov), result, _exit);
	os = _try_overridden(s, l, &ov, _STD_ID_SET, MB_MF_FUNC);
	if((os & MB_MS_DONE) == MB_MS_NONE) {
		_MAKE_NIL(&obj);
		switch(ov.type) {
#ifdef MB_ENABLE_COLLECTION_LIB
		case MB_DT_LIST:
			_public_value_to_internal_object(&ov, &obj);
			while(mb_has_arg(s, l)) {
				mb_make_nil(val);
				_mb_check_mark_exit(mb_pop_int(s, l, &idx), result, _exit);
				_mb_check_mark_exit(mb_pop_value(s, l, &val), result, _exit);
				if(!_set_list(obj.data.list, idx, &val, &oval)) {
					if(oval)
						_destroy_object(oval, 0);

					_handle_error_on_obj(s, SE_RN_CANNOT_FIND_WITH_THE_SPECIFIC_INDEX, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
				}
			}

			break;
		case MB_DT_DICT:
			_public_value_to_internal_object(&ov, &obj);
			while(mb_has_arg(s, l)) {
				mb_make_nil(key);
				mb_make_nil(val);
				_mb_check_mark_exit(mb_pop_value(s, l, &key), result, _exit);
				_mb_check_mark_exit(mb_pop_value(s, l, &val), result, _exit);
				if(!_set_dict(obj.data.dict, &key, &val, 0, 0)) {
					_handle_error_on_obj(s, SE_RN_INVALID_EXPRESSION, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
				}
			}

			break;
#endif /* MB_ENABLE_COLLECTION_LIB */
#ifdef MB_ENABLE_CLASS
		case MB_DT_CLASS:
			mb_make_nil(nv);

			_public_value_to_internal_object(&ov, &obj);
			_mb_check_mark_exit(mb_pop_string(s, l, &field), result, _exit);
			_mb_check_mark_exit(mb_pop_value(s, l, &nv), result, _exit);
			field = mb_strupr(field);
			fnode = _search_identifier_in_class(s, obj.data.instance, field, 0, 0);
			if(fnode && _IS_VAR(fnode->data)) {
				_object_t* nobj = 0;
				fobj = (_object_t*)fnode->data;
				_destroy_object(fobj->data.variable->data, 0);
				_create_internal_object_from_public_value(&nv, &nobj);
				fobj->data.variable->data = nobj;
			} else {
				_handle_error_on_obj(s, SE_RN_VAR_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);
			}

			break;
#endif /* MB_ENABLE_CLASS */
		default:
			_handle_error_on_obj(s, SE_RN_COLLECTION_EXPECTED, s->source_file, DON2(l), MB_FUNC_ERR, _exit, result);

			break;
		}
	}

	_mb_check_mark_exit(mb_attempt_close_bracket(s, l), result, _exit);

	if((os & MB_MS_RETURNED) == MB_MS_NONE) {
		_mb_check_mark_exit(mb_push_value(s, l, ov), result, _exit);
	}

_exit:
	_assign_public_value(s, &ov, 0, true);

	return result;
}

/* PRINT statement */
static int _std_print(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	_object_t val_obj;
	_object_t* val_ptr = 0;
	bool_t pathed_str = false;

	mb_assert(s && l);

	++s->no_eat_comma_mark;
	ast = (_ls_node_t*)*l;
	if(!ast || !ast->next || !ast->next->data) {
		_handle_error_on_obj(s, SE_RN_SYNTAX_ERROR, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	ast = ast->next;

	obj = (_object_t*)ast->data;
	do {
		pathed_str = false;
		val_ptr = &val_obj;
		_MAKE_NIL(val_ptr);
		switch(obj->type) {
		case _DT_VAR:
			if(obj->data.variable->data->type == _DT_ROUTINE) {
				obj = obj->data.variable->data;
				val_ptr = _eval_var_in_print(s, &val_ptr, &ast, obj);

				goto _print;
			}
#ifdef MB_ENABLE_CLASS
			if(obj->data.variable->pathing) {
				_ls_node_t* pathed = _search_identifier_in_scope_chain(s, 0, obj->data.variable->name, _PU(obj->data.variable->pathing), 0, 0);
				if(pathed && pathed->data) {
					if(obj != (_object_t*)pathed->data) {
						obj = (_object_t*)pathed->data;
						val_ptr = _eval_var_in_print(s, &val_ptr, &ast, obj);
						if(val_ptr->type == _DT_STRING)
							pathed_str = true;
					}
				}

				goto _print;
			}
#endif /* MB_ENABLE_CLASS */
			/* Fall through */
		case _DT_ARRAY: /* Fall through */
		case _DT_NIL: /* Fall through */
		case _DT_INT: /* Fall through */
		case _DT_REAL: /* Fall through */
		case _DT_STRING: /* Fall through */
		case _DT_TYPE: /* Fall through */
#ifdef MB_ENABLE_CLASS
		case _DT_CLASS: /* Fall through */
#endif /* MB_ENABLE_CLASS */
		case _DT_FUNC: /* Fall through */
		case _DT_ROUTINE:
			result = _calc_expression(s, &ast, &val_ptr);
			if(val_ptr->type == _DT_ROUTINE) {
#ifdef MB_ENABLE_LAMBDA
				if(val_ptr->data.routine->type != MB_RT_LAMBDA)
					val_ptr->is_ref = true;
#else /* MB_ENABLE_LAMBDA */
				val_ptr->is_ref = true;
#endif /* MB_ENABLE_LAMBDA */
			}
			_REF(val_ptr)
			_UNREF(val_ptr)
_print:
			if(val_ptr->type == _DT_NIL) {
				_get_printer(s)(s, MB_NIL);
			} else if(val_ptr->type == _DT_INT) {
				_get_printer(s)(s, MB_INT_FMT, val_ptr->data.integer);
			} else if(val_ptr->type == _DT_REAL) {
#ifdef MB_MANUAL_REAL_FORMATTING
				_dynamic_buffer_t buf;
				size_t lbuf = 32;
				_INIT_BUF(buf);
				_RESIZE_CHAR_BUF(buf, lbuf);
				_real_to_str(val_ptr->data.float_point, _CHAR_BUF_PTR(buf), lbuf, 5);
				_get_printer(s)(s, "%s", _CHAR_BUF_PTR(buf));
				_DISPOSE_BUF(buf);
#else /* MB_MANUAL_REAL_FORMATTING */
				_get_printer(s)(s, MB_REAL_FMT, val_ptr->data.float_point);
#endif /* MB_MANUAL_REAL_FORMATTING */
			} else if(val_ptr->type == _DT_STRING) {
				_print_string(s, val_ptr);
				if(!val_ptr->is_ref && val_ptr->data.string && !pathed_str) {
					safe_free(val_ptr->data.string);
				}
#ifdef MB_ENABLE_USERTYPE_REF
			} else if(val_ptr->type == _DT_USERTYPE_REF) {
				if(val_ptr->data.usertype_ref->fmt) {
					_dynamic_buffer_t buf;
					size_t lbuf = 0;
					_INIT_BUF(buf);
					while((lbuf = (size_t)val_ptr->data.usertype_ref->fmt(s, val_ptr->data.usertype_ref->usertype, _CHAR_BUF_PTR(buf), (unsigned)_CHARS_OF_BUF(buf))) > _CHARS_OF_BUF(buf)) {
						_RESIZE_CHAR_BUF(buf, lbuf);
					}
					_get_printer(s)(s, "%s", _CHAR_BUF_PTR(buf));
					_DISPOSE_BUF(buf);
				} else {
					_get_printer(s)(s, mb_get_type_string(_internal_type_to_public_type(val_ptr->type)));
				}
#endif /* MB_ENABLE_USERTYPE_REF */
			} else if(val_ptr->type == _DT_TYPE) {
				_get_printer(s)(s, mb_get_type_string(val_ptr->data.type));
#ifdef MB_ENABLE_CLASS
			} else if(val_ptr->type == _DT_CLASS) {
				bool_t got_tostr = false;
				_class_t* instance = val_ptr->data.instance;
				val_ptr = &val_obj;
				_MAKE_NIL(val_ptr);
				if((result = _format_class_to_string(s, (void**)&ast, instance, val_ptr, &got_tostr)) == MB_FUNC_OK) {
					if(got_tostr) {
						obj = val_ptr;

						goto _print;
					} else {
						_get_printer(s)(s, mb_get_type_string(_internal_type_to_public_type(_DT_CLASS)));
					}
				} else {
					goto _exit;
				}
#endif /* MB_ENABLE_CLASS */
			} else {
				_get_printer(s)(s, mb_get_type_string(_internal_type_to_public_type(val_ptr->type)));
			}
			if(result != MB_FUNC_OK)
				goto _exit;
			/* Fall through */
		case _DT_SEP:
			if(!ast)
				break;
			obj = (_object_t*)ast->data;
#if _COMMA_AS_NEWLINE
			if(obj->data.separator == ',') {
#else /* _COMMA_AS_NEWLINE */
			if(obj->data.separator == ';') {
#endif /* _COMMA_AS_NEWLINE */
				_get_printer(s)(s, "\n");
			}

			break;
		default:
			_handle_error_on_obj(s, SE_CM_NOT_SUPPORTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);

			break;
		}

		if(!ast)
			break;
		obj = (_object_t*)ast->data;
		if(_is_print_terminal(s, obj))
			break;
		if(_IS_SEP(obj, ',') || _IS_SEP(obj, ';')) {
			ast = ast->next;
			obj = (_object_t*)ast->data;
		} else {
			_handle_error_on_obj(s, SE_RN_COMMA_OR_SEMICOLON_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
	} while(ast && !_IS_SEP(obj, ':') && !_IS_FUNC(obj, _core_close_bracket) && (obj->type == _DT_SEP || !_is_expression_terminal(s, obj)));

_exit:
	--s->no_eat_comma_mark;

	*l = ast;
	if(result != MB_FUNC_OK)
		_get_printer(s)(s, "\n");

	return result;
}

/* INPUT statement */
static int _std_input(mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	_ls_node_t* ast = 0;
	_object_t* obj = 0;
	char line[_INPUT_MAX_LENGTH];
	char* conv_suc = 0;
	const char* pmt = 0;

	mb_assert(s && l);

	mb_check(mb_attempt_func_begin(s, l));

	mb_check(mb_attempt_func_end(s, l));

	ast = (_ls_node_t*)*l;
	obj = ast ? (_object_t*)ast->data : 0;

	if(!obj || _IS_EOS(obj)) {
#ifdef MB_CP_VC
		getch();
#else /* MB_CP_VC */
		_get_inputer(s)(s, pmt, line, sizeof(line));
#endif /* MB_CP_VC */

		goto _exit;
	}
	if(obj->type == _DT_STRING) {
		pmt = obj->data.string;
#if MB_PRINT_INPUT_PROMPT
		_print_string(s, obj);
#endif /* MB_PRINT_INPUT_PROMPT */
		ast = ast->next;
		obj = (_object_t*)ast->data;
		if(!_IS_SEP(obj, ',')) {
			_handle_error_on_obj(s, SE_RN_COMMA_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
		}
		ast = ast->next;
		obj = (_object_t*)ast->data;
	}
	if(!_IS_VAR(obj)) {
		_handle_error_on_obj(s, SE_RN_VAR_EXPECTED, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}
	if(obj->data.variable->data->type == _DT_INT || obj->data.variable->data->type == _DT_REAL) {
		_get_inputer(s)(s, pmt, line, sizeof(line));
		obj->data.variable->data->type = _DT_INT;
		obj->data.variable->data->data.integer = (int_t)mb_strtol(line, &conv_suc, 0);
		if(*conv_suc == _ZERO_CHAR) {
#if MB_PRINT_INPUT_CONTENT
			_get_printer(s)(s, MB_INT_FMT "\n", obj->data.variable->data->data.integer);
#endif /* MB_PRINT_INPUT_CONTENT */
		} else {
			obj->data.variable->data->type = _DT_REAL;
			obj->data.variable->data->data.float_point = (real_t)mb_strtod(line, &conv_suc);
			if(*conv_suc == _ZERO_CHAR) {
#if MB_PRINT_INPUT_CONTENT
				_get_printer(s)(s, MB_REAL_FMT "\n", obj->data.variable->data->data.float_point);
#endif /* MB_PRINT_INPUT_CONTENT */
			} else {
				_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
			}
		}
		ast = ast->next;
	} else if(obj->data.variable->data->type == _DT_STRING) {
		size_t len = 0;
		if(obj->data.variable->data->data.string && !obj->data.variable->data->is_ref) {
			safe_free(obj->data.variable->data->data.string);
		}
		len = (size_t)_get_inputer(s)(s, pmt, line, sizeof(line));
#if defined MB_CP_VC && defined MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING
		do {
			_dynamic_buffer_t buf;
			_dynamic_buffer_t wbuf;
			_INIT_BUF(buf);
			_INIT_BUF(wbuf);
			while((len = (size_t)mb_bytes_to_wchar_ansi(line, &_WCHAR_BUF_PTR(wbuf), _WCHARS_OF_BUF(wbuf))) > _WCHARS_OF_BUF(wbuf)) {
				_RESIZE_WCHAR_BUF(wbuf, len);
			}
			while((len = mb_wchar_to_bytes(_WCHAR_BUF_PTR(wbuf), &_CHAR_BUF_PTR(buf), _CHARS_OF_BUF(buf))) > _CHARS_OF_BUF(buf)) {
				_RESIZE_CHAR_BUF(buf, len);
			}
#if MB_PRINT_INPUT_CONTENT
			_get_printer(s)(s, "%ls\n", _WCHAR_BUF_PTR(wbuf));
#endif /* MB_PRINT_INPUT_CONTENT */
			_DISPOSE_BUF(wbuf);
			obj->data.variable->data->data.string = _HEAP_CHAR_BUF(buf);
			obj->data.variable->data->is_ref = false;
		} while(0);
#else /* MB_CP_VC && MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING */
		obj->data.variable->data->data.string = mb_memdup(line, (unsigned)(len + 1));
#if MB_PRINT_INPUT_CONTENT
		_get_printer(s)(s, "%s\n", obj->data.variable->data->data.string);
#endif /* MB_PRINT_INPUT_CONTENT */
#endif /* MB_CP_VC && MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING*/
		ast = ast->next;
	} else {
		_handle_error_on_obj(s, SE_RN_INVALID_ID_USAGE, s->source_file, DON(ast), MB_FUNC_ERR, _exit, result);
	}

_exit:
	*l = ast;

	return result;
}

