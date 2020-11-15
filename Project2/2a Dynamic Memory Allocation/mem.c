#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "mem.h"

/*
All Chinese is encoded in UTF-8,
so if Chinese annotation can't be loaded correctly,
please change your change your editor's encoding into UTF-8
*/

typedef struct _node_t 
{
	int size;
	struct _node_t *next;
}node;


const int nodeSize = sizeof(struct _node_t);//node节点的大小
int pageSize;                               //页的大小,通过getpagesize()函数获得
int m_error;                                //头文件中的extern int m_error,表示错误类型
int once = 0;                               //标志位，表示mem_init()被成功调用的次数
int sumSize;                                //整个块的大小
node* _head_,*_tail_;                       //整个块的首地址与末尾地址
int mem_init(int size_of_region)
{
	m_error = 0;
	if(size_of_region <= 0 || once == 1) //参数错误或再次被调用
	{
		m_error = E_BAD_ARGS;
		return -1;
	}
	pageSize = getpagesize(); //获取页面大小
	
	//首先对size_of_region作向上取整，使得其是pageSize的整数倍，
	if(size_of_region%pageSize != 0)
		size_of_region += pageSize-size_of_region%pageSize;
	sumSize = size_of_region; //赋值给全局变量，以便在其他函数中引用。
	
	int fd = open("/dev/zero",O_RDWR);
	node* head = mmap(NULL,size_of_region,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	if(head == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}
	head->size = sumSize-nodeSize;
	//printf("head->size = %d\n",head->size);
	head->next = NULL;
	_head_ = head;
	once = 1;  //设置标志位
	close(fd);
	return 0;
}

void * mem_alloc(int size, int style)
{
	m_error = 0;
	if(_head_ == NULL) //尚未进行块的初始化
	{
		m_error = E_NO_SPACE;
		return NULL;
	}
	if(size%8 != 0)   //要对8补齐，所以对size向上取整，使其为8的倍数
		size += 8-(size%8);
		
	node* head = _head_,*target = NULL;//target为目标块
	//无论何种算法，首先找到大于size的块
	while(head != NULL)
	{
		if(size <= head->size)
		{
			target = head;
			head = head->next;
			break;
		}
		head = head->next;
	}
	if(target == NULL) //没找到，内存不足
	{
		m_error = E_NO_SPACE;
		return NULL;
	}
	
	if(style == M_BESTFIT) 
	{
		while(head != NULL)
		{
			if(size <= head->size && head->size < target->size) //寻找更小的合法块
				target = head;
			head = head->next;
		}
	}
	else if(style == M_WORSTFIT)  
	{
		while(head != NULL)
		{
			if(head->size > target->size) //寻找更大的块，一定合法
				target = head;
			head = head->next;
		}
	}
	//else if(style == M_FIRSTFIT) do nothing
	
	
	//开新块并插入
	if(target->size-size > nodeSize)  //不会产生零头
	{
		node *originalNext = target->next;   //记录原生的后继节点
		target->next = (void*)target+size+nodeSize;  //连接前两个点
		target->next->size = target->size-(size+nodeSize); //设置新节点的size
		target->next->next = originalNext;  //连接后两个点
	}
	target->size = 0;  //标识为已经占用
	return (void*)target+nodeSize;//返回头部节点的末尾地址:
}

int mem_free(void * ptr)
{
	m_error = 0;
	if(ptr == NULL) return 0;
	node *hptr = (void*)ptr-nodeSize;
	if(_head_ == NULL || hptr->size != 0) //未分配或未初始化
	{
		m_error = E_BAD_POINTER;
		return -1;
	}
	
	node* pre = NULL,*late = _head_;
	while(late != NULL)  //搜索前驱节点
	{
	
		if((void*)late+nodeSize == ptr)  //搜索到了节点
		{
			//此时pre就是前驱节点
			if(late->next != NULL) late->size = (void*)late->next-(void*)late-nodeSize;
			else late->size = (void*)_head_+sumSize-(void*)late-nodeSize; //整个块的末尾地址
			if(pre != NULL && pre->size != 0) //和前驱节点合并
			{
				pre->size += late->size+nodeSize;
				pre->next = late->next;
				late = pre;
			}
			if(late->next != NULL && late->next->size != 0) //和后继节点合并
			{
				late->size += late->next->size+nodeSize;
				late->next = late->next->next;
			}
			return 0;
		}
		pre = late;
		late = late->next;
	}
	m_error = E_BAD_POINTER;
	return -1;
}
void mem_dump()
{
	node* head = _head_;
	printf("----------\n");
	while(head != NULL)
	{
		if(head->size != 0)printf("available:%d\n",head->size);
		else 
		{
			if(head->next != NULL) printf("allocted:%ld\n",(void*)head->next-(void*)head-nodeSize);
			else printf("allocted:%ld\n",(void*)_head_+sumSize-(void*)head-nodeSize);
		}
		head = head->next;
	}
	printf("----------\n\n\n");
	return ;
}

