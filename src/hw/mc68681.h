#ifndef HW_MC68681_H__
#define HW_MC68681_H__

#include "types.h"
#include "ringbuf.h"

#include <libehbcfw/syscall.h>
#include <libehbcfw/device.h>

struct device_mc68681 {
    void *mmio_base;
    struct ringbuf8 *cha_rxbuf, *cha_txbuf;
    struct ringbuf8 *chb_rxbuf, *chb_txbuf;
    uint8_t imr;
};

const char *mc68681_get_name(struct device *dev);
const char *mc68681_get_vendor(struct device *dev);

void mc68681_irq_handler(struct device *dev);

int mc68681_probe(struct device *dev);

int mc68681_cha_reset(struct device *dev);
int mc68681_chb_reset(struct device *dev);

int mc68681_cha_set_param(struct device *dev, struct ehbcfw_aio_param param);
int mc68681_chb_set_param(struct device *dev, struct ehbcfw_aio_param param);

int mc68681_cha_read_byte(struct device *dev);
int mc68681_chb_read_byte(struct device *dev);
int mc68681_cha_write_byte(struct device *dev, uint8_t chr);
int mc68681_chb_write_byte(struct device *dev, uint8_t chr);

#endif  // HW_MC68681_H__
