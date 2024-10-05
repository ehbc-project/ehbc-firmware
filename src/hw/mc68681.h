#ifndef HW_MC68681_H__
#define HW_MC68681_H__

#include "types.h"

#include <libehbcfw/syscall.h>

int mc68681_init();
int mc68681_set_param(
    int ch, enum ehbcfw_aio_baud txbaud, enum ehbcfw_aio_baud rxbaud,
    int bpc,
    enum ehbcfw_aio_parity_mode pmode,
    enum ehbcfw_aio_stop_bits sbmode);
void mc68681_enable(int ch, int txenable, int rxenable);
void mc68681_disable(int ch);

void mc68681_irq_handler(void);

int mc68681_tx_polled(int ch, uint8_t chr);
int mc68681_tx(int ch, const char* str);
int mc68681_txn(int ch, const char* str, unsigned long len);

int mc68681_rx_polled(int ch);

int mc68681_read_byte(int ch);
void mc68681_write_byte(int ch, uint8_t chr);

#endif  // HW_MC68681_H__
