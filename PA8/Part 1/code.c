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



	//initialize matrices and vectors
	struct Matrix* a=eye(DIM);
	generateKd(DIM,a);

	struct Vector* b=new_Vector(DIM);
	int i;
	for(i=0; i<DIM; i++){
		b->values[i]=1;
	}
	
	struct Vector* initialVal=new_Vector(DIM);
	for(i=0; i<DIM; i++){
		initialVal->values[i]=0;
	}

	
	bool loop=true;
	SDL_Event event;

	//Exercise 4 output
	struct Vector* eigenvalues = new_Vector(DIM);
	eigenvalues_K_n(DIM,eigenvalues);
	print_Vector(eigenvalues);
	delete_Vector(eigenvalues);
	printf("Condition Number for Dimension %d: %f\n",DIM,condition_number_K_n(DIM));

	while (loop) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loop = false;
			}
			if (event.type== SDL_KEYDOWN) {
				if(event.key.keysym.scancode == SDL_SCANCODE_S){
					steps *= 2;
				}
				if(event.key.keysym.scancode == SDL_SCANCODE_A){
					if (steps > 2) {
						steps /= 2;
					} else {
						steps =1;
					}
				}
			}
		}




		struct Vector* fx = new_Vector(steps);	
	
		double* xvalues=malloc(sizeof(double)*steps);
		for(i=0; i<steps; i++){
			xvalues[i]=i;
		}	


		double* yvalues=malloc(sizeof(double)*steps);
	

		struct Vector** x = malloc(sizeof(struct Vector*)*steps);	
		for (i=0; i < steps; i++) {
			x[i] = new_Vector(DIM); //Vorher: DIM	
		}
		double ymax = 0;
		if(method==0){
			ymax = DIM;
			gradientDescent(a,b,initialVal,x,gamma,DIM,steps);
			for(int i=0;i<steps;i++){
		
					fx->values[i]=normAxb_squared(a,b,x[i],DIM);			
			
			
			
				yvalues[i]=fx->values[i];

			}

			//print_Vector(x[steps-1]);
		}else if (method==1) {
			ymax = DIM*DIM/2;
			int lastIndex = conjugateGradient(a,b,initialVal,x,DIM, gamma, steps);
			//printf("%d",lastIndex);
			if(lastIndex!=-1){
				//print_Vector(x[lastIndex]);
			}else{
				for (int j=0; j < steps; j++) {
					//print_Vector(x[j]);
				}		
			}
			for(int i=0;i<steps;i++){
		
					fx->values[i]=normAxb_squared(a,b,x[i],DIM);			
					//print_Vector(fx);
			
			
				yvalues[i]=fx->values[i];

			}
		}
		
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
			glColor4f(1.0,0.0,1.0,0.0);
			glPointSize(5);
			plotArrayPoints(xvalues, yvalues, steps, 0, steps, 0, ymax , xpos, ypos, width, height);
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
	}
	delete_Vector(b);
	delete_Matrix(a);
	delete_Vector(initialVal);
	return 0;
}
