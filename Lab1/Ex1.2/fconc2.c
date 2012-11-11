#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void doWrite(int fd , const char *buff , int len){
	write(fd,buff,len);
}

void write_file(int fd , const char *infile){
	int size,infd ; 
	char buffer[1024];
	if ((infd = manageOpen(infile,O_RDONLY)) == -1) exit(1);
	while ((size = read(infd,buffer,1024))){
		doWrite(fd,buffer,size);
	}
	if (close(infd) < 0 ) {
		perror("Error while closing input File");
		exit(1);
	}
}

int checkArgs(int argc , char **argv , char ***infile , char ** outfile){

        int i ;
        if (argc < 3 ) {
                write(2,"Usage: ./fconc infile1 infile2 [outfile (default:fconc.out)]\n",62);
		return -1 ; 
        }
        else if (argc < 4 ){
                *infile = (char **)malloc((argc-1)*sizeof(char*));
                for (i = 0 ; i < argc -1; i++)
                        (*infile)[i] = argv[i+1];
           	*outfile = "fconc.out";
		return 0 ; 
		}
	else {
		*infile = (char **)malloc((argc-2)*sizeof(char*));
                for (i = 0 ; i < argc -2; i++)
                        (*infile)[i] = argv[i+1];
		*outfile = argv[argc-1];
		return 0 ; 
	}	
         
 }
      
int manageOpen(char *file , int oflags){
	int fd ; 
        fd = open(file,oflags,S_IRUSR|S_IWUSR);
	if (fd == -1){
		perror(file);
		return -1 ; 
	}
	return fd;

}

int searchIO(int argc, char **infile , char *outfile){

	int found = -1 ;  
	int i = 0 ; 
	// function called only when outfile is user-defined 
	while ((found == -1)&&(i < argc -2)){
		if (strcmp(infile[i],outfile) ==0)
			found = i ;
		
		
		i++;
	}

	return found ; 	   
}
    
int main (int argc , char **argv)
{
        char **infile,*outfile;
	char tmp[20];
        int infd1,infd2,outfd,position,tmpfd;
	int i ; 
	if (checkArgs(argc,argv,&infile,&outfile) == -1 ) 
		return -1 ;
 
	if ((position = searchIO(argc,infile,outfile)) != -1) 
	{
	//Create a temporary file to copy the context of the infile that will be also the outfile
		strncpy(tmp,"/tmp/temp-XXXXXX",sizeof tmp);
		if ((tmpfd = mkstemp(tmp)) == -1) {
			perror("Problem while creating tmp file");
			exit(1);
		}
		write_file(tmpfd,infile[position]);
		if ((outfd = manageOpen(outfile,O_WRONLY|O_CREAT|O_TRUNC)) ==-1) return -1 ; 
		for (i = 0 ; i < argc -2 ; i++){
			if (strcmp(infile[i],outfile) ==0) 
				write_file(outfd,tmp);
			else 
				write_file(outfd,infile[i]);
		}
	}
 	else {
		if ((outfd = manageOpen(outfile,O_WRONLY|O_CREAT)) ==-1) return -1 ; 
		write_file(outfd,infile[0]);
		write_file(outfd,infile[1]);
	}
	if (close(outfd) < 0 ) {
		perror("Error while closing output file");
		exit(1);
	}
	unlink(tmp);
	return 0 ; 
}
