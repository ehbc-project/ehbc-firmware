#include "device.h"

#include <stddef.h>

static struct device *device_list_head = NULL;

struct device *get_device_list_head(void)
{
    return device_list_head;
}

struct device *find_device(int id)
{
    struct device *dev = device_list_head;
    while (dev) {
        if (dev->id == id && (dev->flags & DF_AVAILABLE)) {
            return dev;
        } else if (dev->id > id) {
            break;
        }
        dev = dev->next;
    }
    return NULL;
}

int add_device(struct device *dev)
{
    int new_id = 0;
    if (!device_list_head) {
        device_list_head = dev;
    } else {
        struct device *cur = device_list_head;
        while (cur->next) {
            cur = cur->next;
        }
        cur->next = dev;
        new_id = cur->id + 1;
    }

    dev->id = new_id;
    dev->next = NULL;
    
    if (dev->probe) {
        if (dev->probe(dev)) {
            return dev->id;
        }
        dev->flags |= DF_AVAILABLE;
    }

    if (dev->reset) {
        dev->reset(dev);
    }

    return dev->id;
}
