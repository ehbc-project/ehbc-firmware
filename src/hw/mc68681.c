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
};

static const uint8_t baud_configs[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0A, 0x07, 0x08, 0x09, 0x0B, 0x0C
};

const char *mc68681_get_name(struct device *dev)
{
    return "MC68681";
}

const char *mc68681_get_vendor(struct device *dev)
{
    return "Motorola";
}

int mc68681_probe(struct device *dev)
{
    return 0;
}

static int mc68681_tx_polled(struct mc68681_regs *regs, int ch, uint8_t chr)
{
    uint8_t status;

    do {
        status = !ch ? regs->sra : regs->srb;
    } while (!(status & 0x0C));

    if (!ch) {
        regs->tba = chr;
    } else {
        regs->tbb = chr;
    }

    return !!(status & 0x70);
}

static int mc68681_rx_polled(struct mc68681_regs *regs, int ch)
{
    uint8_t status;

    do {
        status = !ch ? regs->sra : regs->srb;
    } while (!(status & 0x01));

    if (!!(status & 0x70)) return -1;

    return !ch ? regs->rba : regs->rbb;
}

void mc68681_irq_handler(struct device *dev)
{
    struct device_mc68681 *param = dev->param;
    struct mc68681_regs *regs = param->mmio_base;

    if (regs->isr & 0x01) {  // txrdya
        uint8_t data;
        if (ringbuf8_read(param->cha_txbuf, &data)) {
            // if there's no data in txbuf
            param->imr &= ~0x01;  // unmask txrdya
            regs->imr = param->imr;
        } else {
            mc68681_tx_polled(regs, 0, data);  // else transmit
        }
    }

    if (regs->isr & 0x02) {  // rxrdya
        // TODO: handle underflow
        ringbuf8_write(param->cha_rxbuf, mc68681_rx_polled(regs, 0));
    }

    if (regs->isr & 0x10) {  // txrdyb
        uint8_t data;
        if (ringbuf8_read(param->chb_txbuf, &data)) {
            // if there's no data in txbuf
            param->imr &= ~0x10;  // unmask txrdyb
            regs->imr = param->imr;
        } else {
            mc68681_tx_polled(regs, 1, data);  // else transmit
        }
    }

    if (regs->isr & 0x20) {  // rxrdyb
        // TODO: handle underflow
        ringbuf8_write(param->chb_rxbuf, mc68681_rx_polled(regs, 1));
    }
}

int mc68681_cha_reset(struct device *dev)
{
    struct device_mc68681 *param = dev->param;
    struct mc68681_regs *regs = param->mmio_base;

    mc68681_cha_flushtx(dev);
    mc68681_cha_flushrx(dev);

    regs->cra = 0x0A;  // disable tx, rx
    regs->cra = 0x20;  // reset rx
    regs->cra = 0x30;  // reset tx
    regs->cra = 0x40;  // reset error
    regs->cra = 0x50;  // reset interrupt
    regs->cra = 0x70;  // stop break
    regs->cra = 0x10;  // reset mr pointer

    param->imr &= 0xF;  // mask irq

    return 0;
}

int mc68681_chb_reset(struct device *dev)
{
    struct device_mc68681 *param = dev->param;
    struct mc68681_regs *regs = param->mmio_base;

    mc68681_chb_flushtx(dev);
    mc68681_chb_flushrx(dev);

    regs->crb = 0x0A;  // disable tx, rx
    regs->crb = 0x20;  // reset rx
    regs->crb = 0x30;  // reset tx
    regs->crb = 0x40;  // reset error
    regs->crb = 0x50;  // reset interrupt
    regs->crb = 0x70;  // stop break
    regs->crb = 0x10;  // reset mr pointer

    param->imr &= 0xF0;  // mask irq

    return 0;
}

static int mc68681_set_param_common(struct device *dev, int channel,
    const struct ehbcfw_aio_param *aio_param)
{
    if (aio_param->bpc < 5 || aio_param->bpc > 8) return 1;

    struct device_mc68681 *param = dev->param;
    struct mc68681_regs *regs = param->mmio_base;

    hwreg8_t* mr = !channel ? &regs->mra : &regs->mrb;
    hwreg8_t* csr = !channel ? &regs->csra : &regs->csrb;

    if (channel) {
        mc68681_chb_reset(dev);
    } else {
        mc68681_cha_reset(dev);
    }

    uint8_t val = ((aio_param->bpc - 5) & 0x03);  // bpc
    switch (aio_param->parity_mode) {
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
    *mr = val;  // set mr0

    val = 0x30;  // enable rts, cts
    switch (aio_param->stop_bits) {
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
    *mr = val;  // set mr1

    *csr =
        (baud_configs[aio_param->baud] << 4) |  // rx baud
        baud_configs[aio_param->baud];  // tx baud
    regs->acr = 0x80;  // use baud rate set 1

    param->imr &= 0xF << (channel ? 0 : 4);  // mask irq
    param->imr |= 0x2 << (channel ? 4 : 0);  // irq when rxrdy
    regs->imr = param->imr;

    if (channel) {
        regs->crb = 0x05;  // enable tx, rx
    } else {
        regs->cra = 0x05;  // enable tx, rx
    }

    return 0;
}

int mc68681_cha_set_param(struct device *dev,
    struct ehbcfw_aio_param param)
{
    return mc68681_set_param_common(dev, 0, &param);
}

int mc68681_chb_set_param(struct device *dev,
    struct ehbcfw_aio_param param)
{
    return mc68681_set_param_common(dev, 1, &param);
}

int mc68681_cha_read_byte(struct device *dev)
{
    struct device_mc68681 *param = dev->param;

    uint8_t data;
    while (ringbuf8_read(param->cha_rxbuf, &data)) {}
    return data;
}

int mc68681_chb_read_byte(struct device *dev)
{
    struct device_mc68681 *param = dev->param;

    uint8_t data;
    while (ringbuf8_read(param->chb_rxbuf, &data)) {}
    return data;
}

int mc68681_cha_write_byte(struct device *dev, uint8_t chr)
{
    struct device_mc68681 *param = dev->param;
    struct mc68681_regs *regs = param->mmio_base;

    param->imr |= 0x01;  // unmask txrdya
    regs->imr = param->imr;
    while (ringbuf8_write(param->cha_txbuf, chr)) {}
    mc68681_irq_handler(dev);  // is it ok?

    return 0;
}

int mc68681_chb_write_byte(struct device *dev, uint8_t chr)
{
    struct device_mc68681 *param = dev->param;
    struct mc68681_regs *regs = param->mmio_base;

    param->imr |= 0x10;  // unmask txrdyb
    regs->imr = param->imr;
    while (ringbuf8_write(param->chb_txbuf, chr)) {}
    mc68681_irq_handler(dev);  // is it ok?

    return 0;
}

int mc68681_cha_flushtx(struct device *dev)
{
    struct device_mc68681 *param = dev->param;
    struct mc68681_regs *regs = param->mmio_base;
    ringbuf8_flush(param->cha_txbuf);
    return 0;
}

int mc68681_cha_flushrx(struct device *dev)
{
    struct device_mc68681 *param = dev->param;
    ringbuf8_flush(param->cha_rxbuf);
    return 0;
}

int mc68681_chb_flushtx(struct device *dev)
{
    struct device_mc68681 *param = dev->param;
    ringbuf8_flush(param->chb_txbuf);
    return 0;
}

int mc68681_chb_flushrx(struct device *dev)
{
    struct device_mc68681 *param = dev->param;
    ringbuf8_flush(param->chb_rxbuf);
    return 0;
}
