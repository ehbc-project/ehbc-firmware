#ifndef __M68K_PROTO1_SCU_H__
#define __M68K_PROTO1_SCU_H__

#include <assert.h>

#include "types.h"

struct scu_regs {
    hwreg8_t ccr;
    hwreg8_t dcr;
    hwreg8_t fcr;
    hwreg8_t pcr;
    hwreg8_t ider[2];
    hwreg8_t isar;
    hwreg8_t reserved0;
    hwreg8_t abr[8];
    union {
        hwreg8_t icr[12];
        struct {
            hwreg8_t isr[3];
            hwreg8_t iar[7];
            hwreg8_t reserved1;
        } isr_iar;
    };
};

static_assert(sizeof(struct scu_regs) == 28, "Invalid SCU MMIO register struct size");

void scu_power_off(void);

int scu_hpt_init(int channel, int mode, uint16_t data);
int scu_hpt_start(int channel);
int scu_hpt_stop(int channel);
int scu_hpt_reset(int channel);

#endif // __M68K_PROTO1_SCU_H__
