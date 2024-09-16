#ifndef HW_MC68681_H__
#define HW_MC68681_H__

#include "types.h"

enum parity_mode {
    PM_NONE = 0, PM_EVEN, PM_ODD
};

enum stopbit_mode {
    SB_1 = 0, SB_15, SB_2
};

int mc68681_init();
int mc68681_set_param(
    int ch, int txbaud, int rxbaud, int bpc,
    enum parity_mode pmode,
    enum stopbit_mode sbmode);
void mc68681_enable(int ch, int txenable, int rxenable);
void mc68681_disable(int ch);

int mc68681_tx_polled(int ch, uint8_t chr);
int mc68681_tx(int ch, const char* str);
int mc68681_txn(int ch, const char* str, unsigned long len);

int mc68681_rx_polled(int ch);

#endif  // HW_MC68681_H__
