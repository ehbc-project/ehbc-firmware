#include "libehbcfw/syscall.h"

int ehbcfw_aio_initialize_port(int id, struct ehbcfw_aio_param param)
{
    __syscall1(1, 0, 0);  // TODO
    return 0;
}

int ehbcfw_aio_wait_tx(int id, char ch)
{
    return __syscall1(1, 1, (ch << 8) | (id & 0xFF));
}

int ehbcfw_aio_tx(int id, char ch)
{
    return __syscall1(1, 2, (ch << 8) | (id & 0xFF));
}

int ehbcfw_aio_wait_rx(int id, char *buf)
{
    long ret = __syscall1(1, 3, id & 0xFF);
    if (ret < 0) {
        return ret;
    } else {
        *buf = ret;
        return 0;
    }
}

int ehbcfw_aio_rx(int id, char *buf)
{
    long ret = __syscall1(1, 4, id & 0xFF);
    if (ret < 0) {
        return ret;
    } else {
        *buf = ret;
        return 0;
    }
}
