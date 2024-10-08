#include "spinlock.h"

extern int get_irqmask_level(void);
extern void set_irqmask_level(int);

void spinlock_init(struct spinlock *lock)
{
    atomic_flag_clear(&lock->flag);
}

void spinlock_lock(struct spinlock *lock)
{
    while (atomic_flag_test_and_set(&lock->flag)) {}
}

void spinlock_unlock(struct spinlock *lock)
{
    atomic_flag_clear(&lock->flag);
}

void spinlock_irq_init(struct spinlock_irq *lock)
{
    atomic_flag_clear(&lock->flag);
    lock->irq_level = 0;
}

void spinlock_irq_lock(struct spinlock_irq *lock)
{
    lock->irq_level = get_irqmask_level();
    set_irqmask_level(-1);
    while (atomic_flag_test_and_set(&lock->flag)) {}
}

void spinlock_irq_unlock(struct spinlock_irq *lock)
{
    atomic_flag_clear(&lock->flag);
    set_irqmask_level(lock->irq_level);
}


