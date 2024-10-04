#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "arch-syscall.h"

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

int ehbcfw_aio_initialize_port(int port, struct ehbcfw_aio_param param);
int ehbcfw_aio_wait_tx(int port, char ch);
int ehbcfw_aio_tx(int port, char ch);
int ehbcfw_aio_wait_rx(int port, char *buf);
int ehbcfw_aio_rx(int port, char *buf);
int ehbcfw_aio_get_status(int port, int *status);

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

const struct ehbcfw_vmode_param_table *ehbcfw_video_set_mode(int mode);
const struct ehbcfw_vmode_param_table *ehbcfw_video_get_param_table(int mode);
const struct ehbcfw_vmode_param_table *ehbcfw_video_get_status(void);
void ehbcfw_video_set_cursor_shape(int shape);
void ehbcfw_video_set_cursor_pos(int row, int col);
void ehbcfw_video_get_cursor(int *shape, int *row, int *col);
void ehbcfw_video_scroll_area(int count, int attr, int upper_row, int lower_row, int left_col, int right_col);
void ehbcfw_video_read_char_attr(int *ch, int *attr);
void ehbcfw_video_write_char_attr(int ch, int attr);
void ehbcfw_video_write_string(const char *str, unsigned int len, int flags);
void ehbcfw_video_write_ansi_tty(const char *str, unsigned int len);
void ehbcfw_video_load_text_font(void *data);
void ehbcfw_video_load_palette(void *data, unsigned int size);

struct ehbcfw_drive_param_table {
    int id;
};

void ehbcfw_storage_reset(int id);
int ehbcfw_storage_get_status(int id);
const struct ehbcfw_drive_param_table *ehbcfw_storage_get_param_table(int id);
int ehbcfw_storage_read_sectors_chs(int id, int head, int cylinder, int sector, int count, void *buf);
int ehbcfw_storage_write_sectors_chs(int id, int head, int cylinder, int sector, int count, const void *buf);
int ehbcfw_storage_read_sectors_lba(int id, long long lba, int count, void *buf);
int ehbcfw_storage_write_sectors_lba(int id, long long lba, int count, const void *buf);

#endif // __SYSCALL_H__
