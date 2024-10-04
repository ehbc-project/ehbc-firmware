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
