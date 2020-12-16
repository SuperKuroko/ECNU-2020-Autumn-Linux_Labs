#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

#define check(exp, msg) if(exp) {} else {\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
  exit();}

#define DDEBUG 1

#ifdef DDEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif


int workload(int n) {
  int i, j = 0;
  for (i = 0; i < n; i++) {
    j += i * j + 1;
  }
  return j;
}

int
main(int argc, char *argv[])
{
  struct pstat st;
  check(getpinfo(&st) == 0, "getpinfo");

  // Push this thread to the bottom
  workload(80000000);

  int j;
  int c_pid = fork();
  // Child
    if (c_pid == 0) {
      workload(800000000);
      exit();
    }   
  sleep(400);
  workload(8000000);
  check(getpinfo(&st) == 0, "getpinfo");

  for (j = 0; j < NPROC; j++) {
    if (st.inuse[j] && st.pid[j] == getpid()) {
          
          for (int k = 3; k >= 0; k--) {
            DEBUG_PRINT((1, "\t level %d ticks used %d, ticks waited %d\n", k, st.ticks[j][k], st.wait_ticks[j][k]));
          }
         
          check(getpinfo(&st) == 0, "getpinfo");
	  DEBUG_PRINT((1, "\t The priority of the parent process is %d\n", st.priority[j]));
          check(st.priority[j] > 0, "the priority should be boosted to higher\n");
          
	  sleep(100);
          check(getpinfo(&st) == 0, "getpinfo");
          
          DEBUG_PRINT((1, "After priority gets updated and scheduled for some time\npid: %d\n", st.pid[j]));
          for (int k = 3; k >= 0; k--) {
            DEBUG_PRINT((1, "\t level %d ticks used %d\n", k, st.ticks[j][k]));
          }
    }
  }

  // Wait for child processes to finish..
  wait();

  printf(1, "TEST PASSED");

  exit();
}
