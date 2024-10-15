#ifndef __M68K_PROTO1_MC68440_H__
#define __M68K_PROTO1_MC68440_H__

#include "types.h"

struct mc68440_channel_regs {
    hwreg8_t csr, cer;
    hwreg16_t reserved0;
    hwreg8_t dcr, ocr;
    hwreg8_t scr, ccr;
    hwreg16_t reserved1;
    hwreg16_t mtcr;
    hwreg32_t mar;
    hwreg32_t reserved2;
    hwreg32_t dar;
    hwreg16_t reserved3;
    hwreg16_t btcr;
    hwreg32_t bar;
    hwreg32_t reserved4;
    hwreg8_t reserved5, nivr;
    hwreg8_t reserved6, eivr;
    hwreg8_t reserved7, mfcr;
    hwreg16_t reserved8;
    hwreg8_t reserved9, cpr;
    hwreg16_t reserved10;
    hwreg8_t reserved11, dfcr;
    hwreg32_t reserved12;
    hwreg16_t reserved13;
    hwreg8_t reserved14, bfcr;
    hwreg32_t reserved15;
    hwreg8_t reserved16, gcr;
} __attribute__((aligned(2)));

// in real, channel 2 and 3 are reserved.
struct mc68440_regs {
    struct mc68440_channel_regs channel[4];
};

#endif // __M68K_PROTO1_MC68440_H__
