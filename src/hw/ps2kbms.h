#ifndef HW_I8042_H__
#define HW_I8042_H__

#include <libehbcfw/device.h>

#include "types.h"
#include "ringbuf.h"
#include "spinlock.h"

struct device_ps2kbms {
    struct ringbuf8 *kbbuf, *msbuf;
    uint16_t kbd_state;
    uint8_t i8042_ctr;
    uint8_t ms_pkt_idx;
    uint8_t ms_pkt_temp[4];
    uint8_t ms_pkt[4];
    struct spinlock_irq ms_pkt_lock;
};

void ps2kbms_irq_handler(struct device *dev);

const char *ps2kb_get_name(struct device *dev);
const char *ps2kb_get_vendor(struct device *dev);

const char *ps2ms_get_name(struct device *dev);
const char *ps2ms_get_vendor(struct device *dev);

int ps2kb_probe(struct device *dev);
int ps2kb_reset(struct device *dev);

int ps2ms_probe(struct device *dev);
int ps2ms_reset(struct device *dev);

int ps2kb_get_key_char(struct device *dev);
int ps2kb_get_keystroke(struct device *dev);
int ps2kb_get_keyboard_flags(struct device *dev);
void ps2kb_flush_buffer(struct device *dev);

int ps2ms_get_status(struct device *dev, uint8_t *button, int *dx, int *dy);

#endif  // HW_I8042_H__
