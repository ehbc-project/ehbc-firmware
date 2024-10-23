#include <stdint.h>
#include <stddef.h>

#include "device.h"
#include "exec.h"

struct syscall_args {
    long d[5];
};

static long syscall_handler_boot(const struct syscall_args *args)
{
    switch (args->d[0] & 0xFFFF) {
        case 0:
            exec_end(args->d[1]);
            break;
        default:
            break;
    }
    return 0;
}

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
        case 5:
            return dev->aio_ops.flush_tx(dev);
        case 6:
            return dev->aio_ops.flush_rx(dev);
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
            return dev->video_ops.set_mode(dev, (args->d[1] >> 8) & 0xFFFF);
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
        case 7:
            dev->video_ops.scroll_area(dev, (args->d[1] >> 8) & 0xFFFF, args->d[2], (args->d[3] >> 24) & 0xFF, (args->d[3] >> 16) & 0xFF, (args->d[3] >> 8) & 0xFF, args->d[3] & 0xFF);
            return 0;
        case 9:
            dev->video_ops.write_char_attr(dev, (args->d[1] >> 8) & 0xFF, (args->d[1] >> 16) & 0xFF);
            return 0;
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

static long syscall_handler_inputdev(const struct syscall_args *args)
{
    struct device *dev = find_device(args->d[1] & 0xFF);

    switch (args->d[0] & 0xFFFF) {
        case 0:
            if (dev->class != DC_KEYBOARD) return 0;
            return dev->keyboard_ops.get_char(dev);
        case 1:
            if (dev->class != DC_KEYBOARD) return 0;
            return dev->keyboard_ops.wait_get_stroke(dev);
        case 2:
            if (dev->class != DC_KEYBOARD) return 0;
            return dev->keyboard_ops.get_stroke(dev);
        case 6:
            if (dev->class != DC_MOUSE) return 0;
            return dev->mouse_ops.get_status(dev, (void*)args->d[2], (void*)args->d[3], (void*)args->d[4]);
        default:
            break;
    }
    return 0;
}

static long syscall_handler_storage(const struct syscall_args *args)
{
    struct device *dev = find_device(args->d[1] & 0xFF);
    if (dev->class != DC_STORAGE) return 0;

    switch (args->d[0] & 0xFFFF) {
        case 3: {
            struct chs chs = {
                .cylinder = (args->d[2] >> 6) & 0x3FF,
                .head = (args->d[2] >> 16) & 0xFF,
                .sector = args->d[2] & 0x3F,
            };
            return dev->storage_ops.read_sectors_chs(dev, chs, (args->d[1] >> 8) & 0xFF, (void*)args->d[3]);
        }
        case 6: {
            lba_t lba = (((lba_t)(args->d[1] >> 16) & 0xFFFF) << 32) | args->d[2];
            return dev->storage_ops.read_sectors_lba(dev, lba, (args->d[1] >> 8) & 0xFF, (void*)args->d[3]);
        }
        default:
            break;
    }
    return 0;
}

static long syscall_handler_rtc(const struct syscall_args *args)
{
    struct device *dev = find_device(args->d[1] & 0xFF);
    if (dev->class != DC_RTC) return 0;

    switch (args->d[0] & 0xFFFF) {
        case 0:
            *(time_t*)args->d[2] = dev->rtc_ops.get_time(dev);
            return 0;
        default:
            break;
    }
    return 0;
}

long syscall_handler(struct syscall_args args) {
    switch ((args.d[0] >> 16) & 0xFFFF) {
        case 0:
            return syscall_handler_boot(&args);
        case 1:
            return syscall_handler_aio(&args);
        case 2:
            return syscall_handler_video(&args);
        case 3:
            return syscall_handler_inputdev(&args);
        case 4:
            return syscall_handler_storage(&args);
        case 5:
            return syscall_handler_rtc(&args);
        case 6:
        default:
            break;
    }
    return 0;
}
