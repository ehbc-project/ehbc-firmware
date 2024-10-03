/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/mc68681.h"

struct mc68681_regs {
    hwreg8_t mra;
    union { hwreg8_t sra, csra;};
    hwreg8_t cra;
    union { hwreg8_t rba, tba; };
    union { hwreg8_t ipcr, acr; };
    union { hwreg8_t isr, imr; };
    union { hwreg8_t cur, ctur; };
    union { hwreg8_t clr, ctlr; };
    hwreg8_t mrb;
    union { hwreg8_t srb, csrb; };
    hwreg8_t crb;
    union { hwreg8_t rbb, tbb; };
    hwreg8_t ivr;
    union { hwreg8_t ip, opcr; };
} __attribute__((packed));

static struct mc68681_regs* const mc68681 = (void*)0xFF000200;

int mc68681_init()
{
    return 0;
}

int mc68681_set_param(
    int ch, int txbaud, int rxbaud, int bpc,
    enum parity_mode pmode,
    enum stopbit_mode sbmode) {
    if (bpc < 5 || bpc > 8) return 1;

    hwreg8_t* cr = !ch ? &mc68681->cra : &mc68681->crb;
    hwreg8_t* mr = !ch ? &mc68681->mra : &mc68681->mrb;
    hwreg8_t* csr = !ch ? &mc68681->csra : &mc68681->csrb;

    *cr = 0x0A;
    *cr = 0x20;
    *cr = 0x30;
    *cr = 0x40;
    *cr = 0x50;
    *cr = 0x70;

    *cr = 0x10;

    uint8_t val = ((bpc - 5) & 0x03);
    switch (pmode) {
        case PM_NONE:
            val |= 0x10;
            break;
        case PM_ODD:
            val |= 0x04;
            break;
        case PM_EVEN:
            break;
        default:
            return 1;
    }
    *mr = val;

    val = 0x30;
    switch (sbmode) {
        case SB_1:
            val |= 0x04;
            break;
        case SB_15:
            val |= 0x08;
            break;
        case SB_2:
            val |= 0x0C;
            break;
        default:
            return 1;
    }
    *mr = val;

    val = 0x00;
    switch (rxbaud) {
        case 75:
            break;
        case 110:
            val |= 0x01;
            break;
        case 135:
            val |= 0x02;
            break;
        case 150:
            val |= 0x03;
            break;
        case 300:
            val |= 0x04;
            break;
        case 600:
            val |= 0x05;
            break;
        case 1200:
            val |= 0x06;
            break;
        case 2000:
            val |= 0x07;
            break;
        case 2400:
            val |= 0x08;
            break;
        case 4800:
            val |= 0x09;
            break;
        case 1800:
            val |= 0x0A;
            break;
        case 9600:
            val |= 0x0B;
            break;
        case 19200:
            val |= 0x0C;
            break;
        default:
            return 1;
    }
    val <<= 4;
    switch (txbaud) {
        case 75:
            break;
        case 110:
            val |= 0x01;
            break;
        case 135:
            val |= 0x02;
            break;
        case 150:
            val |= 0x03;
            break;
        case 300:
            val |= 0x04;
            break;
        case 600:
            val |= 0x05;
            break;
        case 1200:
            val |= 0x06;
            break;
        case 2000:
            val |= 0x07;
            break;
        case 2400:
            val |= 0x08;
            break;
        case 4800:
            val |= 0x09;
            break;
        case 1800:
            val |= 0x0A;
            break;
        case 9600:
            val |= 0x0B;
            break;
        case 19200:
            val |= 0x0C;
            break;
        default:
            return 1;
    }
    *csr = val;
    mc68681->acr = 0x80;

    return 0;
}

void mc68681_enable(int ch, int txenable, int rxenable)
{
    hwreg8_t* cr = !ch ? &mc68681->cra : &mc68681->crb;
    *cr = (1 << (txenable ? 2 : 3)) | (1 << (rxenable ? 0 : 1));
}

void mc68681_disable(int ch)
{
    mc68681_enable(ch, 0, 0);
}

int mc68681_tx_polled(int ch, uint8_t chr)
{
    uint8_t status;

    do {
        status = !ch ? mc68681->sra : mc68681->srb;
    } while (!(status & 0x0C));

    if (!ch) {
        mc68681->tba = chr;
    } else {
        mc68681->tbb = chr;
    }

    return !!(status & 0x70);
}

int mc68681_tx(int ch, const char* str)
{
    int ret;
    while (*str) {
        ret = mc68681_tx_polled(ch, (uint8_t)*str++);
        if (ret) return ret;
    }
    return 0;
}

int mc68681_txn(int ch, const char* str, unsigned long len)
{
    int ret;
    for (; (len > 0) && *str; len--) {
        ret = mc68681_tx_polled(ch, (uint8_t)*str++);
        if (ret) return ret;
    }
    return 0;
}

int mc68681_rx_polled(int ch)
{
    uint8_t status;

    do {
        status = !ch ? mc68681->sra : mc68681->srb;
    } while (!(status & 0x01));

    if (!!(status & 0x70)) return -1;

    return !ch ? mc68681->rba : mc68681->rbb;
}

