#ifndef EHBCFW_DEVICE_H__
#define EHBCFW_DEVICE_H__

#include <ehbcfw/types.h>
#include <ehbcfw/linked_list.h>

struct device;

struct driver_operations {
    int (*init)(struct device* device);
    int (*probe)(struct device* device);
};

ll_define_type(struct device_driver, {
    const char* name;
    const char* const* compatible_devices;

    const struct driver_operations* ops;
});

ll_define_type(struct device, {
    int id;
    struct device_driver* driver;
    void* device_private;
});

void register_driver(struct device_driver* driver);
int attach_driver(struct device* device, const char* driver_name);

#define DECLARE_DRIVER(driver)                      \
    __attribute__((constructor))                    \
    static void __decl_driver_ ## __DATE__(void) {  \
        register_driver((struct device_driver*)&(driver));                 \
    }

void register_device(struct device* device);
struct device* get_device(const char* id);

#endif
