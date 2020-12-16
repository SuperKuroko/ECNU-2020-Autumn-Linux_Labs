#ifndef _HASH_H
#define _HASH_H

#include "lock.h"
#include "list.h"

typedef struct __hash_t {
	list_t base[50];
	int size;
} hash_t;

void hash_init(hash_t *hash, int size);
void hash_insert(hash_t *hash, unsigned int key);
void hash_delete(hash_t *hash, unsigned int key);
void *hash_lookup(hash_t *hash, unsigned int key);

//for testing & debugging
int hash_total_size(hash_t *hash);
void hash_clear(hash_t *hash);


#endif
