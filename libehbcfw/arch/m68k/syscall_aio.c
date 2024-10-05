#include "libehbcfw/syscall.h"

int ehbcfw_aio_initialize_port(int port, struct ehbcfw_aio_param param)
{
    __syscall1(1, 0, 0);  // TODO
    return 0;
}

int ehbcfw_aio_wait_tx(int port, char ch)
{
    __syscall1(1, 1, ((port & 0xFF) << 8) | ch);
    return 0;
}

int ehbcfw_aio_tx(int port, char ch)
{
    __syscall1(1, 2, ((port & 0xFF) << 8) | ch);
    return 0;
}

int ehbcfw_aio_tx_string(int port, const char *str, unsigned long len)
{
    __syscall2(1, 6, ((port & 0xFF) << 24) | (len & 0xFFFFFF), (long)str);
    return 0;
}
