#ifndef _SPINLOCK_H
#define _SPINLOCK_H

typedef struct __spinlock_t {
	int flag;
} spinlock_t;

void spinlock_init(spinlock_t *lock);
void spinlock_acquire(spinlock_t *lock);
void spinlock_release(spinlock_t *lock);

#endif