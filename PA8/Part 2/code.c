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

/* Calculate eigenvalues of K_n */
void eigenvalues_K_n (int dim, struct Vector* res) {
	for (int k = 0; k < dim; k++) {
		res->values[k] = -1/(float)(dim+1) * (2 - 2* cos((k+1)*PI/(dim+1)));
	}
}

/* Calculate condition number of K_n */
double condition_number_K_n (int dim) {
	return (2 - 2* cos(dim*PI/(dim+1)))/(2 - 2* cos(PI/(dim+1)));
} 

/* Run Program */
int main (int argc, char** argv) {

	//Some input reading for easier use, including which method should be used
	int steps=100;
	if (argc > 1) steps = atoi(argv[1]);
	double gamma=0.01;
	if (argc>2) gamma = atof(argv[2]);
	double method = 0;
	if (argc>3) method = atoi(argv[3]);

	int dim = 1;

	
	//Check if SDL Initialization is successful
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return -1;
	}

	//Create Window
	SDL_Window *win = SDL_CreateWindow("Exercise 5",50,50,1024,800,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!win) {
		fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
	}
	
	//Initialize OpenGL
	SDL_GLContext context = SDL_GL_CreateContext(win);
	if (!context) {
		fprintf(stderr, "Could not create OpenGL context: %s\n", SDL_GetError());
	}
	glViewport(0,0,(GLsizei)1024,(GLsizei)800);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);


	
	bool loop=true;
	SDL_Event event;

	while (loop) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loop = false;
			}
			if (event.type== SDL_KEYDOWN) {
				if(event.key.keysym.scancode == SDL_SCANCODE_S){
					dim = (dim +1)*2-1;
				}
				if(event.key.keysym.scancode == SDL_SCANCODE_A){
					if (dim > 1) {
						dim = (dim +1)/2-1;
					} else {
						dim = 1;
					}
				}
			}
		}

		//initialize matrices and vectors
		struct Matrix* a=eye(dim);
		generateKd(dim,a);

		struct Vector* b=new_Vector(dim);
		int i;
		for(i=0; i<dim; i++){
			b->values[i]=1;
		}
	
		struct Vector* initialVal=new_Vector(dim);
		for(i=0; i<dim; i++){
			initialVal->values[i]=0;
		}

	
		double* xvalues=malloc(sizeof(double)*dim);
		for(i=0; i<dim; i++){
			xvalues[i]=(i+1)/(float)(dim+1);
		}	


		double* yvalues=malloc(sizeof(double)*dim);
	

		struct Vector** x = malloc(sizeof(struct Vector*)*steps);	
		for (i=0; i < steps; i++) {
			x[i] = new_Vector(dim);
		}
		if(method==0){
			gradientDescent(a,b,initialVal,x,gamma,dim,steps);
			for (int i=0; i<dim; i++) {
				yvalues[i] = x[steps-1]->values[i];
			}
			//print_Matrix(a);
		}else if (method==1) {
			int lastIndex = conjugateGradient(a,b,initialVal,x,dim, gamma, steps);
			for (int i=0; i<dim; i++) {
				yvalues[i] = x[lastIndex]->values[i];
			}
			//print_Matrix(a);
		}
		
			//Plot Functions
			double xpos = -0.9;
			double ypos = -0.9;
			double width = 1.8;
			double height = 1.8;
			
			//printf("dim %d\n", dim);
			//for(int i=0; i<dim; i++) {
			//	printf("xval %f, yval %f\n", xvalues[i], yvalues[i]);
			//}
			//printf("\n");


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
			glColor4f(1.0,0.0,1.0,0.0);
			glPointSize(5);
			plotArrayPoints(xvalues, yvalues, dim, 0, 1, -0.25, 0 , xpos, ypos, width, height);
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

			
	

		for(i=0;i<steps;i++){
			delete_Vector(x[i]);
		}
	
		free(x);
		free(xvalues);
		free(yvalues);
		delete_Matrix(a);
		delete_Vector(b);
		delete_Vector(initialVal);
	}
	return 0;
}
