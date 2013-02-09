#
# Makefile
# 

CC = gcc
CFLAGS = -Wall -O2

all: mandel procs-shm pipesem.o pipesem-test ask3-3 bonus rand-fork

proc-common.o: proc-common.h proc-common.h
	$(CC) $(CFLAGS) -c -o proc-common.o proc-common.c

pipesem.o: pipesem.c pipesem.h
	$(CC) $(CFLAGS) -c -o pipesem.o pipesem.c

mysem.o: mysem.c mysem.h
	$(CC) $(CFLAGS) -c -o mysem.o mysem.c
## Pipesem
pipesem-test.o: pipesem.h pipesem-test.c
	$(CC) $(CFLAGS) -c -o pipesem-test.o pipesem-test.c

pipesem-test: pipesem.o pipesem-test.o
	$(CC) $(CFLAGS) -o pipesem-test pipesem.o pipesem-test.o

## Mandel
mandel-lib.o: mandel-lib.h mandel-lib.c
	$(CC) $(CFLAGS) -c -o mandel-lib.o mandel-lib.c

mandel.o: mandel-lib.h mandel.c
	$(CC) $(CFLAGS) -c -o mandel.o mandel.c

mandel: mandel-lib.o mandel.o pipesem.o
	$(CC) $(CFLAGS) -o mandel mandel-lib.o mandel.o pipesem.o

## Procs-shm
procs-shm.o: proc-common.h procs-shm.c
	$(CC) $(CFLAGS) -c -o procs-shm.o procs-shm.c

procs-shm: proc-common.o procs-shm.o
	$(CC) $(CFLAGS) -o procs-shm proc-common.o procs-shm.o
ask3-3.o: proc-common.h ask3-3.c
	$(CC) $(CFLAGS) -c -o ask3-3.o ask3-3.c
ask3-3:	ask3-3.o pipesem.o proc-common.o 
	$(CC) $(CFLAGS) -o ask3-3 pipesem.o proc-common.o ask3-3.o
bonus.o: proc-common.h bonus.c
	$(CC) $(CFLAGS) -c -o bonus.o bonus.c
bonus: bonus.o mysem.o proc-common.o 
	$(CC) $(CFLAGS) -o bonus mysem.o proc-common.o bonus.o
rand-fork: rand-fork.c
	$(CC) $(CFLAGS) -o rand-fork rand-fork.c
clean:
	rm -f pipesem-test mandel procs-shm ask3-3 bonus rand-fork
obj:
	rm -f *.o
