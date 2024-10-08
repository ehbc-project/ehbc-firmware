#ifndef __LIBEHBCFW_DEVICE_H__
#define __LIBEHBCFW_DEVICE_H__

#include <stdint.h>
#include <time.h>
#include <libehbcfw/syscall.h>

#define DF_AVAILABLE    0x00000001

enum device_class {
    DC_MISC = 0,
    DC_AIO,
    DC_STORAGE,
    DC_VIDEO,
    DC_KEYBOARD,
    DC_MOUSE,
    DC_RTC,
};

struct device;

struct aio_device_ops {
    int (*set_param)(struct device *, struct ehbcfw_aio_param);
    int (*wait_send)(struct device *, uint8_t);
    int (*send)(struct device *, uint8_t);
    int (*wait_recv)(struct device *);
    int (*recv)(struct device *);
    int (*get_status)(struct device *, int *);
    int (*flush_tx)(struct device *);
    int (*flush_rx)(struct device *);
};

struct storage_device_ops {
    int (*get_status)(struct device *);
    int (*read_sectors_chs)(struct device *, struct chs, int, void *);
    int (*write_sectors_chs)(struct device *, struct chs, int, const void *);
    int (*read_sectors_lba)(struct device *, lba_t, int, void *);
    int (*write_sectors_lba)(struct device *, lba_t, int, const void *);
};

struct video_device_ops {
    int (*set_mode)(struct device *, int);
    int (*get_mode)(struct device *);
    void (*set_cursor_shape)(struct device *, uint16_t);
    uint16_t (*get_cursor_shape)(struct device *);
    void (*set_cursor_pos)(struct device *, uint16_t);
    uint16_t (*get_cursor_pos)(struct device *);
    void (*scroll_area)(struct device *, int, int, int, int, int, int);
    void (*read_char_attr)(struct device *, int *, int *);
    void (*write_char_attr)(struct device *, int, int);
    void (*write_string)(struct device *, const char *, unsigned int, int);
    void (*write_string_with_attr)(struct device *, const uint16_t *, unsigned int);
    void (*write_ansi_tty)(struct device *, const char *, unsigned int);
    void (*load_text_font)(struct device *, void *);
    void (*load_palette)(struct device *, void *, unsigned int);
};

struct keyboard_device_ops {
    int (*wait_get_stroke)(struct device *, uint8_t *);
    int (*get_stroke)(struct device *, uint8_t *);
    uint32_t (*get_flags)(struct device *);
    void (*flush_buffer)(struct device *);
};

struct mouse_device_ops {
    int (*set_param)(struct device *);
    uint32_t (*get_status)(struct device *);
    int (*get_movement)(struct device *);
};

struct rtc_device_ops {
    time_t (*get_time)(struct device *);
    void (*set_time)(struct device *, time_t);
    time_t (*get_alarm)(struct device *);
    void (*set_alarm)(struct device *, time_t);
    uint8_t (*read_nvsram)(struct device *, int);
    void (*write_nvsram)(struct device *, int, uint8_t);
};

struct device {
    struct device *next;
    int id;
    enum device_class class;
    uint32_t flags;
    const char *(*get_name)(struct device *);
    const char *(*get_vendor)(struct device *);
    int (*probe)(struct device *);
    int (*reset)(struct device *);
    union {
        struct aio_device_ops aio_ops;
        struct storage_device_ops storage_ops;
        struct video_device_ops video_ops;
        struct keyboard_device_ops keyboard_ops;
        struct mouse_device_ops mouse_ops;
        struct rtc_device_ops rtc_ops;
    };
    void *param;
};

#endif // __LIBEHBCFW_DEVICE_H__
