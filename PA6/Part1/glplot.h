#ifndef GLPLOT_H
#define GLPLOT_H

#include "lina.h"

/*
 * Draws a set of 2-dimensional data as lines on an OpenGl canvas
 * INPUTS:	@param x	-	the x coordinates as an array
 *		@param y	-	the y coordinates as an array
 *		@param size	-	the length of the data arrays (x and y need to have equal length)
 *		@param xmin	-	the mininal x coordinate to be drawn
 *		@param xmax	-	the maximal x coordinate to be drawn
 *		@param ymin	-	the minimal y coordinate to be drawn
 *		@param ymax	-	the maximal y coordinate to be drawn
 *		@param xpos	-	on-screen opengl x coordinate of the lower left corner of the canvas
 *		@param ypos 	- 	on-screen opengl y coordinate of the lower left corner of the canvas
 *		@param width	-	on-screen opengl width of the canvas
 *		@param height	-	on-screen opengl height of the canvas
 *
 * OUTPUT: 	@return 	-	none
 */
void plotArray (double* x, double* y, int size, double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height);

/*
 * Draws a set of 2-dimensional data as points on an OpenGl canvas
 * INPUTS:	@param x	-	the x coordinates as an array
 *		@param y	-	the y coordinates as an array
 *		@param size	-	the length of the data arrays (x and y need to have equal length)
 *		@param xmin	-	the mininal x coordinate to be drawn
 *		@param xmax	-	the maximal x coordinate to be drawn
 *		@param ymin	-	the minimal y coordinate to be drawn
 *		@param ymax	-	the maximal y coordinate to be drawn
 *		@param xpos	-	on-screen opengl x coordinate of the lower left corner of the canvas
 *		@param ypos 	- 	on-screen opengl y coordinate of the lower left corner of the canvas
 *		@param width	-	on-screen opengl width of the canvas
 *		@param height	-	on-screen opengl height of the canvas
 *
 * OUTPUT: 	@return		-	none
 */
void plotArrayPoints (double* x, double* y, int size, double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height);

/*
 * Draws a set of 3-dimensional data as points on an OpenGl canvas
 * INPUTS:	@param x	-	the x coordinates as an array
 *		@param y	-	the y coordinates as an array
 *		@param z	- 	the z coordinate as an
 *		@param size	-	the length of the data arrays (x and y and z need to have equal length)
 *
 * OUTPUT: 	@return		-	none
 */
void plotArrayPoints3D (double* x, double* y, double* z, int size);

/*
 * Draws a set of 3-dimensional data as line strip on an OpenGl canvas
 * INPUTS:	@param x	-	the x coordinates as an array
 *		@param y	-	the y coordinates as an array
 *		@param z	- 	the z coordinate as an
 *		@param size	-	the length of the data arrays (x and y and z need to have equal length)
 *
 * OUTPUT: 	@return		-	none
 */
void plotArray3D (double* x, double* y, double* z, int size);

/*
 * Draws 3-dimensional cartesian axis. Axis intersect at the point given as origin.
 * INPUTS:	@param xmin	-	the minimum x coordinate
 *		@param xmax	-	the maximum x coordinate
 *		@param ymin	-	the minimum y coordinate
 *		@param ymax	-	the maximum y coordinate
 *		@param zmin	-	the minimum z coordinate
 *		@param zmax	-	the maximum z coordinate
 *		@param xorigin	-	the origin x coordinate
 *		@param yorigin	-	the origin y coordinate
 *		@param zorigin	-	the origin z coordinate
 *
 * OUTPUT: 	@return		-	none
 */
void plotAxis3D (double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, double xorigin, double yorigin, double zorigin);

/*
 * Draws a the graph of a function on an OpenGl canvas
 * INPUTS:	@param f	-	handle for the function to be drawn
 *		@param xmin	-	the mininal x coordinate to be drawn
 *		@param xmax	-	the maximal x coordinate to be drawn
 *		@param ymin	-	the minimal y coordinate to be drawn
 *		@param ymax	-	the maximal y coordinate to be drawn
 *		@param xpos	-	on-screen opengl x coordinate of the lower left corner of the canvas
 *		@param ypos 	- 	on-screen opengl y coordinate of the lower left corner of the canvas
 *		@param width	-	on-screen opengl width of the canvas
 *		@param height	-	on-screen opengl height of the canvas
 *		@param stepsize	-	distance of the evaluation points (linear interpoltation will be used in between)
 *
 * OUTPUT: 	@return 	-	none
 */
void plotFunction (double (*f)(double), double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height, double stepsize);

/*
 * Creates a Matlab-like meshgrid
 * INPUTS:	@param xmin		-	the mininal x coordinate
 *			@param xmax		-	the maximal x coordinate
 *			@param ymin		-	the minimal y coordinate
 *			@param ymax		-	the maximal y coordinate
 *			@param xsteps	-	number of gridpoints in the x direction
 *			@param ysteps	-	number of gridpoints in the y direction
 *			@param X		-	stores the resulting grids x coordinates
 *			@param Y		-	stores the resulting grids y coordinates
 *
 * OUTPUT: 	@return 	-	none
 */
void meshgrid (double xmin, double xmax, double ymin, double ymax, int xsteps, int ysteps, struct Matrix* X, struct Matrix* Y);

/*
 * Plots a 3D surface
 * INPUTS:	@param X		-	the x coordinate mehsgrid
 *			@param Y		-	the y coordinate meshgrid
 *			@param Z		-	the z coordinate meshgrid
 *
 * OUTPUT: 	@return 	-	none
 */
void surf(struct Matrix* X, struct Matrix* Y, struct Matrix* Z);

#endif /* GLFONT_H */
