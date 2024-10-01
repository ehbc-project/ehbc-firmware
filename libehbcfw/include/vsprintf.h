#ifndef __VSPRINTF_H__
#define __VSPRINTF_H__

#include <stdarg.h>
#include <stddef.h>

__attribute__((format(printf, 2, 3)))
int sprintf(char *buf, const char *fmt, ...);

__attribute__((format(printf, 3, 4)))
int snprintf(char *buf, size_t size, const char *fmt, ...);

int vsprintf(char *buf, const char *fmt, va_list args);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);

#endif // __VSPRINTF_H__
