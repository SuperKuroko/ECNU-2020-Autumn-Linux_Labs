#include "spinlock.h"
#include "xchg.c"

void spinlock_init(spinlock_t *lock) {
	lock->flag = 0;
}

void spinlock_acquire(spinlock_t *lock) {
	while (xchg(&lock->flag, 1) == 1);
}

void spinlock_release(spinlock_t *lock) {
	lock->flag = 0;
}
