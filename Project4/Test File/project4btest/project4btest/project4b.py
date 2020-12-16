#import toolspath
from testing import Xv6Build, Xv6Test
from testing.runtests import main

class Test0(Xv6Test):
   name = "create"
   description = """create"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test1(Xv6Test):
   name = "create2"
   description = """create2"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"
 
class Test3(Xv6Test):
   name = "join"
   description = """join"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test4(Xv6Test):
   name = "join2"
   description = """join2"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"


class Test6(Xv6Test):
   name = "recursion"
   description = """recursion"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"


released_tests = [ Test0, Test1, Test3, Test4, Test6]
#hidden_tests = [TestHidden2]
all_tests = released_tests

main(Xv6Build, released_tests)
