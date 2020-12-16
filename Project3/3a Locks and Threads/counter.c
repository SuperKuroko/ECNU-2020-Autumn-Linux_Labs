#include "lock.h"
#include "counter.h"

#define LOCK (void*)&c->lock
void counter_init(counter_t *c, int val)
{
    c->val = val;
    lock_init(LOCK);
}

int counter_get_value(counter_t *c)
{
    lock_acquire(LOCK);
    int result = c->val;
    lock_release(LOCK);
    return result;
}

void counter_increment(counter_t *c)
{
    lock_acquire(LOCK);
    c->val++;
    lock_release(LOCK);
}

void counter_decrement(counter_t *c)
{
    lock_acquire(LOCK);
    c->val--;
    lock_release(LOCK);
}

void counter_clear(counter_t* c)
{
    free(c);
}
