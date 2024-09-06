#ifndef DRIVER_BLOCK_BLOCK_H__
#define DRIVER_BLOCK_BLOCK_H__

#include <ehbcfw/types.h>
#include <ehbcfw/device.h>

typedef uint64_t lba_t;

struct block_device;

struct block_asyncread {
    void* buf;
    lba_t lba;
    int count;
    void (*callback)(struct block_asyncread*);
    struct block_device* dev;
    int result;
    int finished;
};

struct block_asyncwrite {
    const void* buf;
    lba_t lba;
    int count;
    void (*callback)(struct block_asyncread*);
    struct block_device* dev;
    int result;
    int finished;
};

struct block_operations {
    int (*read_block)(struct block_device* device, void* buf, lba_t lba, int count);
    int (*write_block)(struct block_device* device, const void* buf, lba_t lba, int count);

    int (*read_block_async)(struct block_device* device, struct block_asyncread* params);
    int (*write_block_async)(struct block_device* device, struct block_asyncwrite* params);

    int (*get_max_lba)(struct block_device* device, lba_t* lba);
    int (*get_block_size)(struct block_device* device, size_t* size);
};

struct block_device {
    struct device dev;
    const struct block_operations* ops;
};

#define BLOCK_DEVICE(dev) ((struct block_device*)(dev))

#endif
