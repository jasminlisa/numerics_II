#ifndef NUM_ODE_H
#define NUM_ODE_H

#include "lina.h"

#ifndef DIM
#define DIM 4
#endif


//Define Euler, ModEuler and Heun Single Step Schemes
void euler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);

void symplectic_euler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);


void modEuler (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);

void heun (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double h, double* t, struct Vector** y, int steps);

void adaptive_rk3 (void (*f)(double,struct Vector*,struct Vector*), double t0, struct Vector* y0, double tol, double* t, struct Vector** y, int steps);

#endif /* NUM_ODE_H */
