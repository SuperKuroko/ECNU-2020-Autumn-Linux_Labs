/* syscall argument checks (stack boundaries) */
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

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
  char *arg;

  int fd = open("tmp", O_WRONLY|O_CREATE);
  assert(fd != -1);

  uint STACK = 159*4096;
  uint USERTOP = 160*4096;

  /* below stack */
  arg = (char*) STACK - 1;
  assert(write(fd, arg, 1) == -1);

  /* spanning stack bottom */
  assert(write(fd, arg, 2) == -1);

  /* at stack */
  arg = (char*) STACK;
  assert(write(fd, arg, 1) != -1);

  /* within stack */
  arg = (char*) (STACK + 2048);
  assert(write(fd, arg, 40) != -1);

  /* at stack top */
  arg = (char*) USERTOP-1;
  assert(write(fd, arg, 1) != -1);

  /* spanning stack top */
  assert(write(fd, arg, 2) == -1);

  /* above stack top */
  arg = (char*) USERTOP;
  assert(write(fd, arg, 1) == -1);

  printf(1, "TEST PASSED\n");
  exit();
}
