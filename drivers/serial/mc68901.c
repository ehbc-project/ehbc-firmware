#include "driver/char/mc68901.h"

#include "driver/char/chardev.h"

struct mc68901 {
    struct device* dev;
    struct char_device* cdev;
    struct timer_device* tdev;

    unsigned int reg_alignment;
    hwregu8_t* reg_base;
};

static inline hwregu8_t* get_register(struct serial_mc68901* device, unsigned int idx)
{
    return (device->reg_base + (1 << device->reg_alignment) * idx);
}

static int mc68901_reset(struct device* device)
{
    return 1;
}

static int mc68901_set_parameter(struct serial_device* device, const struct serial_params* param)
{
    return 1;
}

static int mc68901_read(struct serial_device* device, void* buf, int len)
{
    struct serial_mc68901* dev = (struct serial_mc68901*)device;

    hwregu8_t* ucr = get_register(dev, MC68901_UCR);



    return 1;
}

static int mc68901_write(struct serial_device* device, const void* buf, int len)
{
    return 1;
}

static const struct serial_operations mc68901_serial_ops = {
    .set_parameter = mc68901_set_parameter,
    .read = mc68901_read,
    .write = mc68901_write,
};

static const char* const compatible_devices[] = {
    "motorola:mc68901",
    NULL,
};

struct serial_driver mc68901_serial_driver = {
    .parent_driver = {
        .name = "mc68901_serial",
        .compatible_devices = compatible_devices,
    },
    .ops = &mc68901_serial_ops,
};

DECLARE_DRIVER(mc68901_serial_driver)
