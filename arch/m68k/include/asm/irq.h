#ifndef __M68K_IRQ_H__
#define __M68K_IRQ_H__

#define IRQ_SPURIOUS        0
#define IRQ_AUTOVECTOR_1    1
#define IRQ_AUTOVECTOR_2    2
#define IRQ_AUTOVECTOR_3    3
#define IRQ_AUTOVECTOR_4    4
#define IRQ_AUTOVECTOR_5    5
#define IRQ_AUTOVECTOR_6    6
#define IRQ_AUTOVECTOR_7    7

unsigned int m68k_irq_init(void);
void m68k_irq_cleanup(void);

#include <asm-generic/irq.h>

#endif // __M68K_IRQ_H__
