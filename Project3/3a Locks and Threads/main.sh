#!/usr/bin/bash
echo "ECNU OS Lab3a Result" > testout.txt

echo -e "Test Start...\n"
echo "Pthread Mutex Lock is testing..."
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
make
./main 1 >> testout.txt
./main 2 >> testout.txt
./main 3 >> testout.txt
echo -e "Pthread Mutex Lock finished!\n\n"

sed -i 's/#define LOCK_TYPE      1/#define LOCK_TYPE      2/g' lock.h
touch main.c
echo "My Spin Lock is testing..."
make
./main 1 >> testout.txt
./main 2 >> testout.txt
./main 3 >> testout.txt
echo -e "My Spin Lock finished!\n\n"

sed -i 's/#define LOCK_TYPE      2/#define LOCK_TYPE      3/g' lock.h
touch main.c
echo "My Mutex Lock is testing..."
make
./main 1 >> testout.txt
./main 2 >> testout.txt
./main 3 >> testout.txt
echo -e "My Mutex Lock finished!\n\n"

sed -i 's/#define LOCK_TYPE      3/#define LOCK_TYPE      1/g' lock.h
touch main.c
echo -e "All Finished!\n"
echo -e "Please read testout.txt to get test result!\n"
