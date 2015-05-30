#ifndef GLPLOT_H
#define GLPLOT_H

/*
Draws a set of 2-dimensional data as lines on an OpenGl canvas
INPUTS:	x		-	the x coordinates as an array
		y		-	the y coordinates as an array
		size	-	the length of the data arrays (x and y need to have equal length)
		xmin	-	the mininal x coordinate to be drawn
		xmax	-	the maximal x coordinate to be drawn
		ymin	-	the minimal y coordinate to be drawn
		ymax	-	the maximal y coordinate to be drawn
		xpos	-	on-screen opengl x coordinate of the lower left corner of the canvas
		ypos 	- 	on-screen opengl y coordinate of the lower left corner of the canvas
		width	-	on-screen opengl width of the canvas
		height	-	on-screen opengl height of the canvas

OUTPUT: none
*/
void plotArray (double* x, double* y, int size, double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height);

/*
Draws a set of 2-dimensional data as points on an OpenGl canvas
INPUTS:	x		-	the x coordinates as an array
		y		-	the y coordinates as an array
		size	-	the length of the data arrays (x and y need to have equal length)
		xmin	-	the mininal x coordinate to be drawn
		xmax	-	the maximal x coordinate to be drawn
		ymin	-	the minimal y coordinate to be drawn
		ymax	-	the maximal y coordinate to be drawn
		xpos	-	on-screen opengl x coordinate of the lower left corner of the canvas
		ypos 	- 	on-screen opengl y coordinate of the lower left corner of the canvas
		width	-	on-screen opengl width of the canvas
		height	-	on-screen opengl height of the canvas

OUTPUT: none
*/
void plotArrayPoints (double* x, double* y, int size, double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height);

/*
Draws a the graph of a function on an OpenGl canvas
INPUTS:	f		-	handle for the function to be drawn
		xmin	-	the mininal x coordinate to be drawn
		xmax	-	the maximal x coordinate to be drawn
		ymin	-	the minimal y coordinate to be drawn
		ymax	-	the maximal y coordinate to be drawn
		xpos	-	on-screen opengl x coordinate of the lower left corner of the canvas
		ypos 	- 	on-screen opengl y coordinate of the lower left corner of the canvas
		width	-	on-screen opengl width of the canvas
		height	-	on-screen opengl height of the canvas
		stepsize-	distance of the evaluation points (linear interpoltation will be used in between)

OUTPUT: none
*/
void plotFunction (double (*f)(double), double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height, double stepsize);

#endif /* GLFONT_H */
