#ifndef HW_I8042_H__
#define HW_I8042_H__

#include <libehbcfw/device.h>

#include "types.h"
#include "ringbuf.h"

struct device_ps2kbms {
    struct ringbuf8 *kbbuf, *msbuf;
    uint16_t kbd_state;
    uint16_t mouse_x, mouse_y;
};

void ps2kbms_irq_handler(struct device *dev);

const char *ps2kb_get_name(struct device *dev);
const char *ps2kb_get_vendor(struct device *dev);

const char *ps2ms_get_name(struct device *dev);
const char *ps2ms_get_vendor(struct device *dev);

int ps2kbms_probe(struct device *dev);
int ps2kbms_reset(struct device *dev);

int ps2kb_get_key_char(struct device *dev);
int ps2kb_get_keystroke(struct device *dev);
int ps2kb_get_keyboard_flags(struct device *dev);
void ps2kb_flush_buffer(struct device *dev);

#endif  // HW_I8042_H__
