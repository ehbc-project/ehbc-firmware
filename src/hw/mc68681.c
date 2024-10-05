/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/mc68681.h"

#include "ringbuf.h"

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
static struct ringbuf8 txbuf, rxbuf;
static uint8_t txbuf_data[128], rxbuf_data[128];

static const uint8_t baud_configs[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0A, 0x07, 0x08, 0x09, 0x0B, 0x0C
};

int mc68681_init()
{
    ringbuf8_init(&txbuf, sizeof(txbuf_data), txbuf_data);
    ringbuf8_init(&rxbuf, sizeof(rxbuf_data), rxbuf_data);
    return 0;
}

int mc68681_set_param(
    int ch, enum ehbcfw_aio_baud txbaud, enum ehbcfw_aio_baud rxbaud,
    int bpc,
    enum ehbcfw_aio_parity_mode pmode,
    enum ehbcfw_aio_stop_bits sbmode) {
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
        case AIO_PM_NONE:
            val |= 0x10;
            break;
        case AIO_PM_ODD:
            val |= 0x04;
            break;
        case AIO_PM_EVEN:
            break;
        default:
            return 1;
    }
    *mr = val;

    val = 0x30;
    switch (sbmode) {
        case AIO_SB_1:
            val |= 0x04;
            break;
        case AIO_SB_1_5:
            val |= 0x08;
            break;
        case AIO_SB_2:
            val |= 0x0C;
            break;
        default:
            return 1;
    }
    *mr = val;

    val = baud_configs[rxbaud];
    val <<= 4;
    val |= baud_configs[txbaud];
    *csr = val;
    mc68681->acr = 0x80;

    mc68681->imr = 0x2 << (ch ? 4 : 0);  // irq when rxrdy

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

void mc68681_irq_handler(void)
{
    if (mc68681->isr & 0x01) {  // txrdya
        uint8_t data;
        if (ringbuf8_read(&txbuf, &data)) {  // if there's no data in txbuf
            mc68681->imr = 0x02;  // mask txrdya
        } else {
            mc68681_tx_polled(0, data);  // else transmit
        }
    }

    if (mc68681->isr & 0x02) {  // rxrdya
        // TODO: handle underflow
        ringbuf8_write(&rxbuf, mc68681_rx_polled(0));
    }
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
        mc68681->imr = 0x03;
        while (!ringbuf8_write(&txbuf, *str) && *str) { str++; }
    }
    return 0;
}

int mc68681_txn(int ch, const char* str, unsigned long len)
{
    int ret;
    while (len) {
        mc68681->imr = 0x03;
        while (!ringbuf8_write(&txbuf, *str) && len) {
            str++;
            len--;
        }
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

int mc68681_read_byte(int ch)
{
    uint8_t data;
    while (ringbuf8_read(&rxbuf, &data)) {}
    return data;
}

void mc68681_write_byte(int ch, uint8_t chr)
{
    mc68681->imr = 0x03;  // unmask txrdya
    while (ringbuf8_write(&txbuf, chr)) {}
    mc68681_irq_handler();  // is it ok?
}

