//
// Created by lococo on 07/06/19.
//

#ifndef MATRICES_RESOLVER_MATRIXLIBRARY_H
#define MATRICES_RESOLVER_MATRIXLIBRARY_H

#include <stdbool.h>

int ** new_matrix(int n, int m);
void freeMatrix(int ** matrix, int n, int m);
void add_matrix (int ** matrix1, int n1, int m1, int ** matrix2, int ** answer);
void substract_matrix (int ** matrix1, int n1, int m1, int ** matrix2, int ** answer);
void productoEscalar(int ** matrix, int n, int m, int num, int ** answer);

void copyMatrix (int ** matrixToCopy, int n, int m, int ** dest);
void multiply_matrix(int ** matrix1, int n1, int m1, int ** matrix2, int n2, int m2, int** answer);
void transpose_matrix ( int ** matrix, int n, int m, int ** answer);


void multiplyRow(int ** matrix, int n1, int m1, int rowIndex, int num);
void divideRow(int ** matrix, int n, int m, int rowIndex, int num);

void matrixSolver (int ** matrix1, int n1, int m1, int * answer);
void solve_matrix (int ** matrix, int n, int m);
void getResult(int ** matrix, int n1, int m1, int * answer);


void swapRows(int ** matrix, int n1, int m1, int rowIndex1, int rowIndex2);
void swapInNonZeroRow(int ** matrix, int n1,int m1, int rowIndex);
void subtractRow(int ** matrix, int n1, int m1, int rowIndexChanged, int rowIndex);

void printMatrixWithResults(int ** matrix, int n, int m);
void printMatrix (int ** matrix, int n, int m);

void getInvertibleMatrix (int ** matrix, int n, int m, int ** answer);
bool areEqualMatrix(int ** matrix1, int n, int m, int ** matrix2);

bool areResultOfMatrix(int ** matrix, int n, int m, int * answer);


int * newVector (int n);
void printVector(int * vector, int n);

#endif //MATRICES_RESOLVER_MATRIXLIBRARY_H
