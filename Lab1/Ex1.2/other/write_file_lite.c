#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "doWrite.h"
#include "manageOpen.h"

// function called to write an infile to the output file using a
// buffer and repeatedly calling function doWrite until all bytes
// are successfully written to output file.

void write_file(int fd , const char *infile){
	int size,infd ;
	char buffer[1024];
	if ((infd = manageOpen(infile,O_RDONLY)) == -1) {
		exit(1);
	}
	while ((size = read(infd,buffer,1024))){
		doWrite(fd,buffer,size);
	}
	if (close(infd) < 0 ) {
		perror("Error while closing input File");
		exit(1);
	}
}
