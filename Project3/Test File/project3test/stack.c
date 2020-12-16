/* stack should be at high end of address space */
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

void
foo(void *mainlocal) 
{
  int local;
  assert((uint) &local < (uint) mainlocal);
}

int
main(int argc, char *argv[])
{
  assert(argc == 1);
  assert((uint)argv[0] == 0x9fff8);
  assert((uint)argv[1] == 0);
  assert((uint)&argv[1] == 0x9fff4);
  assert((uint)&argv[0] == 0x9fff0);
  assert((uint)&argv == 0x9ffec);
  assert((uint)&argc == 0x9ffe8);
  assert(*(&argc-1) == 0xffffffff);
  int local;
  assert((uint)&local >= 0x9f000);
  foo((void*) &local);
  printf(1, "TEST PASSED\n");
  exit();
}
