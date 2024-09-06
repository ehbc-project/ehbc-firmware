#ifndef __DRIVER_CHAR_SCC_H__
#define __DRIVER_CHAR_SCC_H__


/* WR0 */
#define SCC_NULLCODE        0x00

#define SCC_POINT_HIGH      0x08
#define SCC_RST_EXT_INT     0x10
#define SCC_SEND_ABORT      0x18
#define SCC_INT_NEXT_RX     0x20
#define SCC_RST_TX_INT_PEND 0x28
#define SCC_RST_ERROR       0x30
#define SCC_RST_HIGHEST_IUS 0x38

#define SCC_RST_TX_CRC_CHK  0x40
#define SCC_RST_TX_CRC_GEN  0x80
#define SCC_RST_TX_EOM_LAT  0xC0

/* WR1 */
#define SCC_ENA_WAIT_DRQ    0x80
#define SCC_WAIT_DRQ_SEL    0x40
#define SCC_WAIT_DRQ_ON_RX  0x20

#define SCC_DIS_RX_INT      0x00
#define SCC_RX_INT_1CHR_SC  0x08
#define SCC_RX_INT_CHR_SC   0x10
#define SCC_RX_INT_SC       0x18

#define SCC_PARITY_IS_SC    0x04
#define SCC_ENA_TX_INT      0x02
#define SCC_ENA_EXT_INT     0x01

/* WR3 */
#define SCC_RX_5BPC         0x00
#define SCC_RX_6BPC         0x40
#define SCC_RX_7BPC         0x80
#define SCC_RX_8BPC         0xC0

#define SCC_ENA_AUTO        0x20
#define SCC_ENTER_HUNT      0x10
#define SCC_ENA_RX_CRC      0x08
#define SCC_ADDR_SRCH_MODE  0x04
#define SCC_INHIB_SYNC_CHAR 0x02
#define SCC_ENA_RX          0x01

/* WR4 */
#define SCC_CLK_X1          0x00
#define SCC_CLK_X16         0x40
#define SCC_CLK_X32         0x80
#define SCC_CLK_X64         0xC0

#define SCC_8B_SYNC_CHAR    0x00
#define SCC_16B_SYNC_CHAR   0x10
#define SCC_SDLC_MODE       0x20
#define SCC_EXT_SYNC_MODE   0x30

#define SCC_ENA_SYNC        0x00
#define SCC_STOP_BIT_1      0x04
#define SCC_STOP_BIT_1_5    0x08
#define SCC_STOP_BIT_2      0x0C

#define SCC_EVEN_PARITY     0x02
#define SCC_ENA_PARITY      0x01

/* WR5 */
#define SCC_DTR             0x80

#define SCC_TX_5BPC         0x00
#define SCC_TX_6BPC         0x20
#define SCC_TX_7BPC         0x40
#define SCC_TX_8BPC         0x60

#define SCC_SEND_BREAK      0x10
#define SCC_ENA_TX          0x08
#define SCC_CRC16           0x04
#define SCC_NRTS            0x02
#define SCC_ENA_TX_CRC      0x01




#endif // __DRIVER_CHAR_SCC_H__
