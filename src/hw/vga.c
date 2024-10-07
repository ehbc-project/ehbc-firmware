/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/vga.h"

#include <string.h>

#include "hw/vgaregs.h"
#include "io.h"

#define VGA_MEMORY_BASE     0xFE0A0000

struct video_mode {
    int mode;
    struct video_mode_info info;
    uint8_t miscreg;
    const uint8_t *palette;
    int palette_len;
    const uint8_t *sequencer;  // register 0 is skipped
    const uint8_t *crtc;
    const uint8_t *graphics;
    const uint8_t *attribute;
};

#define END_OF_VMODE_TABLE { -1, { 0, }, 0, NULL, 0, NULL, NULL, NULL, NULL }
#define IS_VMODE_TABLE_END(vmode) ((vmode)->mode < 0)

#include "hw/vgapalette.i"

#define SR_LEN 4
#define CR_LEN 25
#define AR_LEN 20
#define GR_LEN 9

#include "hw/stdvgamodes.i"

static const struct video_mode *current_vmode;

extern const void _binary_VGA8_F16_start;
extern const void _binary_VGA8_F16_end;
extern const unsigned long _binary_VGA8_F16_size;

const char *vga_get_name(struct device *dev)
{
    return "VGA / VGA Compatible";
}

const char *vga_get_vendor(struct device *dev)
{
    return "IBM";
}

static void screen_off()
{
    vga_out_sr(0x01, vga_in_sr(0x01) | 0x20);
}

static void screen_on()
{
    vga_out_sr(0x01, vga_in_sr(0x01) & 0xDF);
    io_read_8(VGA_ISR1);
    io_write_8(VGA_AR_AD, 0x20);
}

static void enable_font_access()
{
    vga_out_sr(0x00, 0x01);
    vga_out_sr(0x02, 0x04);
    vga_out_sr(0x04, 0x07);
    vga_out_sr(0x00, 0x03);
    vga_out_gr(0x04, 0x02);
    vga_out_gr(0x05, 0x00);
    vga_out_gr(0x06, 0x04);
}

static void disable_font_access()
{
    vga_out_sr(0x00, 0x01);
    vga_out_sr(0x02, 0x03);
    vga_out_sr(0x04, 0x03);
    vga_out_sr(0x00, 0x03);
    vga_out_gr(0x06, (vga_in_misc() & 0x01) ? 0x0e : 0x0a);
    vga_out_gr(0x04, 0x00);
    vga_out_gr(0x05, 0x10);
}

static void load_font(const void* fontdata, int glyph_cnt, int glyph_size)
{
    enable_font_access();
    uint8_t *dest = (void*)VGA_MEMORY_BASE;
    const uint8_t *src = fontdata;
    for (int i = 0; i < glyph_cnt; i++) {
        memcpy(dest, src, glyph_size);
        dest += 32;
        src += glyph_size;
    }
    disable_font_access();
}

static void clear_screen(const struct video_mode *mode)
{
    uint16_t* fbuf_base = (uint16_t*)mode->info.buf_base;
        
    switch (mode->info.mmodel) {
    case MM_TEXT:
        memset16(fbuf_base, 0x0007, 0x10000);
        break;
    case MM_CGA:
        memset16(fbuf_base, 0x0000, 0x10000);
        break;
    default:
        memset16(fbuf_base, 0x0000, 0x20000);
    }
}

static const struct video_mode *vga_find_mode(int mode)
{
    for (int i = 0; !IS_VMODE_TABLE_END(&vga_modes[i]); i++) {
        if (mode == vga_modes[i].mode) {
            return &vga_modes[i];
        }
    }
    return NULL;
}

const struct video_mode_info *vga_get_mode_info(struct device *dev, int mode)
{
    return &vga_find_mode(mode)->info;
}

void vga_load_packed_palette()
{
    vga_dac_write(0, palette3, sizeof(palette3) / 3);
}

int vga_set_mode(struct device *dev, int modenum)
{
    const struct video_mode *mode = vga_find_mode(modenum);
    if (!mode) return 1;

    // disable video
    screen_off();

    // set palette mask
    io_write_8(VGA_PALMASK, 0xFF);

    // load palette
    int palsize = mode->palette_len / 3;
    vga_dac_write(0, mode->palette, palsize);
    for (int i = palsize; i < 256; i++) {
        static const uint8_t blankrgb[3] = { 0, };
        vga_dac_write(i, blankrgb,  1);
    }

    const uint8_t* data;
    
    // load attribute registers
    data = mode->attribute;
    for (int i = 0; i < 20; i++) {
        vga_out_ar(i, data[i]);
    }
    vga_out_ar(0x14, 0x00);

    // load sequencer registers
    data = mode->sequencer;
    vga_out_sr(0x00, 0x01);
    for (int i = 0; i < 4; i++) {
        vga_out_sr(i + 1, data[i]);
    }

    // load graphics registers
    data = mode->graphics;
    for (int i = 0; i < 9; i++) {
        vga_out_gr(i, data[i]);
    }

    // load crtc registers
    data = mode->crtc;
    vga_out_cr(0x11, 0x00, !(mode->miscreg & 1));
    for (int i = 0; i < 25; i++) {
        vga_out_cr(i, data[i], !(mode->miscreg & 1));
    }

    // set misc register
    vga_out_misc(mode->miscreg);

    // load font if text mode
    if (mode->info.mmodel == MM_TEXT) {
        load_font((void*)&_binary_VGA8_F16_start, 256, 16);
    }

    // clear screen
    clear_screen(mode);
 
    // turn on screen
    screen_on();

    current_vmode = mode;

    return 0;
}

int vga_get_mode(struct device *dev)
{
    return current_vmode->mode;
}

void vga_set_cursor_shape(struct device *dev, uint16_t shape)
{
    vga_out_cr(0x0A, shape >> 8, 0);
    vga_out_cr(0x0B, shape, 0);
}

uint16_t vga_get_cursor_shape(struct device *dev)
{
    return (vga_in_cr(0x0A, 0) << 8) | vga_in_cr(0x0B, 0);
}

void vga_set_cursor_pos(struct device *dev, uint16_t pos)
{
    vga_out_cr(0x0E, pos >> 8, 0);
    vga_out_cr(0x0F, pos, 0);
}

uint16_t vga_get_cursor_pos(struct device *dev)
{
    return (vga_in_cr(0x0E, 0) << 8) | vga_in_cr(0x0F, 0);
}

void vga_select_plane(int plane)
{
    if (plane < 0) {
        vga_out_sr(0x02, 0x0f);
        vga_out_gr(0x04, 0);
    } else {
        vga_out_sr(0x02, 1 << plane);
        vga_out_gr(0x04, plane);
    }
}

int vga_probe(struct device *dev)
{
    return 0;
}

int vga_reset(struct device *dev)
{
    vga_out_misc(0xC3);
    vga_out_sr(0x04, 0x02);

    return 0;
}

void vga_write_string(struct device *dev, const char *str, unsigned int len, int attribute)
{
    if (current_vmode->info.mmodel != MM_TEXT) return;

    uint16_t cur = vga_get_cursor_pos(dev);
    uint8_t *ptr = (uint8_t*)current_vmode->info.buf_base + (cur << 1);
    int i;
    for (i = 0; i < len && *str; i++) {
        *ptr++ = *str++;
        if (attribute < 0) {
            ptr++;
        } else {
            *ptr++ = attribute;
        }
    }
    vga_set_cursor_pos(dev, cur + i);
}

void vga_write_string_with_attr(struct device *dev, const uint16_t* str, unsigned int len)
{
    if (current_vmode->info.mmodel != MM_TEXT) return;

    uint16_t cur = vga_get_cursor_pos(dev);
    uint16_t *ptr = (uint16_t*)current_vmode->info.buf_base + cur;
    int i;
    for (i = 0; i < len && *str; i++) {
        *ptr++ = *str++;
    }
    vga_set_cursor_pos(dev, cur + i);
}

void vga_write_ansi_tty(struct device *dev, const char* str, unsigned int len)
{
    if (current_vmode->info.mmodel != MM_TEXT) return;

    uint16_t cur = vga_get_cursor_pos(dev);
    uint16_t *ptr = (uint16_t*)current_vmode->info.buf_base;
    int screen_width = current_vmode->info.width;
    int screen_height = current_vmode->info.height;
    
    while (*str) {
        switch (*str) {
            case '\r': {
                cur -= cur % screen_width;
            } break;
            case '\n': {
                if (cur / screen_width < screen_height - 1) {
                    cur += screen_width;
                } else {
                    vga_scroll_area(dev, 1, -1, 0, screen_height - 1, 0, screen_width - 1);
                }
            } break;
            case '\b': {
                if (cur % screen_width) {
                    cur--;
                }
            } break;
            case '\t': {
                cur &= ~0x07;
                cur += 0x08;
            } break;
            default: {
                uint16_t tmp = ptr[cur];
                tmp &= 0x00FF;
                tmp |= *str << 8;
                ptr[cur] = tmp;
                cur++;
            }
        }
        str++;
    }

    vga_set_cursor_pos(dev, cur);
}

void vga_scroll_area(struct device *dev, int amount, int attr, int top, int bottom, int left, int right)
{
    if (current_vmode->info.mmodel != MM_TEXT) return;

    uint16_t *ptr = (uint16_t*)current_vmode->info.buf_base;
    int screen_width = current_vmode->info.width;
    int screen_height = current_vmode->info.height;

    int clear_start, clear_end;
    
    if (amount > 0) {
        for (int i = top + amount; i <= bottom; i++) {
            memcpy(ptr + (i - amount) * screen_width + left, ptr + i * screen_width + left, (right - left + 1) << 1);
        }
        clear_start = bottom - amount + 1;
        clear_end = bottom;
    } else if (amount < 0) {
        for (int i = bottom + amount; i >= top; i--) {
            memcpy(ptr + (i - amount) * screen_width + left, ptr + i * screen_width + left, (right - left + 1) << 1);
        }
        clear_start = top;
        clear_end = top - amount - 1;
    } else {
        clear_start = top;
        clear_end = bottom;
    }

    for (int i = clear_start; i <= clear_end; i++) {
        for (int j = left; j <= right; j++) {
            if (attr < 0) {
                ptr[i * screen_width + j] &= 0x00FF;
            } else {
                ptr[i * screen_width + j] = attr & 0xFF;
            }
        }
    }

    uint16_t cursor_pos = vga_get_cursor_pos(dev);
    int cursor_row = cursor_pos / screen_width - amount;
    int cursor_col = cursor_pos % screen_width;

    if (top <= cursor_row && cursor_row <= bottom && left <= cursor_col && cursor_col <= right) {
        cursor_pos -= screen_width * amount;
    }

    vga_set_cursor_pos(dev, cursor_pos);
}

void vga_read_char_attr(struct device *dev, int *ch, int *attr)
{

}

void vga_write_char_attr(struct device *dev, int ch, int attr)
{

}
