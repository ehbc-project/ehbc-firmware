#ifndef __DRIVER_SERIAL_MC68901_H__
#define __DRIVER_SERIAL_MC68901_H__

#include <ehbcfw/types.h>
#include <ehbcfw/device.h>

#include "driver/char/chardev.h"

#define MC68901_SCR 19
#define MC68901_UCR 20
#define MC68901_RSR 21
#define MC68901_TSR 22
#define MC68901_UDR 23

#define UCR_CLK_DIV16       0x80
#define UCR_BPC_5           0x00
#define UCR_BPC_6           0x20
#define UCR_BPC_7           0x40
#define UCR_BPC_8           0x60
#define UCR_STOPBIT_0       0x00
#define UCR_STOPBIT_1       0x08
#define UCR_STOPBIT_1_5     0x10
#define UCR_STOPBIT_2       0x18
#define UCR_PARITY_ENABLE   0x04
#define UCR_PARITY_ODD      0x00
#define UCR_PARITY_EVEN     0x02

#define RSR_BUFFER_FULL     0x80
#define RSR_OVERRUN_ERROR   0x40
#define RSR_PARITY_ERROR    0x20
#define RSR_FRAME_ERROR     0x10
#define RSR_FOUND_SEARCH    0x08
#define RSR_BREAK           0x08
#define RSR_MATCH           0x04
#define RSR_CIP             0x04
#define RSR_SYNC_STRIP      0x02
#define RSR_RX_ENABLED      0x01

#define TSR_BUFFER_EMPTY    0x80
#define TSR_UNDERRUN_ERROR  0x40
#define TSR_AUTO_TURNAROUND 0x20
#define TSR_END_OF_TRANSMIT 0x10
#define TSR_BREAK           0x08
#define TSR_IDLE_HIGHZ      0x00
#define TSR_IDLE_LOW        0x02
#define TSR_IDLE_HIGH       0x04
#define TSR_IDLE_LOOPBACK   0x06
#define TSR_TX_ENABLED      0x01

#endif
