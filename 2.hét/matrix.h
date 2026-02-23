#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

/** 
 * Initializes the matrix to an identity matrix
 */
void init_identity_matrix(float matrix[3][3]);

/**
 * Multiplies the matrix with a scalar
 */
void multiply_matrix_with_scalar(float matrix[3][3], float scalar);

/**
 * Multiplies 2 matrices
 */
void multiply_matrices(float matrix1[3][3], float matrix2[3][3], float finalmatrix[3][3]);
#endif // MATRIX_H