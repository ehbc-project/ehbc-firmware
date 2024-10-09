#ifndef __LIBEHBCFW_SYSCALL_H__
#define __LIBEHBCFW_SYSCALL_H__

#include <stdint.h>
#include <time.h>

#include <libehbcfw/asm/syscall.h>
#include <libehbcfw/disk.h>

void ehbcfw_boot_next(void);
int ehbcfw_deinitialize(void);

enum ehbcfw_aio_baud {
    AIO_BAUD_75 = 0,
    AIO_BAUD_110,
    AIO_BAUD_135,
    AIO_BAUD_150,
    AIO_BAUD_300,
    AIO_BAUD_600,
    AIO_BAUD_1200,
    AIO_BAUD_1800,
    AIO_BAUD_2000,
    AIO_BAUD_2400,
    AIO_BAUD_4800,
    AIO_BAUD_9600,
    AIO_BAUD_19200
};

enum ehbcfw_aio_parity_mode {
    AIO_PM_NONE = 0,
    AIO_PM_ODD = 4,
    AIO_PM_EVEN = 5,
    AIO_PM_STICK_ODD = 6,
    AIO_PM_STICK_EVEN = 7,
};

enum ehbcfw_aio_stop_bits {
    AIO_SB_1 = 0,
    AIO_SB_1_5,
    AIO_SB_2,
    AIO_SB_AUTO,
};

struct ehbcfw_aio_param {
    enum ehbcfw_aio_baud baud;
    enum ehbcfw_aio_parity_mode parity_mode;
    enum ehbcfw_aio_stop_bits stop_bits;
    int bpc;
};

int ehbcfw_aio_initialize_port(int id, struct ehbcfw_aio_param param);
int ehbcfw_aio_wait_tx(int id, char ch);
int ehbcfw_aio_tx(int id, char ch);
int ehbcfw_aio_wait_rx(int id, char *buf);
int ehbcfw_aio_rx(int id, char *buf);
int ehbcfw_aio_get_status(int id, int *status);
int ehbcfw_aio_flush_tx(int id);
int ehbcfw_aio_flush_rx(int id);

enum ehbcfw_vmode_type {
    VT_TEXT = 0, VT_CGA, VT_PLANAR, VT_PACKED, VT_DIRECT
};

struct ehbcfw_vmode_param_table {
    int mode;
    enum ehbcfw_vmode_type type;
    int width, height;
    int chr_width, chr_height;
    int bpp;
};

int ehbcfw_video_set_mode(int id, int mode);
int ehbcfw_video_get_mode(int id);
void ehbcfw_video_set_cursor_shape(int id, uint16_t shape);
uint16_t ehbcfw_video_get_cursor_shape(int id);
void ehbcfw_video_set_cursor_pos(int id, uint16_t pos);
uint16_t ehbcfw_video_get_cursor_pos(int id);
void ehbcfw_video_scroll_area(int id, int count, int attr, int top, int bottom, int left, int right);
void ehbcfw_video_read_char_attr(int id, int *ch, int *attr);
void ehbcfw_video_write_char_attr(int id, int ch, int attr);
void ehbcfw_video_write_string(int id, const char *str, unsigned int len, int attr);
void ehbcfw_video_write_string_with_attr(int id, const uint16_t *str, unsigned int len);
void ehbcfw_video_write_ansi_tty(int id, const char *str, unsigned int len);
void ehbcfw_video_load_text_font(int id, void *data);
void ehbcfw_video_load_palette(int id, void *data, unsigned int size);

struct ehbcfw_drive_param_table {
    int id;
};

void ehbcfw_storage_reset(int id);
int ehbcfw_storage_get_status(int id);
const struct ehbcfw_drive_param_table *ehbcfw_storage_get_param_table(int id);
int ehbcfw_storage_read_sectors_chs(int id, struct chs chs, int count, void *buf);
int ehbcfw_storage_write_sectors_chs(int id, struct chs chs, int count, const void *buf);
int ehbcfw_storage_read_sectors_lba(int id, lba_t lba, int count, void *buf);
int ehbcfw_storage_write_sectors_lba(int id, lba_t lba, int count, const void *buf);

time_t ehbcfw_rtc_get_time(int id);

#endif // __LIBEHBCFW_SYSCALL_H__
