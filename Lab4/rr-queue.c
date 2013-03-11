#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rr-queue.h"

queue * insert_to_queue(pid_t pid,int id,queue * last,char *execname){
    
    if (last->next == NULL){
       	// insertion of the first process : create self-loop cycle
        last->previous = last;
        last->next = last ;
        
    }
    // rr-queue has at least one process already inserted!
    else {

	queue * new = (queue *) malloc(sizeof(queue));
        if (!new){
        	perror("rr-queue : no memory!");
        	exit(1);
	}
        queue * temp = last->previous;
        //connect new node with the last double-connected
        //         __________________________
        // [new]--'->[last]<->....<->[head]<-'
        new->next = last ;
        // connect the last double-connected with the new node
        //         __________________________
        // [new]<-'->[last]<-->...<->[head]<-'
        last->previous = new ;
        // connect new node with head i.e. close the cycle
        //  ___________________________________
        // '->[new]<-->[last]<-->...<->[head]<-'
        new->previous = temp ;
        temp->next = new ;
        // connection done ! make new node the last connected
        last = new ;
    }
    // write process p(id) [same for both cases]
    last->pid = pid ;
    last->id = id ;
    strcpy(last->procname,execname);
    return last ; 
}
queue * remove_from_queue(queue * pointer){
    if (pointer == pointer->next){
        
    }
    queue * temp = pointer->previous;
    (pointer->previous)->next = pointer->next;
    (pointer->next)->previous = pointer->previous;
    pointer->next = NULL;
    pointer->previous = NULL;
    free(pointer);
    return temp;
    
}
queue * find_pid_queue(queue * pointer , int pid){
    queue * temp = pointer ; 
    while (temp->pid != pid){
	temp = temp->next;
    }
    return temp;
}
void print_queue(queue * head , int num){
    int i = 0 ;
    queue * temp = head ;
    for (i = 0 ; i < num ; i++){
        printf("PID:%d\tID:%d\n",temp->pid,temp->id);
        temp = temp->next ;
    }
}
