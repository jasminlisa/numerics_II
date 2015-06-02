#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "lina.h"
#include "num_ode.h"
#include "glfont.h"
#include "glplot.h"


#define SIGMA 10
#define RHO 28
#define BETA (8/3)

/*
RHS of Lorenz-equation
@author Jan
*/
void f (double t, struct Vector* vec, struct Vector* res) {
	res->values[0] = SIGMA*(vec->values[1]-vec->values[0]);
	res->values[1] = vec->values[0]*(RHO-vec->values[2])-vec->values[1];
	res->values[2] = vec->values[0]*vec->values[1]-BETA*vec->values[2];
}



/* Run Program */
int main (int argc, char** argv) {
	
	//Read input argument
	int steps=1000;
	double tol = 0.1;
	if (argc == 1){
		printf("USAGE: NumberOfSteps Tolerance\nExited.\n");
		return 0;
	} 
	if (argc > 1) steps = atoi(argv[1]);	
	if (argc > 2) tol = atof(argv[2]);	
	printf("Input arguments: %i %f \n", steps, tol);
	
	//Check if SDL Initialization is successful
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return -1;
	}

	//Create Window
	SDL_Window *win = SDL_CreateWindow("Exercise 5, Lorenz Equations",50,50,640,480,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!win) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
	}
	
	//Initialize OpenGL
	SDL_GLContext context = SDL_GL_CreateContext(win);
	if (!context) {
		fprintf(stderr, "Could not create OpenGL context: %s\n", SDL_GetError());
	}
	glViewport(0,0,(GLsizei)640,(GLsizei)480);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glFrustum(-100,100,-100,100,0.01,100);

	//Plot Functions
	double xmin,xmax,ymin,ymax,zmin,zmax;
	double t0;
	struct Vector* v0;
	t0 = 0;
	v0 = new_Vector(DIM);
	v0->values[0] = 0.1;
	v0->values[1] = 0;
	v0->values[2] = 0;
		

	glColor4f(0.0f,0.0f,0.0f,1.0f);

	//Standard calculation
	double* t = malloc(sizeof(double)*steps);
	double* xValues = malloc(sizeof(double)*steps);
	double* yValues = malloc(sizeof(double)*steps);
	double* zValues = malloc(sizeof(double)*steps);
	struct Vector** vec = malloc(sizeof(struct Vector*)*steps);	
	for (int j=1; j < steps; j++) {
		vec[j] = new_Vector(DIM);
	}

	//For Exercise 4: Adaptive Runge-Kutta of Order 3
	adaptive_rk3(f, t0, v0, tol, t, vec, steps);
	glColor4f(1.0f,0.0f,0.0f,1.0f);
	for (int j=0; j < steps; j++) {
		xValues[j] = vec[j]->values[0];
		yValues[j] = vec[j]->values[1];
		zValues[j] = vec[j]->values[2];
	}
	//Calculation of the xmin,xmax,ymin,ymax values to init viewpoint
	xmin = getMinimum(xValues, steps);
	xmax = getMaximum(xValues, steps);
	ymin = getMinimum(yValues, steps);
	ymax = getMaximum(yValues, steps);
	zmin = getMinimum(zValues, steps);
	zmax = getMaximum(zValues, steps);
	
	
	SDL_GL_SwapWindow(win);

	
	//Look Around Loop
	bool loop = true;
	double xMouse = 0;
	double yMouse = 0;
	double xpos = 0;
	double ypos = 0;
	double zpos = 0;
	bool mouseDown = false;
	double mouseZoom = 0.01;
	SDL_Event event;
	while (loop) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loop = false;

			}
			if (event.type== SDL_MOUSEBUTTONDOWN) {
				mouseDown = true;
			}
			if (event.type == SDL_MOUSEBUTTONUP) {
				mouseDown = false;
			}
			if (event.type == SDL_MOUSEMOTION) {
				if(mouseDown){
					xMouse += 0.5*event.motion.xrel;
					yMouse += 0.5*event.motion.yrel;
				}
			}
			if (event.type== SDL_KEYDOWN) {
				if(event.key.keysym.scancode == SDL_SCANCODE_W)
					ypos += 0.1;
				if(event.key.keysym.scancode == SDL_SCANCODE_S)
					ypos -= 0.1;
				if(event.key.keysym.scancode == SDL_SCANCODE_A)
					xpos -= 0.1;
				if(event.key.keysym.scancode == SDL_SCANCODE_D)
					xpos += 0.1;
				if(event.key.keysym.scancode == SDL_SCANCODE_Q)
					zpos += 0.1;
				if(event.key.keysym.scancode == SDL_SCANCODE_E)
					zpos -= 0.1;
			}
			if (event.type== SDL_MOUSEWHEEL) {
				mouseZoom += 0.01*event.wheel.y;
			}
		}
		
		//Draw current frame buffer
		SDL_GL_SwapWindow(win);	

		//Prepare next frame buffer
		glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        	glClear( GL_COLOR_BUFFER_BIT);
        	glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
        	glMatrixMode(GL_PROJECTION);
        	glLoadIdentity();
        	glMatrixMode(GL_MODELVIEW);
        	glLoadIdentity();
		
		glScaled(mouseZoom,mouseZoom,mouseZoom);
		glRotated(-90, 1.0, 0.0, 0.0);
		glRotated(-yMouse, -1.0, 0.0, 0.0);
		glRotated(xMouse, 0.0, 0.0, 1.0);

		glTranslated(xpos, ypos, zpos);

		glColor4f(0.0,0.0,0.0,0.0);
		plotAxis3D(xmin-2,xmax+2,ymin-2,ymax+2,zmin-2,zmax+2,0,0,0);
		glColor4f(1.0,0.0,0.0,0.0);
		plotArrayPoints3D(xValues, yValues, zValues, steps);
	}


	//Clean Up Memory					
	free(t);
	for (int j=1; j < steps; j++) {
		delete_Vector(vec[j]);
	}
	free(vec);
	free(xValues);
	free(yValues);
	free(zValues);
	delete_Vector(v0);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
