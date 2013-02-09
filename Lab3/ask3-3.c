/*
 * procs-shm.c
 *
 * A program to create three processes,
 * working with a shared memory area.
 *
 * Vangelis Koukis <vkoukis@cslab.ece.ntua.gr>
 * Operating Systems
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "proc-common.h"
#include "pipesem.h"
/*
 * This is a pointer to a shared memory area.
 * It holds an integer value, that is manipulated
 * concurrently by all children processes.
 */
int *shared_memory;
struct pipesem sem[3]; /* ... */

/* Proc A: n = n + 1 */
void proc_A(void)
{
	volatile int *n = &shared_memory[0];
	/* ... */
    
	for (;;) {
		/* wait for proc C to throw 1 cookie to assigned pipe 
		 * initially there is no cookie so proc A blocks and 
		 * waits for proc C  i.e. first print of "1" ! 
		 */
		pipesem_wait(&sem[2]);
		*n = *n + 1;
		/* after calculation i.e. n -> n+1  throw 1 cookie to 
		 * assigned pipe in order to be gobbled by proc B
		 * in order for the synchronization to be correct,proc A
		 * must be executed twice before proc B 
		 * A:(n->n+1->n+2) and B:(n+2->n)
		 */
		pipesem_signal(&sem[0]);
	}
    
	exit(0);
}

/* Proc B: n = n - 2 */
void proc_B(void)
{
	volatile int *n = &shared_memory[0];
	/* ... */
    
	for (;;) {
		/* wait for proc A to throw 2 cookies to assigned pipe 
		 * initially there is no cookie so proc B blocks and 
		 * waits for proc A  i.e. n is then n+2 ! 
		 */
		pipesem_wait(&sem[0]);
		pipesem_wait(&sem[0]);
		*n = *n - 2;
		/* after calculation i.e. n+2 -> n  throw 1 cookie to 
		 * assigned pipe in order to be gobbled by proc C
		 */
		pipesem_signal(&sem[1]);
	}
    
	exit(0);
}

/* Proc C: print n */
void proc_C(void)
{
	int val;
    
	volatile int *n = &shared_memory[0];
	/* ... */
    
	for (;;) {
		/* wait for proc B to throw a cookie to assigned pipe 
		 * initially there is only one cookie so proc C does
		 * not block ! :) 
		 */
		pipesem_wait(&sem[1]);
		val = *n;
		printf("Proc C: n = %d\n", val);
		if (val != 1) {
			printf("     ...Aaaaaargh!\n");
		}
		/* after calculation i.e. print throw 2 cookies to 
		 * assigned pipe in order to be gobbled by proc A 
		 */
		pipesem_signal(&sem[2]);
		pipesem_signal(&sem[2]);
	}
	exit(0);
}

/*
 * Use a NULL-terminated array of pointers to functions.
 * Each child process gets to call a different pointer.
 */
typedef void proc_fn_t(void);
static proc_fn_t *proc_funcs[] = {proc_A, proc_B, proc_C, NULL};

int main(void)
{
	int i;
	int status;
	pid_t p;
	proc_fn_t *proc_fn;
    
	/* Initialize semaphores for the three processes */
	pipesem_init(&sem[0],0); // semaphore for proc A 
	pipesem_init(&sem[1],1); // semaphore for proc B
    pipesem_init(&sem[2],0); // semaphore for proc C 
	/* Create a shared memory area */
	shared_memory = create_shared_memory_area(sizeof(int));
	*shared_memory = 1;
    
	for(i = 0; (proc_fn = proc_funcs[i]) != NULL; i++) {
		printf("%lu fork()\n", (unsigned long)getpid());
		p = fork();
		if (p < 0) {
			perror("parent: fork");
			exit(1);
		}
		if (p != 0) {
			/* Father */
			continue;
		}
		/* Child */
		proc_fn();
		assert(0);
	}
    
	/* Parent waits for all children to terminate */
	for (; i >0; i--)
		wait(&status);
    
	return 0;
}
