#include <stdlib.h>

extern void _exit(void);

void exit(int status)
{
    _exit();
}
