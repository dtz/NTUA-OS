#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tree.h"
#include "proc-common.h"

void fork_procs(struct tree_node *root)
{
	pid_t child ;
   	int i,status ;
    pid_t* children;
    children = (pid_t*)malloc((sizeof(pid_t)*root->nr_children));
	if (children == NULL){
			fprintf(stderr, "allocate children failed\n");
			exit(1);
	}
	printf("PID = %ld, name %s, starting...\n",(long)getpid(), root->name);
	change_pname(root->name);
	for (i = 0 ; i < root->nr_children; i++){

        printf("%s:Creating child %s...\n",root->name,root->children[i].name);
        child = fork();
        if (child < 0) {
            perror("child fork");
            exit(1);
        }
        if (child == 0) {
            /* Child */
            fork_procs(&root->children[i]);
        }
	/* Father saves child's pid ! */
		children[i] = child;
    }	
	wait_for_ready_children(root->nr_children);
	printf("%s: All children ready ! Suspending with SIGSTOP...\n",root->name);
	/*
	 * Suspend Self
	 */
	raise(SIGSTOP);
	printf("PID = %ld, name = %s is awake\n",(long)getpid(), root->name);
	for (i = 0 ; i < root->nr_children;i++){
		printf("%s: Activating child %s...\n",root->name,root->children[i].name);
		kill(children[i],SIGCONT);
		children[i] = wait(&status);
		explain_wait_status(children[i],status);
	}

	printf("%s: Exiting...\n",root->name);
	free(children);
	exit(0);
}

/*
 * The initial process forks the root of the process tree,
 * waits for the process tree to be completely created,
 * then takes a photo of it using show_pstree().
 *
 * How to wait for the process tree to be ready?
 * In ask2-{fork, tree}:
 *      wait for a few seconds, hope for the best.
 * In ask2-signals:
 *      use wait_for_ready_children() to wait until
 *      the first process raises SIGSTOP.
 */

int main(int argc, char *argv[])
{
	pid_t pid;
	int status;
	struct tree_node *root;

	if (argc < 2){
		fprintf(stderr, "Usage: %s <tree_file>\n", argv[0]);
		exit(1);
	}

	/* Read tree into memory */
	root = get_tree_from_file(argv[1]);

	/* Fork root of process tree */
	pid = fork();
	if (pid < 0) {
		perror("main: fork");
		exit(1);
	}
	if (pid == 0) {
		/* Child */
		fork_procs(root);
		exit(1);
	}

	/*
	 * Father
	 */
	/* for ask2-signals */
	wait_for_ready_children(1);

	/* Print the process tree root at pid */
	show_pstree(pid);

	/* for ask2-signals */
	kill(pid, SIGCONT);

	/* Wait for the root of the process tree to terminate */
	pid = wait(&status);
	explain_wait_status(pid, status);

	return 0;
}
