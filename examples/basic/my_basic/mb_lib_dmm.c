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

#include "mb_lib_dmm.h"

/** Direct Memory Manipulation lib */

#ifdef MB_ENABLE_DIRECT_MEMORY_MANIPULATION_LIB

static int _dmm_peek(mb_interpreter_t* s, void** l);
static int _dmm_poke(mb_interpreter_t* s, void** l);

MBCONST static const _func_t _dmm_libs[] = {
	{ _DMM_ID_PEEK, _dmm_peek },
	{ _DMM_ID_POKE, _dmm_poke },
};

_MB_DEFINE_LIBRARY(dmm)

/* PEEK function */
static int _dmm_peek(struct mb_interpreter_t *s, void **l) {
	int result = MB_FUNC_OK;
	int_t val;

	mb_assert(s && l);

	mb_check(mb_attempt_open_bracket(s, l));

	mb_check(mb_pop_int(s, l, &val));

	mb_check(mb_attempt_close_bracket(s, l));

	val = *(uint8_t*)val;

	mb_check(mb_push_int(s, l, val));

	return result;
}

/* POKE statement */
static int _dmm_poke(struct mb_interpreter_t *s, void **l) {
	int result = MB_FUNC_OK;
	int_t addr;
	int_t val;

	mb_assert(s && l);

	mb_check(mb_pop_int(s, l, &addr));

	mb_check(mb_pop_int(s, l, &val));

	*(uint8_t*)addr = val;

	return result;
}

#endif /* MB_ENABLE_DIRECT_MEMORY_MANIPULATION_LIB */

