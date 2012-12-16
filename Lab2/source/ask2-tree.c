#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "proc-common.h"
#include "tree.h"

#define SLEEP_PROC_SEC  10
#define SLEEP_TREE_SEC  3

void fork_procs(struct tree_node* node)
{
    
    pid_t child ;
    int i,status ;
    pid_t* children;

    children = (pid_t*)malloc((sizeof(pid_t)*node->nr_children));
	if (children == NULL){
			fprintf(stderr, "allocate children failed\n");
			exit(1);
	}
    change_pname(node->name);
    printf("PID = %ld, name %s, created!\n",(long)getpid(), node->name);
    /* forking method */
    for (i = 0 ; i < node->nr_children; i++){
        printf("%s:Creating child %s...\n",node->name,node->children[i].name);
        child = fork();
        if (child < 0) {
            perror("child fork");
            exit(1);
        }
        if (child == 0) {
            /* Child */
            fork_procs(&node->children[i]);
        }
	/* Father saves child's pid ! */
	children[i] = child;
    }
    if (node->nr_children == 0){
        printf("%s:Sleeping...\n",node->name);
        sleep(SLEEP_PROC_SEC);
        printf("%s:Exiting...\n",node->name);
    }
    else {
		printf("%s:Waiting...\n",node->name);
    	for (i = 0 ; i < node->nr_children;++i)
		{
           children[i]=wait(&status);
           explain_wait_status(children[i],status);
		}        
    	printf("%s:Exiting...\n",node->name);
		free(children);
    }
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
int main(int argc , char *argv[])
{
	pid_t pid;
	int status;
    struct tree_node* root;
    
    if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
		exit(1);
	}
    
	root = get_tree_from_file(argv[1]);
	print_tree(root);
    
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

	/* for ask2-{fork, tree} */
	sleep(SLEEP_TREE_SEC);
	/* Print the process tree root at pid */
	show_pstree(pid);
	/* Wait for the root of the process tree to terminate */
	pid = wait(&status);
	explain_wait_status(pid, status);

	return 0;
}
