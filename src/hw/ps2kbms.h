#ifndef HW_I8042_H__
#define HW_I8042_H__

#include <libehbcfw/device.h>

#include "types.h"
#include "ringbuf.h"

struct device_ps2kbms {
    struct ringbuf8 *kbbuf, *msbuf;
};

const char *ps2kb_get_name(struct device *dev);
const char *ps2ms_get_name(struct device *dev);
const char *ps2kb_get_vendor(struct device *dev);
const char *ps2ms_get_vendor(struct device *dev);

int ps2kbms_probe(struct device *dev);
int ps2kbms_reset(struct device *dev);

void ps2kbms_poll();
int ps2_kb_command(uint16_t command, uint8_t *param);
int ps2_ms_command(uint16_t command, uint8_t *param);

#endif  // HW_I8042_H__
