#ifndef DRIVER_SCSI_SCSI_H__
#define DRIVER_SCSI_SCSI_H__

#include <ehbcfw/types.h>
#include <ehbcfw/bus.h>
#include <ehbcfw/device.h>

struct scsi_bus {
    
};

struct scsi_device;

struct scsi_operations {
    int (*reset)(struct scsi_device* device);
    int (*get_status)(struct scsi_device* device);
    int (*send_command)(struct scsi_device* device, void* cdb_ptr, size_t cdb_len);
};

struct scsi_driver {
    struct device_driver parent_driver;
    const struct scsi_operations* ops;
};  

struct scsi_device {
    struct device dev;
    const struct scsi_driver* driver;
};

#define SCSI_DEVICE(dev) ((struct scsi_device*)(dev))

#endif
