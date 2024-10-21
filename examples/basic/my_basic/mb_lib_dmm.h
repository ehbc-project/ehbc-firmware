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

#ifndef __MB_LIB_DMM_H__
#define __MB_LIB_DMM_H__

#include "config.h"
#include "mb_internal.h"

#ifdef MB_ENABLE_DIRECT_MEMORY_MANIPULATION_LIB

_MB_DECLARE_LIBRARY(dmm)

/* Identifier */
#define _DMM_ID_PEEK "PEEK"
#define _DMM_ID_POKE "POKE"

#endif /* MB_ENABLE_DIRECT_MEMORY_MANIPULATION_LIB */

#endif // __MB_LIB_DMM_H__
