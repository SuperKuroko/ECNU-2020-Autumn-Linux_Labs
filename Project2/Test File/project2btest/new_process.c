#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"
#define check(exp, msg) if(exp) {} else {				\
	printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg); \
	exit();}

int workload(int n) {
    int i, j = 0;
    for(i = 0; i < n; i++)
	j += i * j + 1;
    return j;
}

int
main(int argc, char *argv[])
{
    struct pstat st;
   
    sleep(10);
    int i = workload(80000000), j;

    int pid1;
    int pid2 = getpid();
    if (fork() == 0) {
	pid1 = getpid(); 

	check(getpinfo(&st) == 0, "getpinfo");
	for(i = 0; i < NPROC; i++) {
	    if (st.inuse[i]) {
		if (pid1 == st.pid[i]) {
		    printf(1, "pid: %d priority: %d\n ", st.pid[i], st.priority[i]);
		    for (j = 3; j >= 0; j--)
			printf(1, "\t level %d ticks used %d\n", j, st.ticks[i][j]);  
		    check(st.priority[i] > 0, "new process should have higher priority");
		} else if (pid2 == st.pid[i]) {
		    printf(1, "pid: %d priority: %d\n ", st.pid[i], st.priority[i]);
		    for (j = 3; j >= 0; j--)
			printf(1, "\t level %d ticks used %d\n", j, st.ticks[i][j]);  
		    check(st.priority[i] == 0, "the long CPU-bound process should have lowest priority");
		}
	    }
	}

	printf(1, "TEST PASSED");
    } else {
	wait();
    }

    exit();
}
