all:	zing zing2

zing:	zing.o main.o
	gcc -m32 -Wall zing.o main.o -o zing

zing2:	zing2.o main.o
	gcc -m32 -Wall zing2.o main.o -o zing2
zing2.o:zing2.c
	gcc -m32 -Wall -c zing2.c	
main.o:	main.c
	gcc -m32 -Wall -c main.c
clean:
	rm -f main.o zing2.o
