#include "types.h"
#include "stat.h"
#include "user.h"
int main(void)
{
	printf(1,"this is a test program for getreadcount\n");
	printf(1,"the times of read is %d\n",getreadcount());
	return 0;
}
