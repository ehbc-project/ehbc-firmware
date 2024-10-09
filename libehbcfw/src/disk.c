#include <libehbcfw/disk.h>

const struct chs ata5_geom = { 16383, 16, 63 };

struct chs lba_to_chs(lba_t lba, const struct chs *geom)
{
    struct chs ret;

    ret.sector = lba % geom->sector + 1;
    lba /= geom->sector;
    ret.cylinder = lba % geom->cylinder;
    lba /= geom->cylinder;

    if (lba >= geom->head) {
        // error, head index exceeds its limit
        ret.sector = -1;
        ret.cylinder = -1;
        ret.head = -1;
    } else {
        ret.head = lba;
    }

    return ret;
}

lba_t chs_to_lba(struct chs chs, const struct chs *geom)
{
    if (chs.sector < 1 || chs.sector >= geom->sector) return -1;
    if (chs.cylinder >= geom->cylinder) return -1;
    if (chs.head >= geom->head) return -1;

    lba_t lba = chs.head;
    lba *= geom->cylinder;
    lba += chs.cylinder;
    lba *= geom->sector;
    lba += chs.sector - 1;

    return lba;
}
