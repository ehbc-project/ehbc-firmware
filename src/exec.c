#include "exec.h"

#include <setjmp.h>

static jmp_buf env;

int exec(void *ptr)
{
    int val = setjmp(env);
    if (!val) {
        ((void (*)(void))(long)ptr)();  // jump to address
    }
    return (char)val;
}

void exec_end(char code)
{
    longjmp(env, (int)code | 0x100);  // avoiding the value to be equal to zero
}
