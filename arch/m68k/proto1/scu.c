#include "proto1/scu.h"

void scu_power_off(void)
{
    struct scu_regs *scu = (void*)0xFF000000;
    scu->pcr = 0x80;
}

void scu_change_pclk()
{

}
