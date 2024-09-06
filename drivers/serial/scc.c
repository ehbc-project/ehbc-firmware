#include "drivers/serial/scc.h"


__attribute__((constructor))
static int scc_init_driver()
{
    return 0;
}
