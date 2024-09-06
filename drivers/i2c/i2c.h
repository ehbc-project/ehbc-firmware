#ifndef __DRIVER_I2C_I2C_H__
#define __DRIVER_I2C_I2C_H__

#include "driver/base/device.h"

struct i2c_bus_controller {
    struct device*              dev;
};

struct i2c_bus_device {
    struct device*              dev;
    struct i2c_bus_controller*  controller;
};

#endif // __DRIVER_I2C_I2C_H__
