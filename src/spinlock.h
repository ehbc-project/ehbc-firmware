#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <stdatomic.h>

struct spinlock {
    atomic_flag flag;
};

struct spinlock_irq {
    atomic_flag flag;
    int irq_level;
};

void spinlock_init(struct spinlock *lock);
void spinlock_lock(struct spinlock *lock);
void spinlock_unlock(struct spinlock *lock);

void spinlock_irq_init(struct spinlock_irq *lock);
void spinlock_irq_lock(struct spinlock_irq *lock);
void spinlock_irq_unlock(struct spinlock_irq *lock);

#endif // __SPINLOCK_H__
