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

/* Deconstructor for a matrix */
void delete_Matrix(struct Matrix* m);

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

#endif /* LINA_H */
