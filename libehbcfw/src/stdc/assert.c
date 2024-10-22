#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

void __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function)
{
    printf("%s: %u: %s: Assertion failed: '%s'\n", file, line, function, assertion);

    exit(EXIT_FAILURE);
}
