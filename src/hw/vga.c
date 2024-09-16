/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/vga.h"
#include "hw/vgaregs.h"
#include "memory.h"

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

extern const void _binary_VGA8_F16_start;
extern const void _binary_VGA8_F16_end;
extern const unsigned long _binary_VGA8_F16_size;

static void screen_off()
{
    vga_out_sr(0x01, vga_in_sr(0x01) | 0x20);
}

static void screen_on()
{
    vga_out_sr(0x01, vga_in_sr(0x01) & 0xDF);
    *(hwreg8_t*)VGA_ISR1;
    *(hwreg8_t*)VGA_AR_AD = 0x20;
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

const struct video_mode_info *vga_get_mode_info(int mode)
{
    return &vga_find_mode(mode)->info;
}

void vga_load_packed_palette()
{
    vga_dac_write(0, palette3, sizeof(palette3) / 3);
}

int vga_set_mode(int modenum)
{
    const struct video_mode *mode = vga_find_mode(modenum);
    if (!mode) return 1;

    // disable video
    screen_off();

    // set palette mask
    *(hwreg8_t*)VGA_PALMASK = 0xFF;

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

    return 0;
}

void vga_set_cursor_shape(uint16_t shape)
{
    vga_out_cr(0x0A, shape >> 8, 0);
    vga_out_cr(0x0B, shape, 0);
}

void vga_set_cursor_pos(uint16_t pos)
{
    vga_out_cr(0x0E, pos >> 8, 0);
    vga_out_cr(0x0F, pos, 0);
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

int vga_init()
{
    vga_out_misc(0xC3);
    vga_out_sr(0x04, 0x02);

    return 0;
}
