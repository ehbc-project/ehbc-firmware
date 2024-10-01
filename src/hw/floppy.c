/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/floppy.h"

#include <string.h>
#include <macros.h>

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

int floppy_init(int id, int type)
{
    if (type <= 0 || type >= ARRAY_SIZE(finfo_table)) {
        return 1;
    }

    return 0;
}
