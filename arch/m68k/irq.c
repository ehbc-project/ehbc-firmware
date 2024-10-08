#include <stdint.h>

int get_irqmask_level(void)
{
    uint16_t sr;
    __asm__ volatile ("move.w %%sr,%0\n\t" : "=d"(sr));
    return (sr & 0x0700) >> 8;
}

void set_irqmask_level(int level)
{
    __asm__ volatile (
        "move.w %%sr,%%d1\n\t"
        "andi.w #0xF0FF,%%d1\n\t"
        "or.w %0,%%d1\n\t"
        "move.w %%d1,%%sr" : : "d"((level & 0x7) << 8) : "d1");
}
