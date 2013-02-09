/*
 * pipesem.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "pipesem.h"

void pipesem_init(struct pipesem *sem, int val)
{
	int i,fd[2];
	if (pipe(fd) < 0){
		perror("could not pipe!");
		exit(1);
	}
	sem->rfd = fd[0];
	sem->wfd = fd[1];
	for (i = 0 ; i < val ; i++)
	  pipesem_signal(sem);
}

void pipesem_wait(struct pipesem *sem)
{
	int letter;
    if (read(sem->rfd, &letter, sizeof(letter)) < 0){
		perror("semaphore: could not wait !");
		exit(1);
	}
}

void pipesem_signal(struct pipesem *sem)
{
	int cookie = 1 ; 
		if (write(sem->wfd, &cookie, sizeof(cookie)) != sizeof(cookie)) {
        	perror("semaphore: could not signal !");
        	exit(1);
	}
}

void pipesem_destroy(struct pipesem *sem)
{
	if (close(sem->rfd) < 0){
		perror("semaphore: could not delete reading end !");
		exit(1);
	}
	if (close(sem->wfd) < 0){
		perror("semaphore: could not delete writing end !");
		exit(1);
	}
}
