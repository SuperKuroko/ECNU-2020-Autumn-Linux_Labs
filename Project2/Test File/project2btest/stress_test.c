#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

#define check(exp, msg) if(exp) {} else {				\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg); \
  exit();}

int pow2[] = {80000000, 32, 16, 8};

int workload(int n) {
  int i, j = 0;
  for(i = 0; i < n; i++)
    j += i * j + 1;
  return j;
}

int
main(int argc, char *argv[])
{
  sleep(10);

  struct pstat st;
  check(getpinfo(&st) == 0, "getpinfo");

  int i, c_pid;
  for (i = 0; i <= NPROC - 4; i++) {
    c_pid = fork();
    // Child
    if (c_pid == 0) {
      workload(1000 * i);
      exit();
    }
    
    wait();    
    check(getpinfo(&st) == 0, "getpinfo");
    int j, l;
    for (l = 0; l < NPROC; l++) {
        if (st.inuse[l] && st.pid[l] > 2) {
            check(st.ticks[l][3] > 0, "Every process at the highest level should use at least 1 timer tick");
            printf(1, "pid : %d, level : %d, ticks : %d\n", st.pid[l], st.priority[l], st.ticks[l][st.priority[l]]);
            for (j = 3; j > st.priority[l]; j--) {
                check(st.ticks[l][j] == pow2[j], "incorrect #ticks at this level");
            }
            check(st.ticks[l][j] <= pow2[j],
                "#ticks at this level should not exceeds the maximum allowed");
        }
    }
  }

  printf(1, "TEST PASSED");
  exit();
}

