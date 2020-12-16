#include <assert.h>
#include <stdlib.h>
#include "mem.h"

int main() {
   assert(Mem_Init(1) == 0);
   assert(Mem_Alloc(4048, FIRSTFIT) != NULL);
   exit(0);
}