#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "lina.h"
#include "num_ode.h"


/* Declare some numerical methods for solving ODEs.
 * For details see num_ode.h
 */

/* Explicit Euler Scheme */
void euler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps) {	
	t[0] = t0;
	y[0] = y0;
	struct Vector* tmp1;
	struct Vector* tmp2;
	for (int j=1; j<steps; j++) {
		t[j] = t0+j*h;
		tmp1 = new_Vector(DIM);
		tmp2 = new_Vector(DIM);
		f(t[j-1],y[j-1],tmp1);
		scale_Vector(h,tmp1,tmp2);
		add_Vectors(y[j-1],tmp2,y[j]);
		delete_Vector(tmp1);
		delete_Vector(tmp2);
	}
}

/* Symplectic Euler Scheme. Only to be used for Hamiltonian systems. Requires even dimension DIM */
void symplectic_euler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps) {
	if (DIM%2 != 0) printf("Warning: Tried to use symplectic Euler method for vector of odd dimension");
	else {	
		t[0] = t0;
		y[0] = y0;
		struct Vector* tmp;
		for (int j=1; j<steps; j++) {
			t[j] = t0+j*h;
			tmp = new_Vector(DIM);
			f(t[j-1],y[j-1],tmp);
			//print_Vector(tmp);
			copy_Vector(y[j-1],y[j]);
			for(int i=DIM/2; i<DIM; i++) {
				y[j]->values[i] = y[j-1]->values[i] + h*tmp->values[i];
			}
			f(t[j-1],y[j],tmp);
			for(int i=0; i<DIM/2; i++) {
				y[j]->values[i] = y[j-1]->values[i] + h*tmp->values[i];
			}
			delete_Vector(tmp);
		}
	}
}

/* Modified Euler Scheme */
void modEuler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps) {
	t[0] = t0;
	y[0] = y0;
	struct Vector* tmp1;
	struct Vector* tmp2;
	for (int j=1; j<steps; j++) {
		t[j] = t0+j*h;
		tmp1 = new_Vector(DIM);
		tmp2 = new_Vector(DIM);
		f(t[j-1],y[j-1],tmp1);
		scale_Vector(h/2.0,tmp1,tmp2);
		add_Vectors(y[j-1],tmp2,tmp1);
		f(t[j-1]+h/2.0,tmp1,tmp2);
		scale_Vector(h,tmp2,tmp1);
		add_Vectors(y[j-1],tmp1,y[j]);
		delete_Vector(tmp1);
		delete_Vector(tmp2);
	}
}

/* Method of Heun */
void heun (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps) {
	t[0] = t0;
	y[0] = y0;
	struct Vector* tmp1;
	struct Vector* tmp2;
	struct Vector* tmp3;
	for (int j=1; j<steps; j++) {
		t[j] = t0+j*h;
		tmp1 = new_Vector(DIM);
		tmp2 = new_Vector(DIM);
		tmp3 = new_Vector(DIM);
		f(t[j-1],y[j-1],tmp1);
		scale_Vector(h,tmp1,tmp2);
		add_Vectors(y[j-1],tmp2,tmp1);
		f(t[j],tmp1,tmp2);
		f(t[j-1],y[j-1],tmp1);
		add_Vectors(tmp1,tmp2,tmp3);
		scale_Vector(h/2.0,tmp3,tmp1);
		add_Vectors(y[j-1],tmp1,y[j]);
		delete_Vector(tmp1);
		delete_Vector(tmp2);
		delete_Vector(tmp3);
	}
}

/* Adaptive Runge-Kutta Scheme of order 3, uses tol for stepsize-control */
void adaptive_rk3 (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double tol, double* t, struct Vector** y, int steps) {
	t[0] = t0;
	y[0] = y0;
	double h;
	double norm;
	struct Vector* k1;
	struct Vector* k2;
	struct Vector* k3;
	struct Vector* tmp1;
	struct Vector* tmp2;
	struct Vector* tmp3;
	for (int j=1; j<steps; j++) {
		bool adapt = true;
		h = 1;
		k1 = new_Vector(DIM);
		k2 = new_Vector(DIM);
		k3 = new_Vector(DIM);
		tmp1 = new_Vector(DIM);
		tmp2 = new_Vector(DIM);
		tmp3 = new_Vector(DIM);
		while(adapt) {
			f(t[j-1],y[j-1],k1);
			scale_Vector(h,k1,tmp1);
			add_Vectors(y[j-1],tmp1,tmp2);
			f(t[j-1]+h,tmp2,k2);
			add_Vectors(k1,k2,tmp1);
			scale_Vector(h/4,tmp1,tmp2);
			add_Vectors(y[j-1],tmp2,tmp1);
			f(t[j-1]+h/2,tmp1,k3);
			scale_Vector(2,k3,tmp1);
			scale_Vector(-1,k2,tmp2);
			add_Vectors(tmp1,tmp2,tmp3);
			scale_Vector(-1,k1,tmp1);
			add_Vectors(tmp1,tmp3,tmp2);
			norm = vectornorm(tmp2);
			if (h/3*norm < tol) {
				adapt = false;
				add_Vectors(k1,k2,tmp1);
				scale_Vector(4,k3,tmp2);
				add_Vectors(tmp1,tmp2,tmp3);
				scale_Vector(h/6,tmp3,tmp1);
				add_Vectors(y[j-1],tmp1,y[j]);
			} else {
				if (3/2*tol/norm>0) {
					h = 3/2*tol/norm;
				} else {
					h = h/2;
				}
			}
		}
		t[j] = t[j-1]+h;

		delete_Vector(tmp1);
		delete_Vector(tmp2);
		delete_Vector(tmp3);
		delete_Vector(k1);
		delete_Vector(k2);
		delete_Vector(k3);
	}
}

/*calculate the f(x) for f=||Ax-b||^2 for GDM*/
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

//normal scalarproduct in R^n
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

/*scalarproduct induced by matrix*/
double scalarproductMatrix(struct Matrix* a, struct Vector* x, struct Vector* y, int size){
	struct Vector* help=new_Vector(size);
	multiply_Matrix_Vector(a,x,help);
	return scalarproductRn(help,y,size);


}


/*is supposed to do the conjugate gradient method. Not finished and dimensions might be wrong. */
void conjugateGradient(struct Matrix* a, struct Vector* b, struct Vector* initialVal, struct Vector** x, int size, double precision, int steps){
	x[0]=initialVal;
	
	

	struct Vector** r = malloc(sizeof(struct Vector*)*steps);	
	for (int j=0; j < steps; j++) {
		r[j] = new_Vector(size);
	}
	struct Vector** p = malloc(sizeof(struct Vector*)*steps);	
	for (int j=0; j < steps; j++) {
		p[j] = new_Vector(size);
	}
	
	struct Vector* help=new_Vector(size);
	struct Vector* help1=new_Vector(size);
	struct Vector* alpha=new_Vector(steps);
	struct Vector* beta=new_Vector(steps);
	multiply_Matrix_Vector(a,x[0],help);
	scale_Vector(-1,help,help);
	//set r[0]
	add_Vectors(b,help,r[0]);
	p[0]=r[0];
	int i=0;
	for(i=0;i<steps-1;i++){
		double sp1=scalarproductRn(r[i],r[i],size);
		double sp2=scalarproductMatrix(a,p[i],p[i],size);
		alpha->values[i]=sp1/sp2;
		scale_Vector(alpha->values[i],p[i],help1);
		add_Vectors(x[i],help1,x[i+1]);
		multiply_Matrix_Vector(a,p[i],help);
		scale_Vector(-1,help,help);
		add_Vectors(r[i],help,r[i+1]);
		if(vectornorm(r[i+1])<precision){
			break;
		}
		sp2=scalarproductRn(r[i],r[i],size);
		beta->values[i]=sp1/sp2;
		scale_Vector(beta->values[i],p[i],help);
		add_Vectors(help,r[i+1],p[i+1]);
	
		
	}

}



/* Runge-Kutta Scheme of order 4 */
void rk4 (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps) {
	t[0] = t0;
	y[0] = y0;
	struct Vector* k1;
	struct Vector* k2;
	struct Vector* k3;
	struct Vector* k4;
	struct Vector* tmp1;
	struct Vector* tmp2;
	for (int j=1; j<steps; j++) {
		k1 = new_Vector(DIM);
		k2 = new_Vector(DIM);
		k3 = new_Vector(DIM);
		k4 = new_Vector(DIM);
		tmp1 = new_Vector(DIM);
		tmp2 = new_Vector(DIM);

		f(t[j-1],y[j-1],k1);
		scale_Vector(h/2,k1,tmp1);
		add_Vectors(y[j-1],tmp1,tmp2);
		f(t[j-1]+h/2,tmp2,k2);
		scale_Vector(h/2,k2,tmp1);
		add_Vectors(y[j-1],tmp1,tmp2);
		f(t[j-1]+h/2,tmp2,k3);
		scale_Vector(h,k3,tmp1);
		add_Vectors(y[j-1],tmp1,tmp2);
		f(t[j-1]+h,tmp2,k4);
		
		add_Vectors(k2,k3,tmp1);
		scale_Vector(2,tmp1,tmp2);
		add_Vectors(k1,tmp2,tmp1);
		add_Vectors(tmp1,k4,tmp2);
		scale_Vector(h/6,tmp2,tmp1);
		add_Vectors(y[j-1],tmp1,y[j]);
		t[j] = t[j-1]+h;

		delete_Vector(tmp1);
		delete_Vector(tmp2);
		delete_Vector(k1);
		delete_Vector(k2);
		delete_Vector(k3);
	}
}
