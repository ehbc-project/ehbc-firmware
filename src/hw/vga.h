#ifndef HW_VGA_H__
#define HW_VGA_H__

#include "types.h"

enum video_memory_model {
    MM_TEXT = 0, MM_CGA, MM_PLANAR, MM_PACKED, MM_DIRECT
};

struct video_mode_info {
    enum video_memory_model mmodel;
    int width, height;
    int bpp;
    int chr_width, chr_height;
    void* buf_base;
};

int vga_init();
int vga_set_mode(int mode);
const struct video_mode_info *vga_get_mode_info(int mode);
void vga_set_cursor_shape(uint16_t shape);
void vga_set_cursor_pos(uint16_t pos);
void vga_select_plane(int plane);
void vga_load_packed_palette();

#endif  // HW_VGA_H__
