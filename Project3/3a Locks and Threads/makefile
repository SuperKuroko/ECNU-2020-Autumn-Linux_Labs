all: liblock.so libcounter.so liblist.so libhash.so main
liblock.so: lock.c lock.h spinlock.c spinlock.h mutex.c mutex.h xchg.c sys_futex.c
	@gcc -shared -fPIC -o liblock.so -lpthread lock.c lock.h spinlock.c spinlock.h mutex.c mutex.h xchg.c sys_futex.c
libcounter.so: counter.c counter.h
	@gcc -shared -fPIC -o libcounter.so -I. -L. -llock counter.c counter.h
liblist.so: list.c list.h
	@gcc -shared -fPIC -o liblist.so -I. -L. -llock list.c list.h
libhash.so: hash.c hash.h
	@gcc -shared -fPIC -o libhash.so -I. -L. -llock list.c list.h hash.c hash.h
main: main.c
	@gcc -o main main.c -lpthread -llist -lcounter -lhash -llock -L. -Wall 
clean:
	rm *.o *.so  main -rf
