#ifndef __ASM_GENERIC_MACHDEP_H__
#define __ASM_GENERIC_MACHDEP_H__

#include <libehbcfw/device.h>

void machdep_init_irq(void);

const char *machdep_get_model(void);
const char *machdep_get_processor_name(void);

void machdep_reset(void);
void machdep_halt(void);

struct device *machdep_get_device_list(void);

#endif // __ASM_GENERIC_MACHDEP_H__
