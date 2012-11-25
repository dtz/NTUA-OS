#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char tmp[20] = "";
int manageOpen(const char * , int); //fowarding

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
/// function called to check if arguments were inserted correctly by the use and stores infiles/outfile
// (or creates the default) . In case of error , function just displays  a help message !
int checkArgs(int argc , char **argv , char ***infile , char ** outfile){
    
    int i,fin;
    if (argc < 3 ) {
        write(2,"Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n",62);
		return -1 ;
    }
    else {
        if (argc < 4) {
            fin = argc -1 ;
            *outfile = "fconc.out";
        }
        else {
            fin = argc -2 ;
            *outfile = argv[argc-1];
        }
        *infile = (char **)malloc((fin)*sizeof(char*));
        for (i = 0 ; i < fin; i++)
            (*infile)[i] = argv[i+1];
    }
    return 0 ;
}

// function called for read/write using filename and the correct flags
// returns the (int) file descriptor (as return by open) .In case of
// error displays error message and returns -1
int manageOpen(const char *file , int oflags){
    
	int fd ;
    fd = open(file,oflags,S_IRUSR|S_IWUSR);
	if (fd == -1){
		perror(file);
		return -1 ;
	}
	return fd;
    
}
    
int main (int argc , char **argv)
{
    char **infile,*outfile;
    int outfd,tmpfd;
    int i ,fin; 
	if (checkArgs(argc,argv,&infile,&outfile) == -1 ) 
		return -1 ;
    if (argc < 4) fin = argc -1 ;
    else fin = argc -2 ;
    i = 0 ; 
    while ((i<fin) && (strcmp(tmp,"") == 0)){
      //search for case one infile == outfile
        if (strcmp(infile[i],outfile) ==0){
            ///Create a temporary file to copy the initial context of the infile
            //that will be also the outfile
            strncpy(tmp,"./temp-XXXXXX",sizeof tmp);
            if ((tmpfd = mkstemp(tmp)) == -1) {
                perror("Problem while creating tmp file");
                exit(1);
            }
            //copy the initial state of the future outfile to temp file ! 
            write_file(tmpfd,infile[i]); 
        }
	i++;
    }
    if ((outfd = manageOpen(outfile,O_WRONLY|O_CREAT|O_TRUNC)) ==-1) return -1 ;
    for (i = 0 ; i < fin ; i++){
        if (strcmp(infile[i],outfile) ==0) // bypass the infile using tmp
            write_file(outfd,tmp);
        else // infile is different from outfile
            write_file(outfd,infile[i]);
    }
    
	if (close(outfd) < 0 ) {
		perror("Error while closing output file");
		exit(1);
	}
	free(infile);
    	remove(tmp);
	return 0; 
}
