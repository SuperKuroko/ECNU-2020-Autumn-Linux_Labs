#include "lock.h"
#include <pthread.h>
#include <stdint.h>
void lock_init(void* lock)
{
    #if LOCK_TYPE == P_MUTEX_LOCK
        pthread_mutex_init((pthread_mutex_t*) lock, NULL);
    #elif LOCK_TYPE == MY_SPIN_LOCK
        spinlock_init((spinlock_t*)lock);
    #else
        mutex_init((mutex_t*)lock);
    #endif
}

void lock_acquire(void *lock)
{
    #if LOCK_TYPE == P_MUTEX_LOCK
        pthread_mutex_lock((pthread_mutex_t*)lock);
    #elif LOCK_TYPE == MY_SPIN_LOCK
        spinlock_acquire((spinlock_t*)lock);
    #else
        mutex_acquire((mutex_t*)lock);
    #endif
}

void lock_release(void *lock)
{
	#if LOCK_TYPE == P_MUTEX_LOCK
        pthread_mutex_unlock((pthread_mutex_t*)lock);
    #elif LOCK_TYPE == MY_SPIN_LOCK
        spinlock_release((spinlock_t*)lock);
    #else
        mutex_release((mutex_t*)lock);
    #endif
}
