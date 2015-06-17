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

	int steps=100;
	//double gamma=0.001;
	struct Matrix* a=eye(2);
	a->values[0][0]=9;
	struct Vector* b=new_Vector(2);
	b->values[0]=1;
	b->values[1]=1;
	struct Vector* initialVal=new_Vector(2);
	initialVal->values[0]=1;
	initialVal->values[1]=1;
	struct Vector** x = malloc(sizeof(struct Vector*)*steps);	
	for (int j=0; j < steps; j++) {
		x[j] = new_Vector(DIM);
	}
	//struct Vector* y=new_Vector(steps);
	//gradientDescent(a,b,initialVal,x,y,gamma,2,steps);
	struct Vector* result = conjugateGradient(a,b,initialVal,x,2, 0.001, steps);
	if(result!=NULL){
		print_Vector(result);
	}else{
		for (int j=1; j < steps; j++) {
			print_Vector(x[j]);
		}
		return 0;
	}
}
