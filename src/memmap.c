#include "memmap.h"

static struct memmap *memmap;

void set_memory_map_head(struct memmap *head)
{
    memmap = head;
}

struct memmap *get_memory_map_head(void)
{
    return memmap;
}
