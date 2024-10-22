#include "cfgutil.h"

#include <stddef.h>
#include <string.h>

#include <libehbcfw/syscall.h>

#include "debug.h"

#define TERM_COL    80
#define TERM_ROW    25

#define FG_BLACK    0x00
#define FG_BLUE     0x01
#define FG_GREEN    0x02
#define FG_CYAN     0x03
#define FG_RED      0x04
#define FG_MAGENTA  0x05
#define FG_BROWN    0x06
#define FG_WHITE    0x07
#define FG_BBLACK   0x08
#define FG_BBLUE    0x09
#define FG_BGREEN   0x0A
#define FG_BCYAN    0x0B
#define FG_BRED     0x0C
#define FG_BMAGENTA 0x0D
#define FG_BYELLOW  0x0E
#define FG_BWHITE   0x0F
#define BG_BLACK    0x00
#define BG_BLUE     0x10
#define BG_GREEN    0x20
#define BG_CYAN     0x30
#define BG_RED      0x40
#define BG_MAGENTA  0x50
#define BG_BROWN    0x60
#define BG_WHITE    0x70

extern const void _binary_splash_bin_start;
extern const unsigned long _binary_splash_bin_size;

void show_splash(void)
{
    ehbcfw_video_set_cursor_shape(2, 0x2607);

    ehbcfw_video_scroll_area(2, 0, FG_WHITE | BG_BLACK, 0, TERM_ROW - 2, 0, TERM_COL - 1);    
    ehbcfw_video_scroll_area(2, 0, FG_BLACK | BG_WHITE, TERM_ROW - 1, TERM_ROW - 1, 0, TERM_COL - 1);

    ehbcfw_video_set_cursor_pos(2, TERM_COL * (TERM_ROW - 1));
    ehbcfw_video_write_string(2, " DEL/F2: RUN SETUP  F9: SYSTEM INFORMATION  F12: BOOT MENU", 80, VA_AUTO);

    const uint16_t *cur = &_binary_splash_bin_start;
    for (int i = 0; i < 80 * 24; i++) {
        ehbcfw_video_set_cursor_pos(2, i);
        ehbcfw_video_write_char_attr(2, cur[i] >> 8, cur[i] & 0xFF);
    }

    ehbcfw_video_set_cursor_pos(2, 0);
    ehbcfw_video_set_cursor_shape(2, 0x0C0F);

    return;
}
