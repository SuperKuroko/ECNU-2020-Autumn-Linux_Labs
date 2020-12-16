#include "lock.h"
#include "list.h"
#include "hash.h"

void hash_init(hash_t *hash, int size) 
{
	hash->size = size;
	int i;
	for (i = 0; i < size; i++) 
		list_init(&hash->base[i]);
}

void hash_insert(hash_t *hash, unsigned int key) {
	int index = key%hash->size;
	list_insert(&hash->base[index], key);
}

void hash_delete(hash_t *hash, unsigned int key) {
	int index = key%hash->size;
	list_delete(&hash->base[index], key);
}

void *hash_lookup(hash_t *hash, unsigned int key) {
	int index = key%hash->size;
	return list_lookup(&hash->base[index], key);
}

int hash_total_size(hash_t *hash) 
{
	int i, result = 0;
	for (i = 0; i < hash->size; i++)
		result += list_size(&hash->base[i]);
	return result;
}

void hash_clear(hash_t *hash) 
{
	int i;
	for (i = 0; i < hash->size; ++i) 
		list_clear(&hash->base[i]);
}
