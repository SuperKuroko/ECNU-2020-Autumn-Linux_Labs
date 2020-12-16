#ifndef __LIST_H_
#define __LIST_H_

#include "lock.h"

typedef struct __node_t
{
	unsigned int val;
	struct __node_t *next;
}node_t;

typedef struct __list_t
{
	node_t *head;
    #if LOCK_TYPE == P_MUTEX_LOCK
        pthread_mutex_t lock;
    #elif LOCK_TYPE == MY_SPIN_LOCK
        spinlock_t lock;
    #else
        mutex_t lock;
    #endif
}list_t;

void list_init(list_t *list);
void list_insert(list_t *list, unsigned int key);
void list_delete(list_t *list, unsigned int key);
void *list_lookup(list_t *list, unsigned int key);
void list_clear(list_t *list);

//for testing && debugging
int list_size(list_t *list);
void list_print(list_t *list);
#endif
