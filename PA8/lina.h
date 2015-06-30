#ifndef LINA_H
#define LINA_H

/* Vector struct. Stores an array of coordinates and its dimension */
struct Vector{
	double* values;
	int height;
};

/* Matrix struct. Stores a two-dimensional array of matrix values and its dimensions */
struct Matrix{
	double** values;
	int height;
	int width;
};

/* Contructor for a zero vector of given dimension */
struct Vector* new_Vector(int height);

/* Deconstructor for a vector */
void delete_Vector(struct Vector* v);


/* Declare some functions to manipulate and utilize vectors.
 * Operations like add and scale will not change their argument vectors. The result will stored in res. 
 */
void print_Vector(struct Vector* v);

double vectornorm(struct Vector* v);

void add_Vectors(struct Vector* vec1, struct Vector* vec2, struct Vector* res);

void scale_Vector(double s, struct Vector* v, struct Vector* res);

void copy_Vector(struct Vector* vec, struct Vector* res);

/* Constructor for a zero matrix of given dimensions */
struct Matrix* new_Matrix(int height, int width);

/*Generates Kd matrix for given matrix of given dim*/
void generateKd(int dim, struct Matrix* kd);

/* Deconstructor for a matrix */
void delete_Matrix(struct Matrix* m);


/*A function for easier and better matrix changes*/
void change_Matrix(struct Matrix* m, int i, int j, double value);

/* Declare some functions to manipulate and utilize matrices.
 *Operations like multiply and transpose will not change their argument matrices. The result is stored in res.
 */
struct Matrix* ones(int height, int width);

struct Matrix* eye(int size);

void print_Matrix(struct Matrix* m);

void transpose(struct Matrix* m, struct Matrix* res);

void multiply_Matrices(struct Matrix* m1, struct Matrix* m2, struct Matrix* res);

/* Declare a matrix-vector-multiplication. The result is stored in res */
void multiply_Matrix_Vector(struct Matrix* m, struct Vector* v, struct Vector* res);

/*
Calculates the maximum of an array. Needed for dynamic calculation of ymin and ymax
@author Alex
*/
double getMaximum(double* x, int size);

/*
Calculates the minimum of an array. Needed for dynamic calculation of ymin and ymax
@author Alex
*/
double getMinimum(double* x, int size); 

/*calculate the f(x) for f=||Ax-b||^2 for GDM*/
double normAxb_squared(struct Matrix* a, struct Vector* b, struct Vector* x, int size);

//Calculate the scalar product in R^n
double scalarproductRn(struct Vector* x, struct Vector* y, int size);

//Calculate the scalar product x^T*A*x
double scalarproductMatrix(struct Matrix* a, struct Vector* x, struct Vector* y, int size);

//Get the gradient
void getGradient(struct Matrix* a, struct Vector* b, struct Vector* x, struct Vector* gradient, int size);

//Get the gradient descent
void gradientDescent(struct Matrix* a, struct Vector* b, struct Vector* initialVal, struct Vector** x, double gamma, int size, int steps);

//Use conjugate gradient to solve the linear system Ax=b
int conjugateGradient(struct Matrix* a, struct Vector* b, struct Vector* initialVal, struct Vector** x, int size, double precision, int steps);


#endif /* LINA_H */
