#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "proc-common.h"
#include "tree.h"

#define SLEEP_PROC_SEC  10
#define SLEEP_TREE_SEC  3

void fork_procs(struct tree_node* node, int fd)
{
    
    pid_t child ;
    int i,result,status;
    pid_t* children;
    int f[2][2];
    int op1,op2;
    
    change_pname(node->name);
    printf("PID = %ld, name %s, created!\n",(long)getpid(), node->name);
    
    if (node->nr_children ==0){
		result = atoi(node->name);
        if (write(fd, &result, sizeof(result)) != sizeof(result)) {
            perror("write to pipe");
            exit(1);
        }
		printf("%s: Wrote to pipe result: %d!\n",node->name,result);
        exit(0);
    }
    children = (pid_t*)malloc(2*sizeof(pid_t));
	if (children == NULL){
			fprintf(stderr, "allocate children failed\n");
			exit(1);
	}
    for ( i = 0 ; i < node->nr_children; i++){
        printf("%s: Creating child %s...\n",node->name,node->children[i].name);
        if (pipe(f[i]) < 0 ) {
			perror("pipe");
			exit(1);
		}
        child = fork();
        if (child < 0) {
            perror("fork");
            exit(1);
        }
        if (child == 0) {
            /* Child */
            if (close(f[i][0]) < 0 ) {
				perror("Error while closing File");
				exit(1);
			}
            fork_procs(&node->children[i],f[i][1]);
        }
        children[i] = child ;
        if (close(f[i][1]) < 0 ) {
			perror("Error while closing File");
			exit(1);
		}
    }
	child = wait(&status);
	explain_wait_status(child, status);
	child = wait(&status);
	explain_wait_status(child, status);
	
    if (read(f[0][0], &op1, sizeof(op1)) != sizeof(op1)) {
        perror("child: read from pipe");
        exit(1);
    }
    if (read(f[1][0], &op2, sizeof(op2)) != sizeof(op2)) {
        perror("child: read from pipe");
        exit(1);
    }
	printf("%s: Read from pipes values %d & %d \n",node->name,op1,op2);
    if (strcmp(node->name,"+") == 0)
        result = op1 + op2;
    else
        result = op1 * op2;
	
    if (write(fd, &result, sizeof(result)) != sizeof(result)) {
        perror("parent: write to pipe");
        exit(1);
    }
	printf("%s: Wrote to pipe result: %d!\n",node->name,result);
    printf("%s: Exiting...\n",node->name);
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
int main(int argc , char *argv[])
{
	pid_t pid;
    int pfd[2];
	int status,result;
    struct tree_node* root;
    
    if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_tree_file>\n\n", argv[0]);
		exit(1);
	}
    
	root = get_tree_from_file(argv[1]);
	print_tree(root);
    printf("Parent: Creating pipe...\n");
	if (pipe(pfd) < 0) {
		perror("pipe");
		exit(1);
	}
    
	printf("Parent: Creating child...\n");
	/* Fork root of process tree */
	pid = fork();
	if (pid < 0) {
		perror("main: fork");
		exit(1);
	}
	if (pid == 0) {
		/* Child */
        if (close(pfd[0]) < 0 ) {
			perror("Error while closing File");
			exit(1);
		}
		fork_procs(root,pfd[1]);
	}

	/*
	 * Father
	 */
	//sleep(SLEEP_TREE_SEC);
	//show_pstree(pid);
    if ( close(pfd[1]) < 0 ) {
		perror("Error while closing File");
		exit(1);
	}
    //wait_for_ready_children(1);
	/* Wait for the root of the process tree to terminate */
	pid = wait(&status);
	explain_wait_status(pid, status);
    if (read(pfd[0], &result, sizeof(result)) != sizeof(result)) {
		perror("read from pipe");
		exit(1);
	}
	printf("Result: %d\n", result);
	return 0;
}
