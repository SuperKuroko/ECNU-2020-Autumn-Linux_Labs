#ifndef _LOCK_H
#define _LOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <time.h>

#include "spinlock.h"
#include "mutex.h"

#define P_MUTEX_LOCK   1
#define MY_SPIN_LOCK   2
#define MY_MUTEX_LOCK  3

#define LOCK_TYPE      1
  
void lock_init(void* lock);
void lock_acquire(void *lock);
void lock_release(void *lock);

#endif
