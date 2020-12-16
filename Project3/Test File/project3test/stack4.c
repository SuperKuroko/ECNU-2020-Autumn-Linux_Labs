/* stack should not grow into 5 pages above heap (program must terminate) */
#include "types.h"
#include "user.h"

#undef NULL
#define NULL ((void*)0)

#define assert(x) if (x) {} else { \
  printf(1, "%s: %d ", __FILE__, __LINE__); \
  printf(1, "assert failed (%s)\n", # x); \
  printf(1, "TEST FAILED\n"); \
  exit(); \
}

int
main(int argc, char *argv[])
{
  int ppid = getpid();

  // ensure they actually placed the stack high...
  *(char*)(159*4096) = 'a';

  // should work fine
  *(char*)(159*4096-1) = 'a';
  //*(char*)(157*4096) = 'a';
  *(char*)(158*4096) = 'a';

  int pid = fork();
  if(pid == 0) {
    uint sz = (uint) sbrk(0);
    sz = (sz+4096-1)/4096*4096;
    char* STACK = (char*)(159*4096);

    while (STACK >= (char*)sz+5*4096) {
      *STACK = 'a';
      STACK -= 4096;
    }
    // should fail
    *STACK = 'a';
    printf(1, "TEST FAILED\n");
    kill(ppid);
    exit();
  } else {
    wait();
  }

  printf(1, "TEST PASSED\n");
  exit();
}
