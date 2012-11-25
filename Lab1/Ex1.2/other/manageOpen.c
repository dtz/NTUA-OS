#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int manageOpen(const char *file , int oflags){
    
	int fd ;
    fd = open(file,oflags,S_IRUSR|S_IWUSR);
	if (fd == -1){
		perror(file);
		return -1 ;
	}
	return fd;
    
}
