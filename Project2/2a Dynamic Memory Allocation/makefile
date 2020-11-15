mem : mem.c mem.h
	gcc -c -fpic mem.c -Wall
	gcc -shared -o libmem.so mem.o
.PHONY : clean
clean :
	rm *.o *.so -rf
