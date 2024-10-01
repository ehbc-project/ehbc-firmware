#include "assert.h"

#include <debug.h>

void __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function)
{
    debug_printf("%s: %u: %s: Assertion failed: '%s'\n", file, line, function, assertion);

    for (;;) {}
}
