#ifndef _MUTEX_H
#define _MUTEX_H

typedef struct __mutex_t {
	int flag;
} mutex_t;

void mutex_init(mutex_t *lock);
void mutex_acquire(mutex_t *lock);
void mutex_release(mutex_t *lock);

#endif