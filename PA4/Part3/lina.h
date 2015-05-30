#ifndef LINA_H
#define LINA_H

struct Vector{
	double* values;
	int height;
};

struct Matrix{
	double** values;
	int height;
	int width;
};

struct Vector* new_Vector(int height);

void delete_Vector(struct Vector* v);

void print_Vector(struct Vector* v);

double vectornorm(struct Vector* v);

void add_Vectors(struct Vector* vec1, struct Vector* vec2, struct Vector* res);

void scale_Vector(double s, struct Vector* v, struct Vector* res);

void copy_Vector(struct Vector* vec, struct Vector* res);

struct Matrix* new_Matrix(int height, int width);

void delete_Matrix(struct Matrix* m);

struct Matrix* ones(int height, int width);

struct Matrix* eye(int size);

void print_Matrix(struct Matrix* m);

void transpose(struct Matrix* m, struct Matrix* res);

void multiply_Matrices(struct Matrix* m1, struct Matrix* m2, struct Matrix* res);

void multiply_Matrix_Vector(struct Matrix* m, struct Vector* v, struct Vector* res);

#endif /* LINA_H */
