#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "checkArgs.h"
#include "manageOpen.h"
#include "write_file.h"

char tmp[20] = "";
    
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
