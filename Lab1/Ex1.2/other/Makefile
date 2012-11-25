fconc:	checkArgs.o doWrite.o manageOpen.o write_file_lite.o fconc.o
	gcc checkArgs.o doWrite.o manageOpen.o write_file_lite.o  fconc.o -o fconc
fconc.o: fconc.c
	gcc -Wall -c fconc.c
checkArgs.o: checkArgs.c
	gcc -Wall -c checkArgs.c
doWrite.o: doWrite.c 
	gcc -Wall -c doWrite.c
manageOpen.o: manageOpen.c
	gcc -Wall -c manageOpen.c
write_file_lite.o: write_file_lite.c manageOpen.o doWrite.o
	gcc -Wall -c write_file_lite.c manageOpen.o doWrite.o
bonus: fconc_bonus.c
	gcc fconc_bonus.c -o bonus
r:
	echo 'a' > A
	echo 'b' > B
clean: 
	rm -f *.o
