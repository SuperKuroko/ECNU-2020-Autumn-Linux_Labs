#include "mem.h"
#include <assert.h>
#include <stdlib.h>

int main() {
   assert(Mem_Init(4096) == 0);
   exit(0);
}