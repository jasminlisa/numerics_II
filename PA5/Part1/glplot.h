#ifndef GLPLOT_H
#define GLPLOT_H

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

#endif /* GLFONT_H */
