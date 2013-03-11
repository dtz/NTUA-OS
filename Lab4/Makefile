#
# Makefile
# 

CC = gcc
CFLAGS = -Wall -O2

all: prog scheduler

proc-common.o: proc-common.h proc-common.h
	$(CC) $(CFLAGS) -c -o proc-common.o proc-common.c
prog.o: prog.c
	$(CC) $(CFLAGS) -o prog.o -c prog.c
prog: prog.o proc-common.o
	$(CC) -o prog prog.o proc-common.o

rr-queue.o: rr-queue.h rr-queue.h
	$(CC) $(CFLAGS) -c -o rr-queue.o rr-queue.c

scheduler.o: proc-common.h rr-queue.h scheduler.c
	$(CC) $(CFLAGS) -c -o scheduler.o scheduler.c
scheduler:	scheduler.o rr-queue.o 
	$(CC) $(CFLAGS) -o scheduler rr-queue.o proc-common.o scheduler.o 

clean:
	rm -f *.o prog scheduler
