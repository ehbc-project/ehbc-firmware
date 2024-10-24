#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdarg.h>
#include <stddef.h>

# ifndef PRINTF_MAX_LEN
#   define PRINTF_MAX_LEN 1024

# endif

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

#define EOF         -1

struct _iobuf {
    int fd;
};

typedef struct _iobuf FILE;

extern FILE *stdin, *stdout, *stderr;

__attribute__((format(printf, 2, 3)))
int sprintf(char *buf, const char *fmt, ...);

__attribute__((format(printf, 3, 4)))
int snprintf(char *buf, size_t size, const char *fmt, ...);

__attribute__((format(printf, 1, 2)))
int printf(const char *fmt, ...);

__attribute__((format(printf, 2, 3)))
int fprintf(FILE *stream, const char *fmt, ...);

int vsprintf(char *buf, const char *fmt, va_list args);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int vprintf(const char *fmt, va_list args);

int putchar(int ch);
int puts(const char *str);

int putc(int ch, FILE* fp);
int fputs(const char *str, FILE* fp);

int fgetc(FILE *stream);
#define getc fgetc
int ungetc(int ch, FILE *stream);

char *fgets(char *str, int num, FILE *stream);
char *gets(char *str);

FILE *fopen(const char *filename, const char *mode);
FILE *freopen(const char *filename, const char *mode, FILE *stream);
int fclose(FILE *stream);
int fflush(FILE *stream);

size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
int fseek(FILE *stream, long offset, int origin);
long ftell(FILE *stream);
int feof(FILE *stream);
void clearerr(FILE *stream);
int ferror(FILE *stream);

void perror(const char *str);

#endif // __STDIO_H__
