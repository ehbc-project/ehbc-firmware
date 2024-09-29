#include "io.h"

void debug_write(const char* str)
{
    for (; *str; str++)
        io_write_8(0xE9, *str);
}
