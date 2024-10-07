#include <stdint.h>
#include <stddef.h>

#include "device.h"
#include "hw/vga.h"

struct syscall_args {
    long d[5];
};

static long syscall_handler_aio(const struct syscall_args *args)
{
    struct device *dev = find_device(args->d[1] & 0xFF);
    if (dev->class != DC_AIO) return 0;

    switch (args->d[0] & 0xFFFF) {
        case 1:
        case 2:
            return dev->aio_ops.wait_send(dev, (args->d[1] >> 8) & 0xFF);
        case 3:
        case 4:
            return dev->aio_ops.wait_recv(dev);
        default:
            break;
    }
    return 0;
}

static long syscall_handler_video(const struct syscall_args *args)
{
    struct device *dev = find_device(args->d[1] & 0xFF);
    if (dev->class != DC_VIDEO) return 0;

    switch (args->d[0] & 0xFFFF) {
        case 0:
            return vga_set_mode(dev, (args->d[1] >> 8) & 0xFFFF);
        case 3:
            dev->video_ops.set_cursor_shape(dev, (args->d[1] >> 8) & 0xFFFF);
            return 0;
        case 4:
            return dev->video_ops.get_cursor_shape(dev);
        case 5:
            dev->video_ops.set_cursor_pos(dev, (args->d[1] >> 8) & 0xFFFF);
            return 0;
        case 6:
            return dev->video_ops.get_cursor_pos(dev);
        case 10:
            dev->video_ops.write_string(dev, (const char*)args->d[3], (args->d[1] >> 8) & 0xFFFF, args->d[2]);
            return 0;
        case 12:
            dev->video_ops.write_ansi_tty(dev, (const char*)args->d[2], (args->d[1] >> 8) & 0xFFFF);
            return 0;
        default:
            break;
    }
    return 0;
}

long syscall_handler(struct syscall_args args) {
    switch ((args.d[0] >> 16) & 0xFFFF) {
        case 0:
            break;
        case 1:
            return syscall_handler_aio(&args);
        case 2:
            return syscall_handler_video(&args);
        case 3:
        case 4:
        case 5:
        case 6:
        default:
            break;
    }
    return 0;
}
