#ifndef DRIVER_SERIAL_MC68681_H__
#define DRIVER_SERIAL_MC68681_H__

#include <ehbcfw/types.h>
#include <ehbcfw/device.h>

#include "driver/serial/serial.h"

#define MR1_RXRTR_ENABLE        0x80
#define MR1_RXIRQ_RXRDY         0x00
#define MR1_RXIRQ_FFULL         0x40
#define MR1_ERROR_CHAR          0x00
#define MR1_ERROR_BLOCK         0x20
#define MR1_PARITY_ENABLE       0x00
#define MR1_PARITY_FORCE        0x08
#define MR1_PARITY_DISABLE      0x10
#define MR1_PARITY_MULTIDROP    0x18
#define MR1_PARITY_ODD          0x04
#define MR1_PARITY_EVEN         0x00
#define MR1_BPC_5               0x00
#define MR1_BPC_6               0x01
#define MR1_BPC_7               0x02
#define MR1_BPC_8               0x03

#define MR2_MODE_NORMAL         0x00
#define MR2_MODE_AUTOECHO       0x40
#define MR2_MODE_LOCALLOOP      0x80
#define MR2_MODE_REMOTELOOP     0xC0
#define MR2_TXRTS_ENABLED       0x20
#define MR2_TXCTS_ENABLED       0x10
#define MR2_STOPBIT_0           0x00
#define MR2_STOPBIT_1           0x04
#define MR2_STOPBIT_1_5         0x08
#define MR2_STOPBIT_2           0x0C

enum mc68901_baud {
    BAUD_50 = 0,
    BAUD_110 = 1,
    BAUD_134_5 = 2,
    BAUD_200 = 3,
    BAUD_300 = 4,
    BAUD_600 = 5,
    BAUD_1200 = 6,
    BAUD_1050 = 7,
    BAUD_2000 = 7,
    BAUD_2400 = 8,
    BAUD_4800 = 9,
    BAUD_7200 = 10,
    BAUD_1800 = 10,
    BAUD_9600 = 11,
    BAUD_384000 = 12,
    BAUD_192000 = 12,
    BAUD_TIMER = 13,
    BAUD_IP16X = 14,
    BAUD_IP1X = 15,
};

enum mc68901_misc_cmd {
    MISCCMD_RST_MR = 1,
    MISCCMD_RST_RX = 2,
    MISCCMD_RST_TX = 3,
    MISCCMD_RST_ERROR = 4,
    MISCCMD_RST_BCINT = 5,
    MISCCMD_START_BREAK = 6,
    MISCCMD_STOP_BREAK = 7,
};

enum mc68901_tx_cmd {
    TXCMD_ENABLE = 1,
    TXCMD_DISABLE = 2,
};

enum mc68901_rx_cmd {
    RXCMD_ENABLE = 1,
    RXCMD_DISABLE = 2,
};

struct serial_mc68901 {
    struct serial_device dev;

    unsigned int reg_alignment;
    hwregu8_t* reg_base;
};

#endif
