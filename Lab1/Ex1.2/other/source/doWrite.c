#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
// function thats actually writes a 1024 bytes buffer
// to the outfile using its file descriptor and repeats
// writing to ensure that all bytes are successfuuly written to
// to output file
void doWrite(int fd , const char *buff , int len){
	int size,written;
	written = 0;
	size = len;
	while (written = write(fd,buff,size))
	{	size -=written;
		if (written ==-1){
			perror("Error while writing file");
			exit(1);
		}
	}
}
