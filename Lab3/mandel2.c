/*
 * mandel.c
 *
 * A program to draw the Mandelbrot Set on a 256-color xterm.
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

#include "mandel-lib.h"
#include "pipesem.h"
#define MANDEL_MAX_ITERATION 100000
#define NCHILDREN 3

/***************************
 * Compile-time parameters *
 ***************************/

/*
 * Output at the terminal is is x_chars wide by y_chars long
*/
int y_chars = 50;
int x_chars = 90;
struct pipesem sem[NCHILDREN];
/*
 * The part of the complex plane to be drawn:
 * upper left corner is (xmin, ymax), lower right corner is (xmax, ymin)
*/
double xmin = -1.8, xmax = 1.0;
double ymin = -1.0, ymax = 1.0;
	
/*
 * Every character in the final output is
 * xstep x ystep units wide on the complex plane.
 */
double xstep;
double ystep;

/*
 * This function computes a line of output
 * as an array of x_char color values.
 */
void compute_mandel_line(int line, int color_val[])
{
	/*
	 * x and y traverse the complex plane.
	 */
	double x, y;

	int n;
	int val;

	/* Find out the y value corresponding to this line */
	y = ymax - ystep * line;

	/* and iterate for all points on this line */
	for (x = xmin, n = 0; x <= xmax; x+= xstep, n++) {

		/* Compute the point's color value */
		val = mandel_iterations_at_point(x, y, MANDEL_MAX_ITERATION);
		if (val > 255)
			val = 255;

		/* And store it in the color_val[] array */
		val = xterm_color(val);
		color_val[n] = val;
	}
}

/*
 * This function outputs an array of x_char color values
 * to a 256-color xterm.
 */
void output_mandel_line(int fd, int color_val[])
{
	int i;
	
	char point ='@';
	char newline='\n';

	for (i = 0; i < x_chars; i++) {
		/* Set the current color, then output the point */
		set_xterm_color(fd, color_val[i]);
		if (write(fd, &point, 1) != 1) {
			perror("compute_and_output_mandel_line: write point");
			exit(1);
		}
	}

	/* Now that the line is done, output a newline character */
	if (write(fd, &newline, 1) != 1) {
		perror("compute_and_output_mandel_line: write newline");
		exit(1);
	}
}

void compute_and_output_mandel_line(int fd, int line,int i)
{
	/*
	 * A temporary array, used to hold color values for the line being drawn
	 */
	int color_val[x_chars];
	// Each child computes its line without waiting
	compute_mandel_line(line, color_val);
	// after computation , waits for its turn to output the result ! 
	pipesem_wait(&sem[i]);
	output_mandel_line(fd, color_val);
	pipesem_signal(&sem[(i+1) % NCHILDREN]);
	// after output , gives its turn to the next one 
}

int main(void)
{
    int i,j;
    int pid,status;
	xstep = (xmax - xmin) / x_chars;
	ystep = (ymax - ymin) / y_chars;
    /*
     * First create pipe to communicate with processes
     * and make the child processes via fork
     */
    pipesem_init(&sem[0],1);
    for (i = 1 ; i < NCHILDREN; i++){
        pipesem_init(&sem[i],0);
    }
    for (i = 0 ; i < NCHILDREN; i++){
 
        if ((pid = fork()) < 0) {
            perror("fork could not be done!");
            exit(1);
        }
        /* Child body */
        if (pid == 0){
           for (j = 0 ; j < y_chars ; j++){
		   /* Each child computes the appropriate line and waits
			* for its turn to output the result . As a result the 
			* Mandelbrot set is drawn one line at a time ! 
			*/
			 
				if ((j %NCHILDREN) == i ) 
		 		compute_and_output_mandel_line(1,j,i);
	   		}
	   		exit(0);
		}
     }
     /* Father after creating children waits for them to die! */
	 for (i = 0 ; i < NCHILDREN; i++){
		wait(&status);
	}
	for (i = 0 ; i < NCHILDREN; i++){
		pipesem_destroy(&sem[i]);
	}
	reset_xterm_color(1);
	return 0;
}
