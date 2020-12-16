#ifndef _COUNTER_H
#define _COUNTER_H

#include "lock.h"

typedef struct __counter_t {
	int val;
	#if LOCK_TYPE == P_MUTEX_LOCK
        pthread_mutex_t lock;
    #elif LOCK_TYPE == MY_SPIN_LOCK
        spinlock_t lock;
    #else
        mutex_t lock;
    #endif
}counter_t;

void counter_init(counter_t *c, int val);
int counter_get_value(counter_t *c);
void counter_increment(counter_t *c);
void counter_decrement(counter_t *c);

//for testing
void counter_clear(counter_t* c); 
#endif
