all:	zing zing2

zing:	zing.o main.o
	gcc zing.o main.o -o zing

zing2:	zing2.o main.o
	gcc zing2.o main.o -o zing2

zing2.o:	zing2.c
	gcc -Wall -c zing2.c	
main.o:	main.c
	gcc -Wall -c main.c
