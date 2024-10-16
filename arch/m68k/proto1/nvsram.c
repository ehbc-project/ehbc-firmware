#include <stdint.h>
#include <assert.h>

struct nvsram_data {
    uint32_t crc32_inverted;
    uint16_t floppy_types;
    uint8_t cpu_speed;
    uint8_t dram_timing;
    uint8_t rom_timing;
    uint8_t ide_timing;
    uint8_t isa_timing;
    uint32_t reserved0;
    uint8_t irq_avec_level[12];
};

static_assert(sizeof(struct nvsram_data) <= 50, "Invalid NVSRAM data struct size");
