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



/* Run Program */
int main (int argc, char** argv) {

	//Some input reading for easier use, including which method should be used
	int steps=1000;
	if (argc > 1) steps = atoi(argv[1]);
	double gamma=0.001;
	if (argc>2) gamma = atof(argv[2]);
	double method = 0;
	if (argc>3) method = atoi(argv[3]);
	struct Matrix* a=eye(DIM);
	change_Matrix(a,0,0,4);
	change_Matrix(a,1,1,2);
	struct Vector* b=new_Vector(DIM);
	int i;
	for(i=0; i<DIM; i++){
		b->values[i]=1;
	}
	/*b->values[1]=1;
	b->values[2]=1;
	b->values[3]=1;
	b->values[4]=1;*/
	struct Vector* initialVal=new_Vector(DIM);
	for(i=0; i<DIM; i++){
		initialVal->values[i]=1;
	}
	/*initialVal->values[1]=1;
	initialVal->values[2]=1;
	initialVal->values[3]=1;
	initialVal->values[4]=1;*/
	struct Vector** x = malloc(sizeof(struct Vector*)*steps);	
	for (i=0; i < steps; i++) {
		x[i] = new_Vector(DIM); //Vorher: DIM
	}
	if(method==0){
		gradientDescent(a,b,initialVal,x,gamma,DIM,steps);
		print_Vector(x[steps-1]);
	}else{
		int lastIndex = conjugateGradient(a,b,initialVal,x,DIM, gamma, steps);
		if(lastIndex!=-1){
			print_Vector(x[lastIndex]);
		}else{
			for (int j=0; j < steps; j++) {
				print_Vector(x[j]);
			}		
		}
	}
	for(i=0;i<steps;i++){
		delete_Vector(x[i]);
	}
	free(x);
	delete_Vector(b);
	delete_Matrix(a);
	delete_Vector(initialVal);
	return 0;
}
