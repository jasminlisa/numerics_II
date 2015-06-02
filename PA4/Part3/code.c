#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

#include "lina.h"
#include "num_ode.h"
#include "glfont.h"

struct Vector* force;

//Define Functions f for Right side of ODE/IVP
void gravity_RHS (double t, struct Vector* vec, struct Vector* res) {
	res->values[0] = vec->values[2];
	res->values[1] = vec->values[3];
	double norm = sqrt(vec->values[0]*vec->values[0]+vec->values[1]*vec->values[1]);
	res->values[2] = -1/(norm*norm*norm)*vec->values[0]+force->values[0];
	res->values[3] = -1/(norm*norm*norm)*vec->values[1]+force->values[1];
}

void plotArray (double* x, double* y, int size, double xmin, double xmax, double ymin, double ymax, double xpos, double ypos, double width, double height) {
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

void drawArrows(bool up, bool down) {
	if (up) {
		glColor4f(0.0f,0.0f,0.0f,1.0f);
	} else {
		glColor4f(0.9f,0.9f,0.9f,1.0f);
	}			
	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.94,0.9,0.0);
		glVertex3f(-0.84,0.9,0.0);
		glVertex3f(-0.84,0.8,0.0);
		glVertex3f(-0.94,0.8,0.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.89,0.885,0.0);
		glVertex3f(-0.92,0.86,0.0);
		glVertex3f(-0.90,0.86,0.0);
		glVertex3f(-0.90,0.815,0.0);
		glVertex3f(-0.88,0.815,0.0);
		glVertex3f(-0.88,0.86,0.0);
		glVertex3f(-0.86,0.86,0.0);
	glEnd();
	if (down) {
		glColor4f(0.0f,0.0f,0.0f,1.0f);
	} else {
		glColor4f(0.9f,0.9f,0.9f,1.0f);
	}
	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.94,0.75,0.0);
		glVertex3f(-0.84,0.75,0.0);
		glVertex3f(-0.84,0.65,0.0);
		glVertex3f(-0.94,0.65,0.0);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.89,0.665,0.0);
		glVertex3f(-0.92,0.69,0.0);
		glVertex3f(-0.90,0.69,0.0);
		glVertex3f(-0.90,0.735,0.0);
		glVertex3f(-0.88,0.735,0.0);
		glVertex3f(-0.88,0.69,0.0);
		glVertex3f(-0.86,0.69,0.0);
	glEnd();
}


//Run Program
int main (int argc, char** argv) {
		
	//Check if SDL Initialization is successful
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return -1;
	}

	//Create Window
	SDL_Window *win = SDL_CreateWindow("Spaceship + Gravity",50,50,1280,1024,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!win) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
	}
	
	//Initialize OpenGL
	SDL_GLContext context = SDL_GL_CreateContext(win);
	if (!context) {
		fprintf(stderr, "Could not create OpenGL context: %s\n", SDL_GetError());
	}
	glViewport(0,0,(GLsizei)1280,(GLsizei)1024);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	buildFont();

	//Initialize Variables
	double h = 0.001;
	double xpos = -1;
	double ypos = -1;
	double width = 2;
	double height = 2;
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	double xmin,xmax,ymin,ymax,t0;
	xmin = -1;
	xmax = 1;
	ymin = -1;
	ymax = 1;
	t0 = 0;

	struct Vector* y0;
	force = new_Vector(DIM/2);
	force->values[0] = 0;
	force->values[1] = 0;
	bool up = false;
	bool down = false;
	y0 = new_Vector(DIM);
	y0->values[0] = 0.3;
	y0->values[1] = 0.5;
	y0->values[2] = -1.5;
	y0->values[3] = 0.0;
	int steps = 2; 		//do one step at a time (+initial value)
	double* t = malloc(sizeof(double)*steps);
	double* posx = malloc(sizeof(double)*steps);
	double* posy = malloc(sizeof(double)*steps);
	struct Vector** y = malloc(sizeof(struct Vector*)*steps);	
	posx[1] = y0->values[0];
	posy[1] = y0->values[1];
	plotArray(posx, posy, steps, xmin, xmax, ymin, ymax, xpos, ypos, width, height);


	//Animation Loop
	bool loop = true;
	int count = 0;
	char* fps = malloc(sizeof(char)*32);
	SDL_Event event;
	unsigned int fps_cap = 60;
	unsigned int timer;
	while (loop) {

		//Start frame timer
		timer = SDL_GetTicks();
		
		//Handle events 
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loop = false;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.scancode == SDL_SCANCODE_W) {
					up = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					down = true;
				}
			}
			if (event.type == SDL_KEYUP) {
				if (event.key.keysym.scancode == SDL_SCANCODE_W) {
					up = false;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					down = false;
				}
			}
		}
				
		//Draw current frame buffer
		SDL_GL_SwapWindow(win);	

		//Prepare next frame buffer
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(0.0f,0.0f,0.0f,1.0f);
		glBegin(GL_POINTS);
			glVertex3f(0.0,0.0,0.0);
		glEnd();
		for (int j=1; j < steps; j++) {
			y[j] = new_Vector(DIM);
		}
		if (up && !down) {
			force->values[1] = 0.1;
			drawArrows(true,false);
		} else if (down && !up) {
			force->values[1] = -0.1;
			drawArrows(false,true);
		} else {
			force->values[1] = 0.0;
			drawArrows(false,false);
		}
		symplectic_euler (gravity_RHS, t0, y0, h, t, y, steps);
		for (int j=0; j < steps; j++) {
			posx[j] = y[j]->values[0];
			posy[j] = y[j]->values[1];
		}
		glColor4f(1.0f,0.0f,0.0f,1.0f);
		plotArray(posx, posy, steps, xmin, xmax, ymin, ymax, xpos, ypos, width, height);
		//printf("y0 before: %f %f %f %f\n",y0->values[0],y0->values[1],y0->values[2],y0->values[3]);
		copy_Vector(y[1],y0);
		//printf("y0 after: %f %f %f %f\n",y0->values[0],y0->values[1],y0->values[2],y0->values[3]);
		t0 = t[1];
		
		//Wait for next frame if necessary (FPS cap) 
		if (SDL_GetTicks()-timer < 1000/fps_cap) {
			SDL_Delay(1000/fps_cap-SDL_GetTicks()+timer);
		}
		if (count > 10) { //recalculate fps only every 10 frames
			sprintf(fps,"Frames per Second: %i",1000/(SDL_GetTicks()-timer));
			count = 0;
		}
		h = (SDL_GetTicks()-timer)/1000.0;
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glRasterPos3f(-0.94f, 0.94f, 0.0f);
		printGLs(fps);
		count++;
		//SDL_Delay(h*1000);
	}
			
	//Clean Up Memory	
	free(fps);				
	free(t);
	for (int j=1; j < steps; j++) {
		delete_Vector(y[j]);
	}
	free(y);
	free(posx);
	free(posy);
	delete_Vector(y0);
	delete_Vector(force);
	deleteFont();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
