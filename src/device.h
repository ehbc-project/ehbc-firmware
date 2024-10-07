#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <libehbcfw/device.h>

struct device *get_device_list_head(void);

struct device *find_device(int id);
int add_device(struct device *dev);

#endif // __DEVICE_H__
