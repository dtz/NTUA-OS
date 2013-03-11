/*
 * rr-queue.h
 * 
 * A simple library to implement
 * Round-Robin queue for scheduling 
 * processes 
 */

#ifndef _rr_queue_h
#define _rr_queue_h
#define TASK_NAME_SZ 60  
typedef struct node {
    
    /*  Queue has the following structure :
     *
     *       ------------
     *      | pid  | id  |
     *      |------------|
     *      |  procname  |
     *      |------------|
     *  <-- | prev |next |-->
     *       ------------
     *
     */
    
    pid_t pid ;
    int id ;
    char procname [TASK_NAME_SZ];
    struct node * previous ;
    struct node * next ;
}queue;
queue * insert_to_queue(pid_t pid,int id,queue * last,char * execname);
queue * remove_from_queue(queue * pointer);
queue * find_pid_queue(queue * pointer,int pid);
void print_queue(queue *head,int num);

#endif
