#include <pthread.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#include "lock.h"
#include "hash.h"
#include "list.h"
#include "counter.h"

struct timeval timer;
//pthread_t *p;
double begin,end;
counter_t *counter;
list_t *list;
hash_t *hash;
int operation,threads;
pthread_t p[20];
void* counter_test(void);
void* list_test(void);
void* hash_test(void);
int main(int argc,char** argv)
{
	int test_type = atoi(argv[1]);
	srand((unsigned)time(0));
	if(LOCK_TYPE == 1) 
		printf("\n----------------Pthread Mutex Lock Test-------------\n");
	else if(LOCK_TYPE == 2)
		printf("\n----------------My Spin Lock Test-------------------\n");
	else 
		printf("\n----------------My Mutex Lock Test------------------\n");
	int i;
	if(test_type == 1)
	{
		for(operation = (int)1e4;operation <= 1e6;operation *= 10)
		{
			printf("\n\n/*operation times : %7d*/\n",operation);
			for(threads = 1;threads <= 20;threads++)
			{
				printf("threads number : %2d   ",threads);
				gettimeofday(&timer,NULL);
				begin = timer.tv_sec+timer.tv_usec/1e6;
				counter = (counter_t*)malloc(sizeof(counter_t));
				counter_init(counter,0);
				//p = (pthread_t*)malloc(sizeof(pthread_t)*threads);
				for(i = 0;i < threads;i++)
					pthread_create(&p[i],NULL,(void*)counter_test,NULL);
				for(i = 0;i < threads;i++)
					pthread_join(p[i],NULL);
				//free(p);
				counter_clear(counter);
				gettimeofday(&timer,NULL);
				end = timer.tv_sec+timer.tv_usec/1e6;
				printf("counter test time : %.6f\n",end-begin);
			}
		}
	}
	
	else if(test_type == 2)
	{
		for(operation = (int)1e4;operation <= 1e6;operation *= 10)
		{
			printf("\n\n/*operation times : %7d*/\n",operation);
			for(threads = 1;threads <= 10;threads++)
			{
				printf("threads number : %2d   ",threads);
				gettimeofday(&timer,NULL);
				begin = timer.tv_sec+timer.tv_usec/1e6;
				
				list = (list_t*)malloc(sizeof(list_t));
				list_init(list);
				//p = (pthread_t*)malloc(sizeof(pthread_t)*threads);
				for(i = 0;i < threads;i++)
					pthread_create(&p[i],NULL,(void*)list_test,NULL);
				for(i = 0;i < threads;i++)
					pthread_join(p[i],NULL);
				//free(p);
				list_clear(list);
				gettimeofday(&timer,NULL);
				end = timer.tv_sec+timer.tv_usec/1e6;
				printf("list test time    : %.6f\n",end-begin);
			}
		}
	}
			
	else if(test_type == 3)
	{
		for(operation = (int)1e4;operation <= 1e6;operation *= 10)
		{
			printf("\n\n/*operation times : %7d*/\n",operation);
			printf("threads number : %d   ",threads);
			for(threads = 1;threads <= 10;threads++)
			{
				printf("threads number : %2d   ",threads);
				gettimeofday(&timer,NULL);
				begin = timer.tv_sec+timer.tv_usec/1e6;
			
				hash = (hash_t*)malloc(sizeof(hash_t));
				hash_init(hash,20);
				//p = (pthread_t*)malloc(sizeof(pthread_t)*threads);
				for(i = 0;i < threads;i++)
					pthread_create(&p[i],NULL,(void*)hash_test,NULL);
				for(i = 0;i < threads;i++)
					pthread_join(p[i],NULL);
				//free(p);
				hash_clear(hash);
				gettimeofday(&timer,NULL);
				end = timer.tv_sec+timer.tv_usec/1e6;
				printf("hash test time    : %.6f\n",end-begin);
			}
		}
	}
			
	return 0;
}
void* counter_test(void)
{
	int i;
	for(i = 0;i < operation;i++)
	{
		int cmd = rand()%3;
		if(cmd == 0) counter_get_value(counter);
		else if(cmd == 1) counter_increment(counter);
		else counter_decrement(counter);
	}
	return NULL;
}

void* list_test(void)
{
	int i;
	for(i = 0;i < operation;i++)
	{
		int cmd = rand()%3;
		if(cmd == 0) list_insert(list,i%1000);
		else if(cmd == 1) list_delete(list,i%1000);
		else list_lookup(list,i%1000);
	}
	return NULL;
}

void* hash_test(void)
{
	int i;
	for(i = 0;i < operation;i++)
	{
		int cmd = rand()%3;
		if(cmd == 0) hash_insert(hash,i%1000);
		else if(cmd == 1) hash_delete(hash,i%1000);
		else hash_lookup(hash,i%1000);
	}
	return NULL;
}
