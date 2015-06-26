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

/* Paraboloid Manifold Parametrization */
void paraboloid (struct Matrix* X, struct Matrix* Y, struct Matrix* Z) {
	for (int i=0; i< X->height; i++) {
		for(int j=0; j<X->width; j++) {
			Z->values[i][j] = X->values[i][j]*X->values[i][j]+Y->values[i][j]*Y->values[i][j];
		}
	}
}

/* Iterative method to construct geodesics on the paraboloid */
void geodesic_step(struct Vector* vec, double h, struct Vector* res) {
	double tmpx = vec->values[0]+h*vec->values[3];
	double tmpy = vec->values[1]+h*vec->values[4];
	double tmpz = vec->values[2]+h*vec->values[5];
	double lambda = 0;
	if (tmpx==0 && tmpy==0) {
		res->values[0] = 0;
		res->values[1] = 0;
		res->values[2] = 0;
		res->values[3] = 1/h * -vec->values[0];
		res->values[4] = 1/h * -vec->values[1];
		res->values[5] = 0;
	} else {
		lambda = (-4*tmpx*tmpx-4*tmpy*tmpy-1+sqrt(16*tmpx*tmpx*tmpz+8*tmpx*tmpx+16*tmpy*tmpy*tmpz+8*tmpy*tmpy+1))/(8*(tmpx*tmpx+tmpy*tmpy));
		res->values[0] = tmpx + 2*lambda*tmpx;
		res->values[1] = tmpy + 2*lambda*tmpy;
		res->values[2] = tmpz - lambda;
		double diffx = res->values[0]-vec->values[0];
		double diffy = res->values[1]-vec->values[1];
		double diffz = res->values[2]-vec->values[2];
		double numerator = -(2*diffx*res->values[0]+2*diffy*res->values[1]-diffz);
		double denominator = 4*res->values[0]*res->values[0]+4*res->values[1]*res->values[1]+1;
		res->values[3] = 1/h * (diffx + 2*numerator/denominator*res->values[0]);
		res->values[4] = 1/h * (diffy + 2*numerator/denominator*res->values[1]);
		res->values[5] = 1/h * (diffz - numerator/denominator);
	}		
}

/* RHS to describe a geodesic on the paraboiloid manifold */
void geodesic_rhs (double t, struct Vector* vec, struct Vector* res) {
	res->values[0] = vec->values[3]; //d/dt x = a
	res->values[1] = vec->values[4]; //d/dt y = b
	res->values[2] = vec->values[5]; //d/dt z = c
	res->values[3] = -vec->values[0] * (4* (vec->values[3]*vec->values[3]+vec->values[4]*vec->values[4]))/(1+4*(vec->values[0]*vec->values[0]+vec->values[1]*vec->values[1])); //d/dt a
	res->values[4] = -vec->values[1] * (4* (vec->values[3]*vec->values[3]+vec->values[4]*vec->values[4]))/(1+4*(vec->values[0]*vec->values[0]+vec->values[1]*vec->values[1])); //d/dt b
	res->values[5] = (2* (vec->values[3]*vec->values[3]+vec->values[4]*vec->values[4]))/(1+4*(vec->values[0]*vec->values[0]+vec->values[1]*vec->values[1])); //d/dt c
}

/* Run Program */
int main (int argc, char** argv) {
	
	//Read input argument
	int steps=1000;
	double h = 0.1;
	if (argc == 1){
		printf("USAGE: NumberOfSteps StepSize/Tolerance\nExited.\n");
		return 0;
	} 
	if (argc > 1) steps = atoi(argv[1]);	
	if (argc > 2) h = atof(argv[2]);	
	printf("Input arguments: %i %f \n", steps, h);
	
	//Check if SDL Initialization is successful
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return -1;
	}

	//Create Window
	SDL_Window *win = SDL_CreateWindow("Exercise 5, Lorenz Equations",50,50,800,600,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!win) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
	}
	
	//Initialize OpenGL
	SDL_GLContext context = SDL_GL_CreateContext(win);
	if (!context) {
		fprintf(stderr, "Could not create OpenGL context: %s\n", SDL_GetError());
	}
	glViewport(0,0,(GLsizei)800,(GLsizei)600);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glFrustum(-100,100,-100,100,0.01,100);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1,1);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	GLfloat lightcol[] = {0.9,0.9,0.9,1.0};
	GLfloat lightamb[] = {0.5,0.5,0.5,1.0};
	GLfloat lightpos[] = {20.0,20.0,30.0,1.0};
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightamb);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightcol);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_RESCALE_NORMAL);
	

	//Init all vectors
	double t0 = 0;
	double* t = malloc(sizeof(double)*steps);
	double* xValues = malloc(sizeof(double)*steps);
	double* yValues = malloc(sizeof(double)*steps);
	double* zValues = malloc(sizeof(double)*steps);
	double* xValues_it = malloc(sizeof(double)*steps);
	double* yValues_it = malloc(sizeof(double)*steps);
	double* zValues_it = malloc(sizeof(double)*steps);
	double xmin = -4;
	double xmax = 4;
	double ymin = -4;
	double ymax = 4;
	double xsteps = 21;
	double ysteps = 21;
	struct Matrix* X = new_Matrix(xsteps,ysteps);
	struct Matrix* Y = new_Matrix(xsteps,ysteps);
	struct Matrix* Z = new_Matrix(xsteps,ysteps);	
	meshgrid(xmin,xmax,ymin,ymax,xsteps,ysteps,X,Y);
	paraboloid(X,Y,Z);
	struct Vector* vec0 = new_Vector(DIM);
	vec0->values[0] = 1;
	vec0->values[1] = 1;
	vec0->values[2] = 2;
	vec0->values[3] = 1-0.25;
	vec0->values[4] = -1-0.25;
	vec0->values[5] = -1;
	struct Vector** vec = malloc(sizeof(struct Vector*)*steps);	
	for (int j=1; j < steps; j++) {
		vec[j] = new_Vector(DIM);
	}
	
	//find geodesic iteratively
	struct Vector** vec_it = malloc(sizeof(struct Vector*)*steps);
	vec_it[0] = vec0;	
	for (int j=1; j < steps; j++) {
		vec_it[j] = new_Vector(DIM);
		geodesic_step(vec_it[j-1],h,vec_it[j]);		
	}
	for (int j=0; j < steps; j++) {
		xValues_it[j] = vec_it[j]->values[0];
		yValues_it[j] = vec_it[j]->values[1];
		zValues_it[j] = vec_it[j]->values[2];
	}

	//solve ODE
	//adaptive_rk3(geodesic_rhs,t0,vec0,h,t,vec,steps);
	euler(geodesic_rhs,t0,vec0,h,t,vec,steps);

	for (int j=0; j < steps; j++) {
		xValues[j] = vec[j]->values[0];
		yValues[j] = vec[j]->values[1];
		zValues[j] = vec[j]->values[2];
	}
	
	SDL_GL_SwapWindow(win);

	/* Look Around Loop	*/
	bool loop = true;
	bool wireframe = false;
	bool overlay = true;
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
				if(event.key.keysym.scancode == SDL_SCANCODE_TAB)
					wireframe = !wireframe;
				if(event.key.keysym.scancode == SDL_SCANCODE_O)
					overlay = !overlay;
			}
			if (event.type== SDL_MOUSEWHEEL) {
				if (event.wheel.y < 0)
					mouseZoom /= 1.2;
				else
					mouseZoom *= 1.2;
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
		
		glTranslated(xpos, ypos, zpos);
		glScaled(mouseZoom,mouseZoom,mouseZoom);
		glRotated(-90, 1.0, 0.0, 0.0);
		glRotated(-yMouse, 1.0, 0.0, 0.0);
		glRotated(-xMouse, 0.0, 0.0, 1.0);


		plotAxis3D(-10,10,-10,10,-5,30,0,0,0);
		glColor4f(0.2,0.4,0.5,1.0);
		glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			surf(X,Y,Z);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			surf(X,Y,Z);
			if (overlay) {
				glColor4f(0.2,0.2,0.2,1.0);
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				surf(X,Y,Z);
			}
		}
		
		glColor4f(0.85,0.85,0.10,1.0);
		plotArray3D(xValues,yValues,zValues,steps);
		glColor4f(0.9,0.2,0.4,1.0);
		plotArray3D(xValues_it,yValues_it,zValues_it,steps);
		glColor4f(0.0,0.0,0.0,1.0);
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
	delete_Vector(vec0);
	delete_Matrix(X);
	delete_Matrix(Y);
	delete_Matrix(Z);

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
