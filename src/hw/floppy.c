/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/floppy.h"

#include <string.h>
#include <macros.h>

#define FDC_SRA     0x3F0
#define FDC_SRB     0x3F1
#define FDC_DOR     0x3F2
#define FDC_TDR     0x3F3
#define FDC_MSR     0x3F4
#define FDC_DRSR    0x3F4
#define FDC_FIFO    0x3F5
#define FDC_DIR     0x3F7
#define FDC_CCR     0x3F7

#define CMD_SPECIFY 0x03
#define CMD_WRITE   0x05
#define CMD_READ    0x06
#define CMD_RECALIB 0x07
#define CMD_SENSE   0x08
#define CMD_SEEK    0x09

static const struct floppy_info finfo_table[] = {
    // Unknown
    { { 0, 0, 0 },   0x00, 0x00 },
    // 1 - 360KB, 5.25" - 2 heads, 40 tracks, 9 sectors
    { { 2, 40, 9 },  FSZ_525, FDR_300K },
    // 2 - 1.2MB, 5.25" - 2 heads, 80 tracks, 15 sectors
    { { 2, 80, 15 }, FSZ_525, FDR_500K },
    // 3 - 720KB, 3.5"  - 2 heads, 80 tracks, 9 sectors
    { { 2, 80, 9 },  FSZ_35,  FDR_250K },
    // 4 - 1.44MB, 3.5" - 2 heads, 80 tracks, 18 sectors
    { { 2, 80, 18 }, FSZ_35,  FDR_500K },
    // 5 - 2.88MB, 3.5" - 2 heads, 80 tracks, 36 sectors
    { { 2, 80, 36 }, FSZ_35,  FDR_1M },
    // 6 - 160k, 5.25"  - 1 heads, 40 tracks, 8 sectors
    { { 1, 40, 8 },  FSZ_525, FDR_250K },
    // 7 - 180k, 5.25"  - 1 heads, 40 tracks, 9 sectors
    { { 1, 40, 9 },  FSZ_525, FDR_300K },
    // 8 - 320k, 5.25"  - 2 heads, 40 tracks, 8 sectors
    { { 2, 40, 8 },  FSZ_525, FDR_250K },
};

const char *floppy_get_name(struct device *dev)
{
    return "Floppy Drive";
}

const char *floppy_get_vendor(struct device *dev)
{
    return "Unknown";
}

int floppy_probe_common(struct device *dev, int drive)
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

int floppy_reset_common(struct device *dev, int drive)
{
    return 0;
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

