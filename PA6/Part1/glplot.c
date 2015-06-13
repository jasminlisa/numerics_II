#include <stdio.h>
#include <GL/gl.h>
#include "glplot.h"
#include "lina.h"

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

/* Plots 3D array data as points, for details see glplot.h  */
void plotArrayPoints3D (double* x, double* y, double* z, int size) {
	glPointSize(1.0f);
	double t = 0;
	for (int j=1; j<size; j++) {
		t = (double)(j)/(double)(size);
		glColor4f(0.65*t+0.85*(1-t),0.35*t+0.85*(1-t),0.25*t+0.10*(1-t),1.0);
		glBegin(GL_POINTS);
			glVertex3f(x[j],y[j],z[j]);
		glEnd();
	}	
}

/* Plots 3D array data as line strip, for details see glplot.h  */
void plotArray3D (double* x, double* y, double* z, int size) {
	double t = 0;
	glBegin(GL_LINE_STRIP);
		for (int j=1; j<size; j++) {
			t = (double)(j)/(double)(size);
			glColor4f(0.65*t+0.85*(1-t),0.35*t+0.85*(1-t),0.25*t+0.10*(1-t),1.0);
			glVertex3f(x[j],y[j],z[j]);
		}	
	glEnd();
}

/* Plots 3D cartesian axis, for details see glplot.h */
void plotAxis3D (double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, double xorigin, double yorigin, double zorigin) {
	glBegin(GL_LINES);
		glColor4f(1.0,0.0,0.0,1.0);
		glVertex3f(xmin,yorigin,zorigin);
		glVertex3f(xmax,yorigin,zorigin);
		glColor4f(0.0,1.0,0.0,1.0);
		glVertex3f(xorigin,ymin,zorigin);
		glVertex3f(xorigin,ymax,zorigin);
		glColor4f(0.0,0.0,1.0,1.0);
		glVertex3f(xorigin,yorigin,zmin);
		glVertex3f(xorigin,yorigin,zmax);
	glEnd();
}


/* Plots the graph of a function, for details see glplot.h */
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

/* Creates a Matlab-like meshgrid, for details see glplot.h */
void meshgrid (double xmin, double xmax, double ymin, double ymax, int xsteps, int ysteps, struct Matrix* X, struct Matrix* Y) {
	if ((X->height != Y->height) || (X->width != Y->width) || (X->height != xsteps) || (Y->width != ysteps)) {
		printf("WARNING: Wrong Dimensions. Trying to write a %dx%d grid into %dx%d and %dx%d matrices.\n",xsteps,ysteps,X->height,X->width,Y->height,Y->width);
		return;
	}
	double xinc = (xmax-xmin)/(xsteps-1);
	double yinc = (ymax-ymin)/(ysteps-1);
	for (int i=0; i< xsteps; i++) {
		for (int j=0; j<ysteps; j++) {
			X->values[i][j] = xmin + i*xinc;
			Y->values[i][j] = ymin + j*yinc;
		}
	}
}

/* Plots a 3D surface, for details see glplot.h */
void surf(struct Matrix* X, struct Matrix* Y, struct Matrix* Z) {
	for (int i=0; i < X->height-1; i++) {
		glBegin(GL_TRIANGLE_STRIP);
			for (int j=0; j < X->width; j++) {
				glVertex3f(X->values[i][j],Y->values[i][j],Z->values[i][j]);
				glVertex3f(X->values[i+1][j],Y->values[i+1][j],Z->values[i+1][j]);
			}
		glEnd();
	}
}
