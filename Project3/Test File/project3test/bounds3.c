/* syscall argument checks (dynamic stack boundaries) */
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

  /* grow the stack a bit */
  char* STACK = (char*)(159*4096);
  *STACK = 'a';
  STACK -= 4096;
  *STACK = 'b';
  STACK -= 4096;
  STACK[4095] = 'c';
  uint USERTOP = 160*4096;

  /* below stack */
  arg = (char*) STACK - 1;
  assert(write(fd, arg, 1) == -1);
  assert(open(arg, O_WRONLY|O_CREATE) == -1);

  /* spanning stack bottom */
  assert(write(fd, arg, 2) == -1);

  /* at stack */
  arg = (char*) STACK;
  assert(write(fd, arg, 1) != -1);
  strcpy(arg, "bar");
  fd = open(arg, O_WRONLY|O_CREATE);
  assert(fd != -1);
  assert(unlink(arg) != -1);

  /* within stack */
  arg = (char*) (STACK + 6*1024);
  assert(write(fd, arg, 40) != -1);
  strcpy(arg, "foo");
  fd = open(arg, O_WRONLY|O_CREATE);
  assert(fd != -1);
  assert(unlink(arg) != -1);

  /* at stack top */
  arg = (char*) USERTOP-1;
  assert(write(fd, arg, 1) != -1);
  arg = (char*) USERTOP-2;
  assert(arg[0] == '\0');
  strcpy(arg, "x");
  fd = open(arg, O_WRONLY|O_CREATE);
  assert(fd != -1);
  assert(unlink(arg) != -1);
  arg[0] = '\0';

  /* spanning stack top */
  arg = (char*) USERTOP-1;
  assert(write(fd, arg, 2) == -1);
  arg[0] = 'a';
  assert(open(arg, O_WRONLY|O_CREATE) == -1);

  /* above stack top */
  arg = (char*) USERTOP;
  assert(write(fd, arg, 1) == -1);
  assert(open(arg, O_WRONLY|O_CREATE) == -1);

  printf(1, "TEST PASSED\n");
  exit();
}
