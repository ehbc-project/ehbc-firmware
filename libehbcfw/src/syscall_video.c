#include "libehbcfw/syscall.h"

int ehbcfw_video_set_mode(int id, int mode)
{
    return __syscall1(2, 0, (id & 0xFF) | (mode << 8));
}

void ehbcfw_video_set_cursor_shape(int id, uint16_t shape)
{
    __syscall1(2, 3, (id & 0xFF) | (shape << 8));
}

uint16_t ehbcfw_video_get_cursor_shape(int id)
{
    return __syscall1(2, 4, id & 0xFF);
}

void ehbcfw_video_set_cursor_pos(int id, uint16_t pos)
{
    __syscall1(2, 5, (id & 0xFF) | (pos << 8));
}

uint16_t ehbcfw_video_get_cursor_pos(int id)
{
    return __syscall1(2, 6, id & 0xFF);
}

void ehbcfw_video_scroll_area(int id, int count, int attr, int top, int bottom, int left, int right)
{
    __syscall3(2, 7, (id & 0xFF) | ((count & 0xFFFF) << 8), attr, ((top & 0xFF) << 24) | ((bottom & 0xFF) << 16) | ((left & 0xFF) << 8) | (right & 0xFF));
}


void ehbcfw_video_write_char_attr(int id, int ch, int attr)
{
    __syscall1(2, 9, (id & 0xFF) | ((ch & 0xFF) << 8) | ((attr & 0xFF) << 16));
}

void ehbcfw_video_write_string(int id, const char *str, unsigned int len, int attr)
{
    __syscall3(2, 10, (id & 0xFF) | (len << 8), attr, (long)str);
}

void ehbcfw_video_write_ansi_tty(int id, const char *str, unsigned int len)
{
    __syscall2(2, 12, (id & 0xFF) | (len << 8), (long)str);
}
