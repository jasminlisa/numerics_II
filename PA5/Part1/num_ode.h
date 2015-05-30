#ifndef NUM_ODE_H
#define NUM_ODE_H

#include "lina.h"

#ifndef DIM
#define DIM 4  //<--- the dimension macro needs to be adjusted depending on the problem
#endif


/* Declare some numerical methods for solving ODEs
 * INPUTS:	@param f	- 	handle on the funtion defining the ODE, i.e. y'(t) = f(t,y(t))
 *		@param t0	- 	initial time
 *		@param y0	- 	initial value (Vector of dimension DIM)
 *		@param h/tol		step size/tolerance (depending on the method)
 *		@param t	- 	array to store time steps
 *		@param y	- 	array to store approximated solutions at times t
 *		@param steps	- 	number of steps (length of the arrays t and y)
 *
 * OUTPUT: 	@return		- 	none (implicitly given via t and y)
 */

void euler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);

void symplectic_euler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);

void modEuler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);

void heun (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);

void adaptive_rk3 (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double tol, double* t, struct Vector** y, int steps);

void rk4 (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double tol, double* t, struct Vector** y, int steps);

#endif /* NUM_ODE_H */
