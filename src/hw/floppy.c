/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/floppy.h"

#include <string.h>
#include <macros.h>
#include <asm/io.h>
#include <debug.h>

#define FDC_SRA             0x0
#define FDC_SRB             0x1
#define FDC_DOR             0x2
#define FDC_TDR             0x3
#define FDC_MSR             0x4
#define FDC_DRSR            0x4
#define FDC_FIFO            0x5
#define FDC_DIR             0x7
#define FDC_CCR             0x7

#define FDC_DOR_MOTOR_D     0x80    // Set to turn drive 3's motor ON
#define FDC_DOR_MOTOR_C     0x40    // Set to turn drive 2's motor ON
#define FDC_DOR_MOTOR_B     0x20    // Set to turn drive 1's motor ON
#define FDC_DOR_MOTOR_A     0x10    // Set to turn drive 0's motor ON
#define FDC_DOR_MOTOR_MASK  0xf0    
#define FDC_DOR_IRQ         0x08    // Set to enable IRQs and DMA
#define FDC_DOR_RESET       0x04    // Clear = enter reset mode, Set = normal operation
#define FDC_DOR_DSEL_MASK   0x03    // "Select" drive number for next access

//      command name        irq wait    response     param      opcode
#define CMD_CONFIGURE       (0 << 16) | (0 << 12)  | (3 << 8) | 0x13
#define CMD_DUMPREG         (1 << 16) | (10 << 12) | (0 << 8) | 0x0E
#define CMD_FORMAT_TRACK    (1 << 16) | (7 << 12)  | (5 << 8) | 0x0D
#define CMD_LOCK            (1 << 16) | (1 << 12)  | (0 << 8) | 0x94
#define CMD_UNLOCK          (1 << 16) | (1 << 12)  | (0 << 8) | 0x14
#define CMD_MODE            (1 << 16) | (0 << 12)  | (4 << 8) | 0x01
#define CMD_NSC             (0 << 16) | (1 << 12)  | (0 << 8) | 0x18
#define CMD_PERPENDICULAR   (1 << 16) | (0 << 12)  | (1 << 8) | 0x12
#define CMD_READ_SECTOR     (1 << 16) | (7 << 12)  | (8 << 8) | 0x06
#define CMD_READ_DELETED    (1 << 16) | (7 << 12)  | (8 << 8) | 0x0C
#define CMD_READ_ID         (1 << 16) | (7 << 12)  | (1 << 8) | 0x0A
#define CMD_READ_TRACK      (1 << 16) | (7 << 12)  | (8 << 8) | 0x02
#define CMD_RECALIBRATE     (1 << 16) | (0 << 12)  | (1 << 8) | 0x07
#define CMD_RSEEK_IN        (1 << 16) | (0 << 12)  | (2 << 8) | 0x87
#define CMD_RSEEK_OUT       (1 << 16) | (0 << 12)  | (2 << 8) | 0xC7
#define CMD_CMP_EQ          (1 << 16) | (7 << 12)  | (8 << 8) | 0x11
#define CMD_CMP_EQ_HIGH     (1 << 16) | (7 << 12)  | (8 << 8) | 0x1D
#define CMD_CMP_EQ_LOW      (1 << 16) | (7 << 12)  | (8 << 8) | 0x19
#define CMD_SEEK            (1 << 16) | (0 << 12)  | (2 << 8) | 0x0F
#define CMD_SENSE_STATUS    (1 << 16) | (1 << 12)  | (1 << 8) | 0x04
#define CMD_SENSE_IRQ       (0 << 16) | (2 << 12)  | (0 << 8) | 0x08
#define CMD_SET_TRACK       (1 << 16) | (1 << 12)  | (2 << 8) | 0x21
#define CMD_SPECIFY         (0 << 16) | (0 << 12)  | (2 << 8) | 0x03
#define CMD_VERIFY          (1 << 16) | (7 << 12)  | (8 << 8) | 0x16
#define CMD_VERSION         (0 << 16) | (1 << 12)  | (0 << 8) | 0x10
#define CMD_WRITE_SECTOR    (1 << 16) | (7 << 12)  | (8 << 8) | 0x05
#define CMD_WRITE_DELETED   (1 << 16) | (7 << 12)  | (8 << 8) | 0x09

#define CMD_FLAG_MT         0x80
#define CMD_FLAG_MFM        0x40
#define CMD_FLAG_SK         0x20

#define FLOPPY_SPECIFY1     0xAF    // step rate 12 ms, head unload time 240 ms
#define FLOPPY_SPECIFY2     0x02    // head load time 4 ms, dma used
#define FLOPPY_MOTOR_TICKS  0x25    // motor off delay ~2 s
#define FLOPPY_SECTOR_SIZE  0x02    // 512 bytes per sector
#define FLOPPY_SECTOR_GAP_5_25  0x2A
#define FLOPPY_SECTOR_GAP_3_5   0x1B
#define FLOPPY_FORMAT_GAP_5_25  0x50
#define FLOPPY_FORMAT_GAP_3_5   0x6C
#define FLOPPY_FORMAT_FILTER    0xF6
#define FLOPPY_HEAD_SETTLE  0x0F    // 15 ms
#define FLOPPY_STARTUP_TIME 0x08    // 1 s

static const struct floppy_info finfo_table[] = {
    // Unknown
    { { 0, 0, 0 },   0x00, 0x00 },
    // 1 - 360KB, 5.25" - 2 heads, 40 tracks, 9 sectors
    { { 40, 2, 9 },  FSZ_525, FDR_300K },
    // 2 - 1.2MB, 5.25" - 2 heads, 80 tracks, 15 sectors
    { { 80, 2, 15 }, FSZ_525, FDR_500K },
    // 3 - 720KB, 3.5"  - 2 heads, 80 tracks, 9 sectors
    { { 80, 2, 9 },  FSZ_35,  FDR_250K },
    // 4 - 1.44MB, 3.5" - 2 heads, 80 tracks, 18 sectors
    { { 80, 2, 18 }, FSZ_35,  FDR_500K },
    // 5 - 2.88MB, 3.5" - 2 heads, 80 tracks, 36 sectors
    { { 80, 2, 36 }, FSZ_35,  FDR_1M },
    // 6 - 160k, 5.25"  - 1 heads, 40 tracks, 8 sectors
    { { 40, 1, 8 },  FSZ_525, FDR_250K },
    // 7 - 180k, 5.25"  - 1 heads, 40 tracks, 9 sectors
    { { 40, 1, 9 },  FSZ_525, FDR_300K },
    // 8 - 320k, 5.25"  - 2 heads, 40 tracks, 8 sectors
    { { 40, 2, 8 },  FSZ_525, FDR_250K },
};

__attribute__((packed))
struct diskette_param_table {
    uint8_t specify1;
    uint8_t specify2;
    uint8_t motor_off_delay;
    uint8_t bytes_per_sector;
    uint8_t sectors_per_track;
    uint8_t sector_gap;
    uint8_t data_length;
    uint8_t format_gap;
    uint8_t format_filter;
    uint8_t head_settle_time;
    uint8_t motor_start_time;
    uint8_t max_track_num;
    uint8_t data_rate;
    uint8_t drive_type;
};

static struct diskette_param_table param_525dd9 = {
    .specify1 = FLOPPY_SPECIFY1,
    .specify2 = FLOPPY_SPECIFY2,
    .motor_off_delay = FLOPPY_MOTOR_TICKS,
    .bytes_per_sector = FLOPPY_SECTOR_SIZE,
    .sectors_per_track = 9,
    .sector_gap = FLOPPY_SECTOR_GAP_5_25,
    .format_gap = FLOPPY_FORMAT_GAP_5_25,
    .format_filter = FLOPPY_FORMAT_FILTER,
    .head_settle_time = FLOPPY_HEAD_SETTLE,
    .motor_start_time = FLOPPY_STARTUP_TIME,
    .max_track_num = 39,
    .data_rate = 0,
    .drive_type = 1
};

static struct diskette_param_table param_525hd = {
    .specify1 = FLOPPY_SPECIFY1,
    .specify2 = FLOPPY_SPECIFY2,
    .motor_off_delay = FLOPPY_MOTOR_TICKS,
    .bytes_per_sector = FLOPPY_SECTOR_SIZE,
    .sectors_per_track = 15,
    .sector_gap = FLOPPY_SECTOR_GAP_5_25,
    .format_gap = FLOPPY_FORMAT_GAP_5_25,
    .format_filter = FLOPPY_FORMAT_FILTER,
    .head_settle_time = FLOPPY_HEAD_SETTLE,
    .motor_start_time = FLOPPY_STARTUP_TIME,
    .max_track_num = 79,
    .data_rate = 0,
    .drive_type = 2
};

static struct diskette_param_table param_35dd = {
    .specify1 = FLOPPY_SPECIFY1,
    .specify2 = FLOPPY_SPECIFY2,
    .motor_off_delay = FLOPPY_MOTOR_TICKS,
    .bytes_per_sector = FLOPPY_SECTOR_SIZE,
    .sectors_per_track = 9,
    .sector_gap = FLOPPY_SECTOR_GAP_3_5,
    .format_gap = FLOPPY_FORMAT_GAP_3_5,
    .format_filter = FLOPPY_FORMAT_FILTER,
    .head_settle_time = FLOPPY_HEAD_SETTLE,
    .motor_start_time = FLOPPY_STARTUP_TIME,
    .max_track_num = 79,
    .data_rate = 0,
    .drive_type = 3
};

static struct diskette_param_table param_35hd = {
    .specify1 = FLOPPY_SPECIFY1,
    .specify2 = FLOPPY_SPECIFY2,
    .motor_off_delay = FLOPPY_MOTOR_TICKS,
    .bytes_per_sector = FLOPPY_SECTOR_SIZE,
    .sectors_per_track = 18,
    .sector_gap = FLOPPY_SECTOR_GAP_3_5,
    .format_gap = FLOPPY_FORMAT_GAP_3_5,
    .format_filter = FLOPPY_FORMAT_FILTER,
    .head_settle_time = FLOPPY_HEAD_SETTLE,
    .motor_start_time = FLOPPY_STARTUP_TIME,
    .max_track_num = 79,
    .data_rate = 0,
    .drive_type = 4
};

static struct diskette_param_table param_35ed = {
    .specify1 = FLOPPY_SPECIFY1,
    .specify2 = FLOPPY_SPECIFY2,
    .motor_off_delay = FLOPPY_MOTOR_TICKS,
    .bytes_per_sector = FLOPPY_SECTOR_SIZE,
    .sectors_per_track = 36,
    .sector_gap = FLOPPY_SECTOR_GAP_3_5,
    .format_gap = FLOPPY_FORMAT_GAP_3_5,
    .format_filter = FLOPPY_FORMAT_FILTER,
    .head_settle_time = FLOPPY_HEAD_SETTLE,
    .motor_start_time = FLOPPY_STARTUP_TIME,
    .max_track_num = 79,
    .data_rate = 0,
    .drive_type = 5
};

static struct diskette_param_table param_525dd8 = {
    .specify1 = FLOPPY_SPECIFY1,
    .specify2 = FLOPPY_SPECIFY2,
    .motor_off_delay = FLOPPY_MOTOR_TICKS,
    .bytes_per_sector = FLOPPY_SECTOR_SIZE,
    .sectors_per_track = 8,
    .sector_gap = FLOPPY_SECTOR_GAP_5_25,
    .format_gap = FLOPPY_FORMAT_GAP_5_25,
    .format_filter = FLOPPY_FORMAT_FILTER,
    .head_settle_time = FLOPPY_HEAD_SETTLE,
    .motor_start_time = FLOPPY_STARTUP_TIME,
    .max_track_num = 39,
    .data_rate = 0,
    .drive_type = 6
};

struct fdc_command_block {
    uint8_t param_size;
    uint8_t response_size;
    uint8_t wait_irq;
    uint8_t command;
    uint8_t data[10];
};

static void make_command_block(struct fdc_command_block *block, uint32_t cmd)
{
    block->command = cmd & 0xFF;
    block->param_size = (cmd >> 8) & 0xF;
    block->response_size = (cmd >> 12) & 0xF;
    block->wait_irq = (cmd & (1 << 16)) ? 1 : 0;
}

const char *floppy_get_name(struct device *dev)
{
    return "Floppy Drive";
}

const char *floppy_get_vendor(struct device *dev)
{
    return "Unknown";
}

void floppy_irq_handler(struct device *dev)
{
    struct device_fdc *param = dev->param;

    param->irq_done = 1;
}

// low-level fdc i/o functions

#define FDC_IO_READ(dev, reg) io_read_8(((struct device_fdc*)(dev)->param)->port + (reg))
#define FDC_IO_WRITE(dev, reg, data) io_write_8(((struct device_fdc*)(dev)->param)->port + (reg), data)

static uint8_t fdc_dor_read(struct device *dev)
{
    return FDC_IO_READ(dev, FDC_DOR);
}

static void fdc_dor_write(struct device *dev, uint8_t val)
{
    debug_printf("dor write: %02X\n", val);
    FDC_IO_WRITE(dev, FDC_DOR, val);
}

static void fdc_dor_mask(struct device *dev, uint8_t off, uint8_t on)
{
    fdc_dor_write(dev, (fdc_dor_read(dev) & ~off) | on);
}

static void fdc_disable(struct device *dev)
{
    fdc_dor_mask(dev, FDC_DOR_IRQ | FDC_DOR_RESET, 0);
}

static void fdc_wait_irq(struct device *dev)
{
    struct device_fdc *param = dev->param;
    while (!param->irq_done) {}
    param->irq_done = 0;
}

static int fdc_pio_command(struct device *dev, struct fdc_command_block *cmd)
{
    const uint8_t *cmddata = &cmd->command;
    uint8_t status;

    debug_write("fdc pio command:");

    // send command and parameter
    for (int i = 0; i < cmd->param_size + 1; i++) {
        // wait until ready
        while (!(FDC_IO_READ(dev, FDC_MSR) & 0x80)) {}

        status = FDC_IO_READ(dev, FDC_MSR);
        if (status & 0x40) {  // fdc error
            fdc_disable(dev);
            return 1;
        }

        debug_printf(" %02X", *cmddata);

        FDC_IO_WRITE(dev, FDC_FIFO, *cmddata++);
    }

    if (cmd->wait_irq) {
        fdc_wait_irq(dev);
    }

    debug_write("\nresponse:");
    // receive response data
    for (int i = 0; i < cmd->response_size; i++) {
        // wait until ready
        while (!(FDC_IO_READ(dev, FDC_MSR) & 0x80)) {}

        status = FDC_IO_READ(dev, FDC_MSR);
        if (!(status & 0x40)) {  // fdc error
            debug_printf("\nstatus: %02X\n", status);
            fdc_disable(dev);
            return 1;
        }
        cmd->data[i] = FDC_IO_READ(dev, FDC_FIFO);
        debug_printf(" %02X", cmd->data[i]);
    }

    status = FDC_IO_READ(dev, FDC_MSR);
    debug_printf("\nstatus: %02X\n", status);
    if (status & 0x40) {
        fdc_disable(dev);
        return 1;
    }

    return 0;
}

static int fdc_enable(struct device *dev)
{
    fdc_dor_mask(dev, FDC_DOR_RESET, FDC_DOR_IRQ);

    // usleep(4);  // we don't have usleep

    fdc_dor_mask(dev, 0, FDC_DOR_IRQ | FDC_DOR_RESET);

    fdc_wait_irq(dev);

    struct fdc_command_block cmd;
    int ret;
    for (int i = 0; i < 4; i++) {
        make_command_block(&cmd, CMD_SENSE_IRQ);
        ret = fdc_pio_command(dev, &cmd);
        if (ret) return ret;
    }

    return 0;
}

static int floppy_drive_pio_command(struct device *dev, int drive, struct fdc_command_block *cmd)
{
    if (!(fdc_dor_read(dev) & FDC_DOR_RESET)) {
        int ret = fdc_enable(dev);
        if (ret) return ret;
    }

    uint8_t motor_mask = FDC_DOR_MOTOR_A << drive;
    int motor_running = fdc_dor_read(dev) & motor_mask;

    fdc_dor_write(dev, motor_mask | FDC_DOR_IRQ | FDC_DOR_RESET | drive);

    if (!motor_running) {
        // msleep(FLOPPY_STARTUP_TIME * 125);  // we don't have msleep
    }

    int ret = fdc_pio_command(dev, cmd);
    if (ret) return ret;

    if (cmd->wait_irq && cmd->response_size == 0) {
        struct fdc_command_block cmd2;
        make_command_block(&cmd2, CMD_SENSE_IRQ);
        return fdc_pio_command(dev, &cmd2);
    }
    return 0;
}

extern int dmac_setup_floppy_write(const void *buf, int count);
extern int dmac_setup_floppy_read(void *buf, int count);

static int floppy_drive_dma_write_command(struct device *dev, int drive, struct fdc_command_block *cmd, const void *buf, int count)
{
    int ret = dmac_setup_floppy_write(buf, count);
    if (ret) return ret;

    ret = floppy_drive_pio_command(dev, drive, cmd);
    if (ret) return ret;

    if (cmd->data[0] & 0xC0) {
        return 1;
    }

    return 0;
}

static int floppy_drive_dma_read_command(struct device *dev, int drive, struct fdc_command_block *cmd, void *buf, int count)
{
    int ret = dmac_setup_floppy_read(buf, count);
    if (ret) return ret;

    ret = floppy_drive_pio_command(dev, drive, cmd);
    if (ret) return ret;

    if (cmd->data[0] & 0xC0) {
        // return 1;
    }

    return 0;
}

// fdc commands

static int floppy_drive_recalibrate(struct device *dev, int drive)
{
    struct device_fdc *param = dev->param;

    struct fdc_command_block cmd;
    make_command_block(&cmd, CMD_RECALIBRATE);
    cmd.data[0] = drive;
    int ret = floppy_drive_pio_command(dev, drive, &cmd);
    if (ret) return ret;

    param->recalib_status |= 1 << drive;
    param->track[drive] = 0;
    return 0;
}

static int floppy_drive_specify(struct device *dev)
{
    struct fdc_command_block cmd;
    make_command_block(&cmd, CMD_SPECIFY);
    cmd.data[0] = FLOPPY_SPECIFY1;
    cmd.data[1] = FLOPPY_SPECIFY2;
    return fdc_pio_command(dev, &cmd);
}

static int floppy_drive_readid(struct device *dev, int drive, uint8_t data_rate, uint8_t head)
{
    FDC_IO_WRITE(dev, FDC_DIR, data_rate);

    struct fdc_command_block cmd;
    make_command_block(&cmd, CMD_READ_ID);
    cmd.data[0] = (head << 2) | drive;
    int ret = floppy_drive_pio_command(dev, drive, &cmd);
    if (ret) return ret;
    // if (cmd.data[0] & 0xC0) return 1;
    return 0;
}

static int floppy_media_sense(struct device *dev, int drive)
{
    int ret = floppy_drive_readid(dev, 0, 0x00, 0);
    if (ret) return ret;
    return 0;
}

int floppy_prepare(struct device *dev, int drive, int cylinder)
{
    struct device_fdc *param = dev->param;

    if (!(param->recalib_status & (1 << drive))) {
        // recalibrate
        int ret = floppy_drive_recalibrate(dev, drive);
        if (ret) return ret;

        // sense media type
        ret = floppy_media_sense(dev, drive);
        if (ret) return ret;

        // specify drive
        ret = floppy_drive_specify(dev);
        if (ret) return ret;
    }

    // seek to cyl
    if (cylinder != param->track[drive]) {
        struct fdc_command_block cmd;
        make_command_block(&cmd, CMD_SEEK);
        cmd.data[0] = drive;
        cmd.data[1] = cylinder;
        int ret = floppy_drive_pio_command(dev, drive, &cmd);
        if (ret) return ret;
        param->track[drive] = cylinder;
    }

    return 0;
}

// dma commands

static int floppy_read(struct device *dev, int drive, struct chs chs, int count, void *buf)
{
    // chs hack
    int head = chs.cylinder & 1;
    chs.cylinder &= ~1;
    chs.cylinder |= chs.head & 1;
    chs.head = head;

    int ret = floppy_prepare(dev, drive, chs.cylinder);
    if (ret) return -1;

    if (chs.sector + count - 1 > finfo_table[4].chs.sector) {
        count = finfo_table[4].chs.sector - chs.sector + 1;
    }

    // send read-normal-data command to controller
    uint8_t floppyid = drive;
    struct fdc_command_block cmd;
    make_command_block(&cmd, CMD_READ_SECTOR | CMD_FLAG_MFM | CMD_FLAG_MT);
    cmd.data[0] = (chs.head << 2) | drive;
    cmd.data[1] = chs.cylinder;
    cmd.data[2] = chs.head;
    cmd.data[3] = chs.sector;
    cmd.data[4] = FLOPPY_SECTOR_SIZE;
    cmd.data[5] = chs.sector + count - 1;  // last sector to read on track
    cmd.data[6] = FLOPPY_SECTOR_GAP_3_5;
    cmd.data[7] = 0xFF;

    ret = floppy_drive_dma_read_command(dev, drive, &cmd, buf, count * 512);
    if (ret) return -1;

    return count;
}

static int floppy_probe_common(struct device *dev, int drive)
{
    return 0;
}

int floppy_drive0_probe(struct device *dev)
{
    return floppy_probe_common(dev, 0);
}

int floppy_drive1_probe(struct device *dev)
{
    return floppy_probe_common(dev, 1);
}

int floppy_drive2_probe(struct device *dev)
{
    return floppy_probe_common(dev, 2);
}

int floppy_drive3_probe(struct device *dev)
{
    return floppy_probe_common(dev, 3);
}

static int floppy_reset_common(struct device *dev, int drive)
{
    struct device_fdc *param = dev->param;

    param->recalib_status &= ~(1 << drive);
    param->state[drive] = 0;
    param->track[drive] = 0;

    struct fdc_command_block cmd;
    make_command_block(&cmd, CMD_CONFIGURE);
    cmd.data[0] = 0x00;
    cmd.data[1] = 0x20;
    cmd.data[2] = 0x00;
    fdc_pio_command(dev, &cmd);

    fdc_disable(dev);
    return fdc_enable(dev);
}

int floppy_drive0_reset(struct device *dev)
{
    return floppy_reset_common(dev, 0);
}

int floppy_drive1_reset(struct device *dev)
{
    return floppy_reset_common(dev, 1);
}

int floppy_drive2_reset(struct device *dev)
{
    return floppy_reset_common(dev, 2);
}

int floppy_drive3_reset(struct device *dev)
{
    return floppy_reset_common(dev, 3);
}

int floppy_drive0_read_sectors_chs(struct device *dev, struct chs chs, int count, void *buf)
{
    return floppy_read(dev, 0, chs, count, buf);
}

int floppy_drive0_read_sectors_lba(struct device *dev, lba_t lba, int count, void *buf)
{
    return floppy_read(dev, 0, lba_to_chs(lba, &finfo_table[4].chs), count, buf);
}
