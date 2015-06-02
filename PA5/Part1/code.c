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


/*
The first function of the PA (for exercise 1)
@author Alex
*/
void f1 (double t, struct Vector* vec, struct Vector* res) {
	scale_Vector(-2,vec,res);
}

/*
The second function of the PA (fir exercise 2)
@author Alex
*/
void f2 (double t, struct Vector* vec, struct Vector* res) {
	res->values[0] = -1001*vec->values[0]+999*vec->values[1];
	res->values[1] =  999*vec->values[0]-1001*vec->values[1];
}


/*Run Program.
Copied from PA4 Part 1, still working on it
@author Alex
*/
int main (int argc, char** argv) {
	
	//Read input argument
	int in = 0;
	double h = 0.5;
	int steps=1000;
	int orbit = 1;
	if (argc == 1){
		printf("USAGE: NumberOfExercise stepSize NumberOfSteps Tolerance\nExited.\n");
		return 0;
	} 
	if (argc > 1) in = atoi(argv[1]);	
	if (argc > 2) h = atof(argv[2]);
	if( argc > 3) steps = atoi(argv[3]);	
	if (argc > 4) orbit = atoi(argv[4]);	
	//printf("Input arguments: %i %f %i %f \n", in, h, steps, tol);
	
	//Check if SDL Initialization is successful
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return -1;
	}

	//Create Window
	SDL_Window *win = SDL_CreateWindow("Exercise 5",50,50,640,480,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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
	glDisable(GL_DEPTH_TEST);

	//Plot Functions
	double xmin,xmax,ymin,ymax,t0;
	struct Vector* y0;
	void (*f)(double,struct Vector*,struct Vector*);
	switch(in) {
		case 1: //Case of exercise 1. This should be finished
			t0 = 1;
			y0 = new_Vector(DIM);
			y0->values[0] = 3*expl(-2)+0.001;
			y0->values[1] = 3*expl(-2);
			f = f1;
		break;

		case 2: //Case of exercise 2. This should be finished 
			t0 = 1;
			y0 = new_Vector(DIM);
			y0->values[0] = 3*expl(-2)+0.001;
			y0->values[1] = 3*expl(-2);
			f = f2;
		break;
		
		default:
			printf("Invalid argument: %i\n",in);
		break;	
	}

	glColor4f(0.0f,0.0f,0.0f,1.0f);

	//Standard calculation
	double* t = malloc(sizeof(double)*steps);
	double* yValues = malloc(sizeof(double)*steps);
	double* xValues = malloc(sizeof(double)*steps);
	struct Vector** y = malloc(sizeof(struct Vector*)*steps);	
	for (int j=1; j < steps; j++) {
		y[j] = new_Vector(DIM);
	}

	//For Exercise 1: Simply do the explicit euler
	euler (f, t0, y0, h, t, y, steps);
	glColor4f(1.0f,0.0f,0.0f,1.0f);
	for (int j=0; j < steps; j++) {
		xValues[j] = y[j]->values[0];
		yValues[j] = y[j]->values[1];
	}
	//Dynamic calculation of the xmin,xmax,ymin,ymax values for better plotting
	xmin = getMinimum(xValues, steps);
	xmax = getMaximum(xValues, steps);
	ymin = getMinimum(yValues, steps);
	ymax = getMaximum(yValues, steps);
	//printf("test: %f %f %f %f \n ", xmin, xmax, ymin, ymax);
	
	if (orbit == 0) { //3d plotting (time is z axis)

		//Look Around Loop
		bool loop = true;
		double xMouse = 0;
		double yMouse = 0;
		double xpos = 0;
		double ypos = 0;
		double zpos = 0;
		bool mouseDown = false;
		double mouseZoom = 0.1;
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
					if (event.wheel.y>0) {
						mouseZoom *= 2;
					}
					if (event.wheel.y<0) {
						mouseZoom /= 2;
					}
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
		
			glScaled(mouseZoom,mouseZoom,0.5*mouseZoom);
			glRotated(-90, 1.0, 0.0, 0.0);
			glRotated(-yMouse, -1.0, 0.0, 0.0);
			glRotated(xMouse, 0.0, 0.0, 1.0);

			glTranslated(xpos, ypos, zpos);

		
			//Plot as array points (otherwise nothing interesting happens)
			glColor4f(0.0,0.0,0.0,0.0);
			plotAxis3D(-xmax,xmax,-ymax,ymax,t0,t0+steps*h+1,0,0,t0);
			//plotArrayPoints(xValues, yValues, steps, xmin, xmax, ymin, ymax, xpos, ypos, width, height);
			plotArrayPoints3D(xValues,yValues,t,steps);
			glColor4f(0.0,0.0,0.0,0.0);
		}
	} else { //2d orbit plotting
			//Plot Functions
			double xpos = -0.9;
			double ypos = -0.9;
			double width = 1.8;
			double height = 1.8;
			glColor4f(0.0f,0.0f,0.0f,1.0f);
			glBegin(GL_LINE_LOOP);
				glVertex3f(xpos,ypos,0.0f);
				glVertex3f(xpos+width,ypos,0.0f);
				glVertex3f(xpos+width,ypos+height,0.0f);
				glVertex3f(xpos,ypos+height,0.0f);
			glEnd();
			glBegin(GL_LINES);
				for (int j=1; j<10; j++) {
					glVertex3f(xpos+j*width/10,ypos,0.0f);
					glVertex3f(xpos+j*width/10,ypos+0.02f,0.0f);
					glVertex3f(xpos+j*width/10,ypos+height,0.0f);
					glVertex3f(xpos+j*width/10,ypos+height-0.02f,0.0f);
					glVertex3f(xpos,ypos+j*height/10,0.0f);
					glVertex3f(xpos+0.02f,ypos+j*height/10,0.0f);
					glVertex3f(xpos+width,ypos+j*height/10,0.0f);
					glVertex3f(xpos+width-0.02f,ypos+j*height/10,0.0f);
				}
			glEnd();

			//Plot as array points (otherwise nothing interesting happens)
			glColor4f(0.0,0.0,0.0,0.0);
			plotArrayPoints(xValues, yValues, steps, xmin, xmax, ymin, ymax, xpos, ypos, width, height);
			SDL_GL_SwapWindow(win);

			//Wait for Window Close Event
			SDL_Event event;
			while (SDL_WaitEvent(&event) >= 0) {
				if (event.type == SDL_QUIT) {
					SDL_GL_DeleteContext(context);
					SDL_DestroyWindow(win);
					SDL_Quit();
					break;
				}
			}
	}

	
	//Clean Up Memory					
	free(t);
	for (int j=1; j < steps; j++) {
		delete_Vector(y[j]);
	}
	free(y);
	free(yValues);
	free(xValues);
	delete_Vector(y0);

	SDL_GL_SwapWindow(win);
	return 0;
}
