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



//Run Program
int main (int argc, char** argv) {
		
	//Check if SDL Initialization is successful
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return -1;
	}

	//Create Window
	SDL_Window *win = SDL_CreateWindow("Title",50,50,640,480,SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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
	buildFont();

	//Initialize Variables
	//pass


	//Animation Loop
	bool loop = true;
	int count = 0;
	char* fps = malloc(sizeof(char)*32);
	SDL_Event event;
	unsigned int fps_cap = 200;
	unsigned int timer;
	while (loop) {

		//Start frame timer
		timer = SDL_GetTicks();
		
		//Handle events 
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				loop = false;
			}
		}
				
		//Draw current frame buffer
		SDL_GL_SwapWindow(win);	

		//Prepare next frame buffer
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(0.0f,0.0f,0.0f,1.0f);
		//calculations and plotting here
		
		//Wait for next frame, if necessary (FPS cap) 
		if (SDL_GetTicks()-timer < 1000/fps_cap) {
			SDL_Delay(1000/fps_cap-SDL_GetTicks()+timer);
		}
		if (count > 10) { //redraw fps count only every 10 frames
			sprintf(fps,"Frames per Second: %i",1000/(SDL_GetTicks()-timer));
			count = 0;
		}
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glRasterPos3f(-0.94f, 0.94f, 0.0f);
		printGLs(fps);
		count++;
	}
			
	//Clean Up Memory	
	free(fps);				
	deleteFont();
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
