#include "libehbcfw/syscall.h"

int ehbcfw_kbd_get_char(int id)
{
    return __syscall1(3, 0, id);
}

int ehbcfw_kbd_wait_get_keystroke(int id)
{
    return __syscall1(3, 1, id);
}

int ehbcfw_kbd_get_keystroke(int id)
{
    return __syscall1(3, 2, id);
}
