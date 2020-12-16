#include "list.h"
#include "lock.h"

#define LOCK (void*)&list->lock

void list_init(list_t *list)
{
    list->head == NULL;
    lock_init(LOCK);
}

void list_insert(list_t *list, unsigned int key)
{
    lock_acquire(LOCK);
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->val = key;
    node->next = list->head;
    list->head = node;
    lock_release(LOCK);
}

void list_delete(list_t *list, unsigned int key)
{
    if(list->head == NULL) return ; //none list
    lock_acquire(LOCK);
    node_t *pre = NULL,*node = list->head;
    while(node != NULL)
    {
        if(node->val == key) break;
        pre = node;
        node = node->next;
    }
    if(node == NULL) return ; //not exist
    if(pre == NULL) list->head = node->next; //head is target
    else pre->next = node->next; //normal
    free(node);
}

void *list_lookup(list_t *list, unsigned int key)
{
    lock_acquire(LOCK);
    node_t *node = list->head;
    while(node != NULL)
    {
        if(node->val == key) break;
        node = node->next; 
    }
    lock_release(LOCK);
    return node;
}

void list_clear(list_t *list)
{
    node_t *node = list->head;
    node_t *p;
    while(node != NULL)
    {
        p = node;
        node = node->next; 
        free(p);
    }
}

int list_size(list_t *list)
{   
    int size = 0;
    node_t *node = list->head;
    while(node != NULL)
    {
        size++;
        node = node->next; 
    }
    return size;
}

void list_print(list_t *list)
{
    printf("List:\n");
    node_t *node = list->head;
    int index = 0;
    while(node != NULL)
    {
        printf("node %d: %d\n",index++,node->val);
        node = node->next;
    }
}
