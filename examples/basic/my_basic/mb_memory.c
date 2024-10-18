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

/** Memory manipulations */

/* Initialize a chunk of resizable dynamic buffer */
void _init_dynamic_buffer(_dynamic_buffer_t* buf) {
	mb_assert(buf);

	memset(buf->bytes, 0, sizeof(buf->bytes));
	buf->pointer.charp = buf->bytes;
	buf->size = sizeof(buf->bytes);
}

/* Dispose a chunk of resizable dynamic buffer */
void _dispose_dynamic_buffer(_dynamic_buffer_t* buf) {
	mb_assert(buf);

	if(buf->pointer.charp != buf->bytes) {
		safe_free(buf->pointer.charp);
	}
	buf->pointer.charp = 0;
	buf->size = 0;
}

/* Get the element count of a chunk of resizable dynamic buffer */
size_t _countof_dynamic_buffer(_dynamic_buffer_t* buf, size_t es) {
	mb_assert(buf);

	return buf->size / es;
}

/* Resize a chunk of resizable dynamic buffer */
void _resize_dynamic_buffer(_dynamic_buffer_t* buf, size_t es, size_t c) {
	size_t as = es * c;

	mb_assert(buf);

	if(as > buf->size) {
		if(buf->pointer.charp != buf->bytes) {
			safe_free(buf->pointer.charp);
		}
		buf->pointer.charp = (char*)mb_malloc(as);
		buf->size = as;
	}
}

/* Allocate a chunk of memory with a specific size */
void* mb_malloc(size_t s) {
	char* ret = 0;
	size_t rs = s;

#ifdef MB_ENABLE_ALLOC_STAT
	if(!_MB_CHECK_MEM_TAG_SIZE(size_t, s))
		return 0;
	rs += _MB_MEM_TAG_SIZE;
#endif /* MB_ENABLE_ALLOC_STAT */
	if(_mb_allocate_func)
		ret = _mb_allocate_func((unsigned)rs);
	else
		ret = (char*)malloc(rs);
	mb_assert(ret);
#ifdef MB_ENABLE_ALLOC_STAT
	_mb_allocated += s;
	ret += _MB_MEM_TAG_SIZE;
	_MB_WRITE_MEM_TAG_SIZE(ret, s);
#endif /* MB_ENABLE_ALLOC_STAT */

	return (void*)ret;
}

/* Free a chunk of memory */
void mb_free(void* p) {
	mb_assert(p);

#ifdef MB_ENABLE_ALLOC_STAT
	do {
		size_t os = _MB_READ_MEM_TAG_SIZE(p);
		_mb_allocated -= os;
		p = (char*)p - _MB_MEM_TAG_SIZE;
	} while(0);
#endif /* MB_ENABLE_ALLOC_STAT */
	if(_mb_free_func)
		_mb_free_func((char*)p);
	else
		free(p);
}

/* Compare two chunks of memory */
int mb_memcmp(void* l, void* r, size_t s) {
	unsigned char* lc = (unsigned char*)l;
	unsigned char* rc = (unsigned char*)r;
	int i = 0;

	if(mb_is_little_endian()) {
		for(i = (int)s - 1; i >= 0; i--) {
			if(lc[i] < rc[i])
				return -1;
			else if(lc[i] > rc[i])
				return 1;
		}
	} else {
		for(i = 0; i < (int)s; i++) {
			if(lc[i] < rc[i])
				return -1;
			else if(lc[i] > rc[i])
				return 1;
		}
	}

	return 0;
}

/* Detect whether a chunk of memory contains any non-zero byte */
size_t mb_memtest(void* p, size_t s) {
	size_t result = 0;
	size_t i = 0;

	for(i = 0; i < s; i++)
		result += ((unsigned char*)p)[i];

	return result;
}

/* Duplicate a string */
char* mb_strdup(const char* p, size_t s) {
#ifdef MB_ENABLE_ALLOC_STAT
	if(!s) {
		s = _MB_READ_MEM_TAG_SIZE(p);
	}

	return mb_memdup(p, (unsigned)s);
#else /* MB_ENABLE_ALLOC_STAT */
	if(s)
		return mb_memdup(p, (unsigned)s);

	return mb_memdup(p, (unsigned)(strlen(p) + 1));
#endif /* MB_ENABLE_ALLOC_STAT */
}

/* Change a string to upper case */
char* mb_strupr(char* s) {
	char* t = s;

	while(*s) {
		*s = toupper(*s);
		++s;
	}

	return t;
}

/* Determine whether it's running on a little endian platform */
bool_t mb_is_little_endian(void) {
	int i = 1;

	return ((char*)&i)[0] == 1;
}
