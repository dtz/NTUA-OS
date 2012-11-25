#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
