
/*
 * mysem.h by dtz 
 *
 * A simple library to implement
 * semaphores over Unix pipes.
 *
 * Vangelis Koukis <vkoukis@cslab.ece.ntua.gr>
 */

#ifndef MYSEM_H__
#define MYSEM_H__

struct pipesem {
	/*
	 * Two file descriptors:
	 * one for the read and one for the write end of a pipe
	 */
	int rfd;
	int wfd;
};

/*
 * Function prototypes
 */
void pipesem_init(struct pipesem *sem, int val);
void pipesem_wait(struct pipesem *sem,int val);
void pipesem_signal(struct pipesem *sem,int val);
void pipesem_destroy(struct pipesem *sem);

#endif /* MYSEM_H__ */
