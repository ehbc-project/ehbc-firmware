#include <stdint.h>

#include "mmio.h"

void scu_power_off()
{
    mmio_write_8(0x03, 0x80);  // PMCR, power off
}

void scu_change_pclk()
{

}
