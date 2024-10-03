#ifndef __STDIO_H__
#define __STDIO_H__

# ifndef PRINTF_MAX_LEN
#   define PRINTF_MAX_LEN 1024

# endif

#include <vsprintf.h>

struct _iobuf {
    int fd;
};

typedef struct _iobuf FILE;

extern FILE *stdin, *stdout, *stderr;

__attribute__((format(printf, 1, 2)))
int printf(const char *fmt, ...);

int putchar(int ch);
int puts(const char *str);


__attribute__((format(printf, 2, 3)))
int fprintf(FILE* fp, const char *fmt, ...);

int putc(int ch, FILE* fp);
int fputs(const char *str, FILE* fp);

#endif // __STDIO_H__
