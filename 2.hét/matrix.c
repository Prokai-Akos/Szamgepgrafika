#include "matrix.h"

#include <stdio.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void init_identity_matrix(float matrix[3][3]){
    int i;
    int j;
    
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            if(j=0,j=0) matrix[i][j]=1;
            else if(i=1,j=1) matrix[i][j]=1;
            else if(i=2,j=2) matrix[i][j]=1;
            else matrix[i][j]=0;   
        }
    }
}

void multiply_matrix_with_scalar(float matrix[3][3], float scalar){
    int i;
    int j;

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++)
        { matrix[i][j] = matrix[i][j] * scalar; }  
    }
}

void multiply_matrices(float matrix1[3][3], float matrix2[3][3], float finalmatrix[3][3]){
    int i;
    int j;
    int k;

    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            for (int k = 0; i < 3; i++) {finalmatrix[i][j] += matrix1[i][k] + matrix2[k][i]; }
        }
    }
}

void transform_point(float matrix[3][3]){
    
}