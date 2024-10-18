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

/* Unicode operations */

#ifdef MB_ENABLE_UNICODE
/* Determine whether a buffer starts with a UTF8 encoded character, and return taken byte count */
int mb_uu_ischar(const char* ch) {
	/* Copyright 2008, 2009 Bjoern Hoehrmann, http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ */
#define _TAKE(__ch, __c, __r) do { __c = *__ch++; __r++; } while(0)
#define _COPY(__ch, __c, __r, __cp) do { _TAKE(__ch, __c, __r); __cp = (__cp << 6) | ((unsigned char)__c & 0x3Fu); } while(0)
#define _TRANS(__m, __cp, __g) do { __cp &= ((__g[(unsigned char)c] & __m) != 0); } while(0)
#define _TAIL(__ch, __c, __r, __cp, __g) do { _COPY(__ch, __c, __r, __cp); _TRANS(0x70, __cp, __g); } while(0)

	MBCONST static const unsigned char range[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
		0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		8, 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		10, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 11, 6, 6, 6, 5, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
	};

	int result = 0;
	unsigned codepoint = 0;
	unsigned char type = 0;
	char c = 0;

	if(!ch)
		return 0;

	_TAKE(ch, c, result);
	if(!(c & 0x80)) {
		codepoint = (unsigned char)c;

		return 1;
	}

	type = range[(unsigned char)c];
	codepoint = (0xFF >> type) & (unsigned char)c;

	switch(type) {
	case 2: _TAIL(ch, c, result, codepoint, range); return result;
	case 3: _TAIL(ch, c, result, codepoint, range); _TAIL(ch, c, result, codepoint, range); return result;
	case 4: _COPY(ch, c, result, codepoint); _TRANS(0x50, codepoint, range); _TAIL(ch, c, result, codepoint, range); return result;
	case 5: _COPY(ch, c, result, codepoint); _TRANS(0x10, codepoint, range); _TAIL(ch, c, result, codepoint, range); _TAIL(ch, c, result, codepoint, range); return result;
	case 6: _TAIL(ch, c, result, codepoint, range); _TAIL(ch, c, result, codepoint, range); _TAIL(ch, c, result, codepoint, range); return result;
	case 10: _COPY(ch, c, result, codepoint); _TRANS(0x20, codepoint, range); _TAIL(ch, c, result, codepoint, range); return result;
	case 11: _COPY(ch, c, result, codepoint); _TRANS(0x60, codepoint, range); _TAIL(ch, c, result, codepoint, range); _TAIL(ch, c, result, codepoint, range); return result;
	default: return 0;
	}

#undef _TAKE
#undef _COPY
#undef _TRANS
#undef _TAIL
}

/* Tell how many UTF8 character are there in a string */
int mb_uu_strlen(const char* ch) {
	int result = 0;

	if(!ch)
		return 0;

	while(*ch) {
		int t = mb_uu_ischar(ch);
		if(t <= 0)
			return t;
		ch += t;
		result++;
	}

	return result;
}

/* Retrieve a sub string of a UTF8 string */
int mb_uu_substr(const char* ch, int begin, int count, char** o) {
	int cnt = 0;
	const char* b = 0;
	const char* e = 0;
	int l = 0;

	if(!ch || begin < 0 || count <= 0 || !o)
		return -1;

	while(*ch) {
		int t = mb_uu_ischar(ch);
		if(t <= 0)
			return t;
		if(cnt == begin) {
			b = ch;

			break;
		}
		ch += t;
		cnt++;
	}

	while(*ch) {
		int t = mb_uu_ischar(ch);
		if(t <= 0)
			return t;
		if(cnt == begin + count) {
			e = ch;

			break;
		}
		ch += t;
		e = ch;
		cnt++;
	}

	if(!(*ch) && (cnt != begin + count))
		return -1;

	l = (int)(e - b);
	*o = (char*)mb_malloc(l + 1);
	memcpy(*o, b, l);
	(*o)[l] = _ZERO_CHAR;

	return l;
}

#endif /* MB_ENABLE_UNICODE */

#if defined MB_CP_VC && defined MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING
/* Map a UTF8 character string to a UTF16 (wide character) string */
int mb_bytes_to_wchar(const char* sz, wchar_t** out, size_t size) {
	int result = MultiByteToWideChar(CP_UTF8, 0, sz, -1, 0, 0);
	if((int)size >= result)
		MultiByteToWideChar(CP_UTF8, 0, sz, -1, *out, result);

	return result;
}

/* Map an ANSI character string to a UTF16 (wide character) string */
int mb_bytes_to_wchar_ansi(const char* sz, wchar_t** out, size_t size) {
	int result = MultiByteToWideChar(CP_ACP, 0, sz, -1, 0, 0);
	if((int)size >= result)
		MultiByteToWideChar(CP_ACP, 0, sz, -1, *out, result);

	return result;
}

/* Map a UTF16 (wide character) string to a UTF8 character string */
int mb_wchar_to_bytes(const wchar_t* sz, char** out, size_t size) {
	int result = WideCharToMultiByte(CP_UTF8, 0, sz, -1, 0, 0, 0, 0);
	if((int)size >= result)
		WideCharToMultiByte(CP_UTF8, 0, sz, -1, *out, result, 0, 0);

	return result;
}
#endif /* MB_CP_VC && MB_ENABLE_UNICODE && MB_UNICODE_NEED_CONVERTING */

/* Determine whether a string begins with a BOM, and ignore it */
int mb_uu_getbom(const char** ch) {
#ifdef __cplusplus
	signed char** ptr = (signed char**)ch;
#else /* __cplusplus */
	char** ptr = (char**)ch;
#endif /* __cplusplus */

	if(!ptr && !(*ptr))
		return 0;

	if((*ptr)[0] == -17 && (*ptr)[1] == -69 && (*ptr)[2] == -65) {
		*ptr += 3;

		return 3;
	} else if((*ptr)[0] == -2 && (*ptr)[1] == -1) {
		*ptr += 2;

		return 2;
	}

	return 0;
}

