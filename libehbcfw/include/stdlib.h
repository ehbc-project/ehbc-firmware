#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <stddef.h>

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1

#define RAND_MAX        0x7FFF

__attribute__((noreturn))
void exit(int status);

__attribute__((noreturn))
void _Exit(int status);

__attribute__((noreturn))
void quick_exit(int status);

int atexit(void (*func)(void));
int at_quick_exit(void (*func)(void));

double atof(const char *str);
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);
float strtof(const char *str, char **endptr);
double strtod(const char *str, char **endptr);
long double strtold(const char *str, char **endptr);
long strtol(const char *str, char **endptr, int base);
long long strtoll(const char *str, char **endptr, int base);
unsigned long strtoul(const char *str, char **endptr, int base);
unsigned long long strtoull(const char *str, char **endptr, int base);

void qsort(void *base, size_t num, size_t size, int (*cmp)(const void*, const void*));
void *bsearch(const void *key, const void *base, size_t num, size_t size, int (*cmp)(const void*, const void*));

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

typedef struct {
    long long quot;
    long long rem;
} lldiv_t;

int abs(int n);
div_t div(int numer, int denom);
long labs(long n);
ldiv_t ldiv(long numer, long denom);
long long llabs(long long n);
lldiv_t lldiv(long long numer, long long denom);


int rand(void);
void srand(unsigned int seed);

void *malloc(size_t size);
void *calloc(size_t num, size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);

#endif // __STDLIB_H__
