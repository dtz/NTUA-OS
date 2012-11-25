#include <unistd.h>
#include <sys/types.h>

void zing(){
	
	char *name ; 
	name = getlogin();	 //returns  a  pointer  to a string containing the name of the
      				// user logged in on the controlling terminal of the process
        write(1,"Welcome ",8); //space is also counted 
	write(1,name,2*sizeof(name));
	write(1,"!\n",2);
	
}
