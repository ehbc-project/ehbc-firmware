/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/ata.h"
#include "memory.h"
#include "macros.h"

struct ata_ioregs {
    hwreg8_t dr;
    union { hwreg8_t er, fr; };
    hwreg16_t scr;
    hwreg16_t snr;
    hwreg16_t clr;
    hwreg16_t chr;
    hwreg16_t dhr;
    union { hwreg16_t sr, cr; };
};

struct ata_ctrlregs {
    union { hwreg8_t asr, dcr; };
    hwreg8_t dar;
};

// Global defines -- ATA register and register bits.

#define ATA_CB_ASTAT 2   // alternate status in     pio_base_addr2+2
#define ATA_CB_DC    2   // device control      out pio_base_addr2+2
#define ATA_CB_DA    3   // device address   in     pio_base_addr2+3

#define ATA_CB_ER_ICRC 0x80    // ATA Ultra DMA bad CRC
#define ATA_CB_ER_BBK  0x80    // ATA bad block
#define ATA_CB_ER_UNC  0x40    // ATA uncorrected error
#define ATA_CB_ER_MC   0x20    // ATA media change
#define ATA_CB_ER_IDNF 0x10    // ATA id not found
#define ATA_CB_ER_MCR  0x08    // ATA media change request
#define ATA_CB_ER_ABRT 0x04    // ATA command aborted
#define ATA_CB_ER_NTK0 0x02    // ATA track 0 not found
#define ATA_CB_ER_NDAM 0x01    // ATA address mark not found

#define ATA_CB_ER_P_SNSKEY 0xf0   // ATAPI sense key (mask)
#define ATA_CB_ER_P_MCR    0x08   // ATAPI Media Change Request
#define ATA_CB_ER_P_ABRT   0x04   // ATAPI command abort
#define ATA_CB_ER_P_EOM    0x02   // ATAPI End of Media
#define ATA_CB_ER_P_ILI    0x01   // ATAPI Illegal Length Indication

// ATAPI Interrupt Reason bits in the Sector Count reg (CB_SC)
#define ATA_CB_SC_P_TAG    0xf8   // ATAPI tag (mask)
#define ATA_CB_SC_P_REL    0x04   // ATAPI release
#define ATA_CB_SC_P_IO     0x02   // ATAPI I/O
#define ATA_CB_SC_P_CD     0x01   // ATAPI C/D

// bits 7-4 of the device/head (CB_DH) reg
#define ATA_CB_DH_DEV0 0xa0    // select device 0
#define ATA_CB_DH_DEV1 0xb0    // select device 1
#define ATA_CB_DH_LBA 0x40    // use LBA

// status reg (CB_STAT and CB_ASTAT) bits
#define ATA_CB_STAT_BSY  0x80  // busy
#define ATA_CB_STAT_RDY  0x40  // ready
#define ATA_CB_STAT_DF   0x20  // device fault
#define ATA_CB_STAT_WFT  0x20  // write fault (old name)
#define ATA_CB_STAT_SKC  0x10  // seek complete
#define ATA_CB_STAT_SERV 0x10  // service
#define ATA_CB_STAT_DRQ  0x08  // data request
#define ATA_CB_STAT_CORR 0x04  // corrected
#define ATA_CB_STAT_IDX  0x02  // index
#define ATA_CB_STAT_ERR  0x01  // error (ATA)
#define ATA_CB_STAT_CHK  0x01  // check (ATAPI)

// device control reg (CB_DC) bits
#define ATA_CB_DC_HD15   0x08  // bit should always be set to one
#define ATA_CB_DC_SRST   0x04  // soft reset
#define ATA_CB_DC_NIEN   0x02  // disable interrupts

// Most mandtory and optional ATA commands (from ATA-3),
#define ATA_CMD_NOP                          0x00
#define ATA_CMD_CFA_REQUEST_EXT_ERR_CODE     0x03
#define ATA_CMD_DEVICE_RESET                 0x08
#define ATA_CMD_RECALIBRATE                  0x10
#define ATA_CMD_READ_SECTORS                 0x20
#define ATA_CMD_READ_SECTORS_EXT             0x24
#define ATA_CMD_READ_DMA_EXT                 0x25
#define ATA_CMD_READ_DMA_QUEUED_EXT          0x26
#define ATA_CMD_READ_NATIVE_MAX_ADDRESS_EXT  0x27
#define ATA_CMD_READ_MULTIPLE_EXT            0x29
#define ATA_CMD_READ_LOG_EXT                 0x2F
#define ATA_CMD_WRITE_SECTORS                0x30
#define ATA_CMD_WRITE_SECTORS_EXT            0x34
#define ATA_CMD_WRITE_DMA_EXT                0x35
#define ATA_CMD_WRITE_DMA_QUEUED_EXT         0x36
#define ATA_CMD_SET_MAX_ADDRESS_EXT          0x37
#define ATA_CMD_CFA_WRITE_SECTORS_WO_ERASE   0x38
#define ATA_CMD_WRITE_MULTIPLE_EXT           0x39
#define ATA_CMD_WRITE_VERIFY                 0x3C
#define ATA_CMD_WRITE_LOG_EXT                0x3F
#define ATA_CMD_READ_VERIFY_SECTORS          0x40
#define ATA_CMD_READ_VERIFY_SECTORS_EXT      0x42
#define ATA_CMD_FORMAT_TRACK                 0x50
#define ATA_CMD_SEEK                         0x70
#define ATA_CMD_CFA_TRANSLATE_SECTOR         0x87
#define ATA_CMD_EXECUTE_DEVICE_DIAGNOSTIC    0x90
#define ATA_CMD_INITIALIZE_DEVICE_PARAMETERS 0x91
#define ATA_CMD_STANDBY_IMMEDIATE2           0x94
#define ATA_CMD_IDLE_IMMEDIATE2              0x95
#define ATA_CMD_STANDBY2                     0x96
#define ATA_CMD_IDLE2                        0x97
#define ATA_CMD_CHECK_POWER_MODE2            0x98
#define ATA_CMD_SLEEP2                       0x99
#define ATA_CMD_PACKET                       0xA0
#define ATA_CMD_IDENTIFY_PACKET_DEVICE       0xA1
#define ATA_CMD_CFA_ERASE_SECTORS            0xC0
#define ATA_CMD_READ_MULTIPLE                0xC4
#define ATA_CMD_WRITE_MULTIPLE               0xC5
#define ATA_CMD_SET_MULTIPLE_MODE            0xC6
#define ATA_CMD_READ_DMA_QUEUED              0xC7
#define ATA_CMD_READ_DMA                     0xC8
#define ATA_CMD_WRITE_DMA                    0xCA
#define ATA_CMD_WRITE_DMA_QUEUED             0xCC
#define ATA_CMD_CFA_WRITE_MULTIPLE_WO_ERASE  0xCD
#define ATA_CMD_STANDBY_IMMEDIATE            0xE0
#define ATA_CMD_IDLE_IMMEDIATE               0xE1
#define ATA_CMD_STANDBY                      0xE2
#define ATA_CMD_IDLE                         0xE3
#define ATA_CMD_READ_BUFFER                  0xE4
#define ATA_CMD_CHECK_POWER_MODE             0xE5
#define ATA_CMD_SLEEP                        0xE6
#define ATA_CMD_FLUSH_CACHE                  0xE7
#define ATA_CMD_WRITE_BUFFER                 0xE8
#define ATA_CMD_IDENTIFY_DEVICE              0xEC
#define ATA_CMD_SET_FEATURES                 0xEF
#define ATA_CMD_READ_NATIVE_MAX_ADDRESS      0xF8
#define ATA_CMD_SET_MAX                      0xF9

#define ATA_SET_FEATRUE_TRANSFER_MODE        0x03
#define ATA_TRANSFER_MODE_ULTRA_DMA          0x40
#define ATA_TRANSFER_MODE_MULTIWORD_DMA      0x20
#define ATA_TRANSFER_MODE_PIO_FLOW_CTRL      0x08
#define ATA_TRANSFER_MODE_DEFAULT_PIO        0x00

#define IDE_TIMEOUT 1048576

static struct ata_ioregs *ata_io = (void*)0xFE0001F0;
static struct ata_ctrlregs *ata_ctrl = (void*)0xFE0003F6;

static int powerup_await_non_bsy(void)
{
    uint8_t orstatus = 0, status;
    for (int i = 0; i < 4096; i++) {
        status = ata_io->sr;
        if (!(status & ATA_CB_STAT_BSY)) break;
        orstatus |= status;
        if (orstatus == 0xFF) {
            return orstatus;
        }
    }
    return status;
}

static int await_ide(uint8_t mask, uint8_t flags, int timeout)
{
    for (;;) {
        uint8_t status = ata_io->sr;
        if ((status & mask) == flags) return status;
    }
    return -1;
}

static int await_not_bsy()
{
    return await_ide(ATA_CB_STAT_BSY, 0, IDE_TIMEOUT);
}

static int await_rdy()
{
    return await_ide(ATA_CB_STAT_RDY, ATA_CB_STAT_RDY, IDE_TIMEOUT);
}

static inline int pause_await_not_bsy()
{
    ata_io->sr;
    return await_not_bsy();
}

static inline int delay_await_not_bsy()
{
    for (volatile int i = 0; i < 512; i++) {}
    return await_not_bsy();
}

static void reset_channel(int slave)
{
    ata_ctrl->dcr = ATA_CB_DC_HD15 | ATA_CB_DC_NIEN | ATA_CB_DC_SRST;
    for (volatile int i = 0; i < 1024; i++) {}
    ata_ctrl->dcr = ATA_CB_DC_HD15 | ATA_CB_DC_NIEN;
    for (volatile int i = 0; i < 65536; i++) {}

    int status = await_not_bsy();
    if (status < 0) goto finish;

    if (slave) {
        for (int i = 0; i < 8192; i++) {
            ata_io->dhr = ATA_CB_DH_DEV1;
            status = delay_await_not_bsy();
            if (status < 0) goto finish;
            if (ata_io->dhr == ATA_CB_DH_DEV1) break;
            if (i == 8191) goto finish;  // timeout
        }
    } else {
        ata_io->dhr = ATA_CB_DH_DEV0;
    }

    status = await_rdy();

finish:
    ata_ctrl->dcr = ATA_CB_DC_HD15;
}

int ata_identify_device(void *buf, int len);
int ata_read(void *buf, uint64_t lba, uint16_t count);
int ata_write(const void *buf, uint64_t lba, uint16_t count);

int ata_init(void)
{
    int didreset = 0;

    for (int slave = 0; slave <= 1; slave++) {
        // wait for bsy reset
        int status = powerup_await_non_bsy();
        if (status < 0) continue;
        uint8_t newdh = slave ? ATA_CB_DH_DEV1 : ATA_CB_DH_DEV0;
        ata_io->dhr = newdh;
        for (volatile int i = 0; i < 512; i++) {}
        status = powerup_await_non_bsy();
        if (status < 0) continue;

        // check if ioport registers look valid
        ata_io->dhr = newdh;
        uint8_t dh = ata_io->dhr;
        if (dh != newdh) continue;
        ata_io->scr = 0x55;
        if (0x55 != ata_io->scr) continue;
        ata_io->snr = 0xAA;
        if (0xAA != ata_io->snr) continue;

        // reset channel
        if (!didreset) {
            reset_channel(slave);
            didreset = 1;
        }

        // TODO: check for ATAPI
    }
    return 0;
}
