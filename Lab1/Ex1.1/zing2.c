#include <unistd.h>
#include <sys/types.h>

void zing(){
	
	char *name ; 
	name = getlogin();
        write(1,"Welcome ",8); //space is also counted 
	write(1,name,8);
	write(1,"!\n",2);
	
}
