#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "lina.h"

/* Define some basic linear algebra structs and functions. For details see lina.h */

struct Vector* new_Vector(int height) {
	struct Vector* vec = malloc(sizeof(struct Vector));
	vec->height = height;
	vec->values = calloc(height,sizeof(double));
	return vec;
}

void delete_Vector(struct Vector* v) {
	free(v->values);
	free(v);
}

void print_Vector(struct Vector* v) {
	printf("[");
	int i;
	for(i=0; i<v->height; i++) {
		if (i != v->height-1) printf("%f,\t",v->values[i]);
		else printf("%f",v->values[i]);
	}
	printf("]\n\n");
}

double vectornorm(struct Vector* v) {
	double n = 0;
	for (int i=0; i<v->height; i++) {
		n = n + v->values[i]*v->values[i];
	}
	return sqrt(n);
}

void add_Vectors(struct Vector* vec1, struct Vector* vec2, struct Vector* res) {
	if ((vec1->height != vec2->height) || (vec1->height != res->height)) {
		printf("WARNING: Wrong Dimensions. Trying to add %dx1 with %dx1 to obtain %dx1.\n",vec1->height,vec2->height,res->height);
		return;
	}
	for (int j=0; j<vec1->height; j++) {
		res->values[j] = vec1->values[j] + vec2->values[j];
	}
}

void scale_Vector(double s, struct Vector* v, struct Vector* res) {
	if (v->height != res->height) {
		printf("WARNING: Wrong Dimensions. Trying to scale %dx1 to obtain %dx1.\n",v->height,res->height);
		return;
	}
	for (int j=0; j < v->height; j++) {
		res->values[j] = s*v->values[j];
	} 
}

void copy_Vector(struct Vector* vec, struct Vector* res) {
	scale_Vector(1,vec,res);
}

struct Matrix* new_Matrix(int height, int width) {
	struct Matrix* m = malloc(sizeof(struct Matrix));
	m->height = height;
	m->width = width;
	m->values = malloc(sizeof(double*)*height);
	m->values[0] = calloc(height*width,sizeof(double));
	int i;
	for(i=1; i<height; i++) {
		m->values[i] = m->values[0]+i*width;
	}
	return m;
}

void delete_Matrix(struct Matrix* m) {
	free(m->values[0]);
	free(m->values);
	free(m);
}

struct Matrix* ones(int height, int width) {
	struct Matrix* m = new_Matrix(height, width);
	int i,j;
	for(i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			m->values[i][j] = 1.0;
		}
	}
	return m;
}

struct Matrix* eye(int size) {
	struct Matrix* m = new_Matrix(size, size);
	int i;
	for(i=0; i<size; i++) {
		m->values[i][i] = 1.0;
	}
	return m;
}

void print_Matrix(struct Matrix* m) {
	printf("[");
	int i,j;
	for(i=0; i<m->height; i++) {
		for (j=0; j<m->width; j++) {
			if (j != m->width-1 || i != m->height-1) printf("%f,\t",m->values[i][j]);
			else printf("%f",m->values[i][j]);
		}
		if (i<m->height-1) printf("\n ");
	}
	printf("]\n\n");
}

void transpose(struct Matrix* m, struct Matrix* res) {
	if ((m->height != res->height) || (m->width != res->width)) {
		printf("WARNING: Wrong Dimensions. Trying to transpose %dx%d to obtain %dx%d.\n",m->height,m->width,res->height,res->width);
		return;
	}
	int i,j;
	for(i=0; i<m->height; i++) {
		for(j=0; j<m->width; j++) {
			res->values[j][i] = m->values[i][j];
		}
	}
}

void multiply_Matrices(struct Matrix* m1, struct Matrix* m2, struct Matrix* res) {
	if ((m1->width != m2->height) || (m1->height != res->height) || (m2->width != res->width)) {
		printf("WARNING: Wrong Dimensions. Trying to multiply %dx%d with %dx%d to obtain %dx%d.\n",m1->height,m1->width,m2->height,m2->width,res->height,res->width);
		return;
	}
	int i,j,k;
	double val;
	for (i=0; i<m1->height; i++) {
		for (j=0; j<m2->width; j++) {
			val = 0.0;
			for (k=0; k<m1->width; k++) {
				val = val + m1->values[i][k]*m2->values[k][j];
			}
			res->values[i][j] = val;
		}
	}
}

void multiply_Matrix_Vector(struct Matrix* m, struct Vector* v, struct Vector* res) {
	if ((m->width != v->height) || (m->height != res->height)) {
		printf("WARNING: Wrong Dimensions. Trying to multiply %dx%d with %dx1 to obtain %dx1.\n",m->height,m->width,v->height,res->height);
		return;
	}
	int i,j;
	double val;
	for (i=0; i<m->height; i++) {
		val = 0.0;
		for (j=0; j<m->width; j++) {
			val = val + m->values[i][j]*v->values[j];
		}
		res->values[i] = val;
	}
}


double getMaximum(double* x, int size){
	double max=-1.0/0.0;
	int i;
	for(i=0;i<size;i++){
		if(x[i]>max){
			max=x[i];
		}
	}
	return max;
}


double getMinimum(double* x, int size){
	double min=1.0/0.0;
	int i;
	for(i=0;i<size;i++){
		if(x[i]<min){
			min=x[i];
		}
	}
	return min;
}


double normAxb_squared(struct Matrix* a, struct Vector* b, struct Vector* x, int size){
	int i=0;
	double scalarprod=0;
	struct Vector* help= new_Vector(size);
	multiply_Matrix_Vector(a,x, help);
	for(i=0;i<size;i++){
		help->values[i]=help->values[i]-b->values[i];	
		help->values[i]*=help->values[i];
		scalarprod+=help->values[i];
	}	
	delete_Vector(help);
	return scalarprod;

}

/*scalarproduct induced by matrix*/
double scalarproductMatrix(struct Matrix* a, struct Vector* x, struct Vector* y, int size){
	struct Vector* help=new_Vector(size);
	multiply_Matrix_Vector(a,x,help);
	return scalarproductRn(help,y,size);


}

double scalarproductRn(struct Vector* x, struct Vector* y, int size){
	int i;
	double scalarprod=0;
	double help=0;
	for(i=0;i<size;i++){
		help=x->values[i]*y->values[i];
		scalarprod+=help;
	}
	return scalarprod;
}

/*Gives you the gradient in x of function above, result is stored in gradient*/
void getGradient(struct Matrix* a, struct Vector* b, struct Vector* x, struct Vector* gradient, int size){
	struct Vector* help= new_Vector(size);
	struct Vector* help2= new_Vector(size);
	struct Matrix* transposed=new_Matrix(size,size);
	transpose(a,transposed);
	multiply_Matrix_Vector(a,x, help);
	for(int i=0;i<size;i++){
		help->values[i]=help->values[i]-b->values[i];	
	}
	multiply_Matrix_Vector(transposed,help,help2);
	scale_Vector(2,help2,gradient);
	
}

/*   Gradient Descent Method, assuming gamma is given?*/
void gradientDescent(struct Matrix* a, struct Vector* b, struct Vector* initialVal, struct Vector** x, struct Vector* y, double gamma, int size, int steps){

	x[0]=initialVal;
	int i=0;
	struct Vector* gradient=new_Vector(size);
		
	for(i=1;i<steps;i++){
		getGradient(a,b,x[i-1],gradient,size);
		scale_Vector(-gamma,gradient,gradient);
		
		add_Vectors(x[i-1],gradient,x[i]);
		y->values[i]=normAxb_squared(a,b,x[i],size);
	}


}

/*Conjugate gradient method from wikipedia. First version by Jasmin, changes made by Alex*/
struct Vector* conjugateGradient(struct Matrix* a, struct Vector* b, struct Vector* initialVal, struct Vector** x, int size, double precision, int steps){
	x[0]=initialVal;	
	
	//This could still be optimised
	struct Vector** r = malloc(sizeof(struct Vector*)*steps);	
	for (int j=0; j < steps; j++) {
		r[j] = new_Vector(size);
	}
	struct Vector* p = new_Vector(size);
	struct Vector* help=new_Vector(size);
	struct Vector* help1=new_Vector(size);
	double beta;
	double alpha;
	double sp1;
	double sp2;
	multiply_Matrix_Vector(a,x[0],help);
	scale_Vector(-1,help,help);
	//set r[0]
	add_Vectors(b,help,r[0]);
	p=r[0];
	int i=0;
	for(i=0;i<steps-1;i++){
		sp1 =scalarproductRn(r[i],r[i],size);
		sp2 =scalarproductMatrix(a,p,p,size);
		alpha = sp1/sp2;
		scale_Vector(alpha,p,help1);
		add_Vectors(x[i],help1,x[i+1]);
		multiply_Matrix_Vector(a,p,help);
		scale_Vector(-1*alpha,help,help);
		add_Vectors(r[i],help,r[i+1]);
		if(vectornorm(r[i+1])<precision){
			//Deleting some things, r is making problems
			delete_Vector(p);
			delete_Vector(help);
			delete_Vector(help1);
			free(r);
			return x[i+1];
		}
		sp2=scalarproductRn(r[i],r[i],size);
		beta = sp1/sp2;
		scale_Vector(beta,p,help);
		add_Vectors(help,r[i+1],p);		
	}
	delete_Vector(p);
	delete_Vector(help);
	delete_Vector(help1);
	free(r);
	return NULL;

}
