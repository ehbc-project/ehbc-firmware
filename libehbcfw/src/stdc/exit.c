#include <stdlib.h>

extern void _exit(int status);

void exit(int status)
{
    _exit(status);

    for (;;) {}  // prevent compiler from complaining about noreturn
}
