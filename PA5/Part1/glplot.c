#include <GL/gl.h>
#include "glplot.h"

/* Plots array data, for details see glplot.h */
void plotArray (double* x, double* y, int size, double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height) {
	double xdiff = xmax-xmin;
	double xfac = width/xdiff;
	double ydiff = ymax-ymin;
	double yfac = height/ydiff;
	glPointSize(3.0f);
	glBegin(GL_LINE_STRIP);
		for (int j=1; j<size; j++) {
			glVertex3f((x[j]-xmin)*xfac+xpos,(y[j]-ymin)*yfac+ypos,0.0f);
		}	
	glEnd();
}

/* Plots array data as points, for details see glplot.h */
void plotArrayPoints (double* x, double* y, int size, double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height) {
	double xdiff = xmax-xmin;
	double xfac = width/xdiff;
	double ydiff = ymax-ymin;
	double yfac = height/ydiff;
	glPointSize(3.0f);
	glBegin(GL_POINTS);
		for (int j=1; j<size; j++) {
			glVertex3f((x[j]-xmin)*xfac+xpos,(y[j]-ymin)*yfac+ypos,0.0f);
		}	
	glEnd();
}

/*Plots the graph of a function, for details see glplot.h */
void plotFunction (double (*f)(double), double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height, double stepsize) {
	double xdiff = xmax-xmin;
	double xfac = width/xdiff;
	double ydiff = ymax-ymin;
	double yfac = height/ydiff;
	double x = xmin;
	double y;
	glBegin(GL_LINE_STRIP);
		y = f(x);
		glVertex3f(xpos,(y-ymin)*yfac+ypos,0.0f);
		while (x < xmax-stepsize) {
			x += stepsize;
			y = f(x);
			glVertex3f((x-xmin)*xfac+xpos,(y-ymin)*yfac+ypos,0.0f);
		}	
		y = f(xmax);
		glVertex3f(xpos+width,(y-ymin)*yfac+ypos,0.0f);	
	glEnd();
}
