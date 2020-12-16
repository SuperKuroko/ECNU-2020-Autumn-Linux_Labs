import toolspath
from testing import Xv6Build, Xv6Test

class Test0(Xv6Test):
   name = "getpinfo"
   description = """Workload: Check getpinfo works
  Expected: pstat fields filled by getpinfo"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test1(Xv6Test):
   name = "test_ptable"
   description = """Workload: Checks getpinfo mirrors ptable
  Expected: init, sh, tester are in the ptable"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test2(Xv6Test):
   name = "single_job_long"
   description = """Workload: Single long running process (spinning)
  Expected: Single long job should use up all time ticks of level 0-2 and running at the lowest priority"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test3(Xv6Test):
   name = "new_process"
   description = """Workload: Parent process runs until at lowest, fork child process
  Expected: Child process gets scheduled at the higher priority than parent"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test4(Xv6Test):
   name = "stress_test"
   description = """Workload: Fill the ptable multiple times with new processes.
  Expected: OS does not fail to allocate processes"""
   tester = name + ".c"
   timeout = 30
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test6(Xv6Test):
   name = "multiple_jobs"
   description = """Workload: Synchronous workload varying tick workload. (Spin duration)
  Expected: Verify each process only runs for the allotted amount of time at each level. (ie. p3 - 8 ticks, p2 - 16 ticks, p1 - 32 ticks, etc.)"""
   tester = name + ".c"
   timeout = 180
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test7(Xv6Test):
   name = "multiple_jobs_wait_times"
   description = """Workload: Synchronous workload varying tick workload. (Spin duration)
  Expected: Verify each process only waits for the expected amount of time 10x before being bumped up to higher priority except p = 3"""
   tester = name + ".c"
   timeout = 180
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test9(Xv6Test):
   name = "round_robin"
   description = """Workload: 4 processes running long running workloads (spinning)
   Expected: All processes have used up the timer ticks in level 3, 2 and 1 - executes in round robin fashion"""
   tester = name + ".c"
   timeout = 120
   point_value = 10
   make_qemu_args = "CPUS=1"

class Test10(Xv6Test):
   name = "priority_boost"
   description = """Workload: 2 processes running long running workloads
   Expected: A priority boost of the parent process after waiting for child to execute"""
   tester = name + ".c"
   timeout = 120
   point_value = 10
   make_qemu_args = "CPUS=1"

released_tests = [ Test0, Test1, Test2, Test3, Test4, Test6, Test7, Test9, Test10 ]
hidden_tests = []
all_tests = released_tests

import toolspath
from testing.runtests import main
main(Xv6Build, released_tests)
