#import toolspath
from testing import Xv6Build, Xv6Test
from testing.runtests import main

class Test0(Xv6Test):
   name = "null"
   description = """null"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test1(Xv6Test):
   name = "null2"
   description = """null2"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"
 
   
class Test2(Xv6Test):
   name = "bounds"
   description = """bounds"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test3(Xv6Test):
   name = "bounds2"
   description = """bounds2"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test4(Xv6Test):
   name = "stack"
   description = """stack"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"


class Test6(Xv6Test):
   name = "heap"
   description = """heap"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"


class Test7(Xv6Test):
   name = "stack2"
   description = """stack2"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"


class Test8(Xv6Test):
   name = "bounds3"
   description = """bounds3"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"


class Test10(Xv6Test):
   name = "stack4"
   description = """stack4"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"


released_tests = [ Test0, Test1, Test2, Test3, Test4, Test6, Test7, Test8, Test10 ]
all_tests = released_tests

main(Xv6Build, released_tests)
