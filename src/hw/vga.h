#ifndef HW_VGA_H__
#define HW_VGA_H__

#include "types.h"

#include <libehbcfw/device.h>

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

struct device_vga {
    int mode;
};

const char *vga_get_name(struct device *dev);
const char *vga_get_vendor(struct device *dev);

int vga_probe(struct device *dev);

int vga_reset(struct device *dev);

int vga_set_mode(struct device *dev, int mode);
int vga_get_mode(struct device *dev);
const struct video_mode_info *vga_get_mode_info(struct device *dev, int mode);
void vga_set_cursor_shape(struct device *dev, uint16_t shape);
uint16_t vga_get_cursor_shape(struct device *dev);
void vga_set_cursor_pos(struct device *dev, uint16_t pos);
uint16_t vga_get_cursor_pos(struct device *dev);
void vga_scroll_area(struct device *dev, int amount, int attr, int top, int bottom, int left, int right);
void vga_write_string(struct device *dev, const char *str, unsigned int len, int attr);
void vga_write_string_with_attr(struct device *dev, const uint16_t* str, unsigned int len);
void vga_write_ansi_tty(struct device *dev, const char *str, unsigned int len);
void vga_read_char_attr(struct device *dev, int *ch, int *attr);
void vga_write_char_attr(struct device *dev, int ch, int attr);

void vga_select_plane(int plane);
void vga_load_packed_palette();

#endif  // HW_VGA_H__
