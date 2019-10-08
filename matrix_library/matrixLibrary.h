#ifndef MATRICES_RESOLVER_MATRIXLIBRARY_H
#define MATRICES_RESOLVER_MATRIXLIBRARY_H

#include <stdbool.h>
#include <stdint.h>

uint8_t ** new_matrix(int n, int m);
void freeMatrix(uint8_t ** matrix, int n, int m);
void add_matrix (uint8_t ** matrix1, int n1, int m1, uint8_t ** matrix2, uint8_t ** answer);
void substract_matrix (uint8_t ** matrix1, int n1, int m1, uint8_t ** matrix2, uint8_t ** answer);
void productoEscalar(uint8_t ** matrix, int n, int m, uint8_t num, uint8_t ** answer);

void copyMatrix (uint8_t ** matrixToCopy, int n, int m, uint8_t ** dest);
void multiply_matrix(uint8_t ** matrix1, int n1, int m1, uint8_t ** matrix2, int n2, int m2, uint8_t** answer);
void transpose_matrix ( uint8_t ** matrix, int n, int m, uint8_t ** answer);
void power_matrix (uint8_t ** matrix, int n, int m, int power, uint8_t ** answer);


void multiplyRow(uint8_t ** matrix, int n1, int m1, int rowIndex, uint8_t num);
void divideRow(uint8_t ** matrix, int n, int m, int rowIndex, uint8_t num);

void matrixSolver (uint8_t ** matrix1, int n1, int m1, uint8_t * answer);
void solve_matrix (uint8_t ** matrix, int n, int m);
void getResult(uint8_t ** matrix, int n1, int m1, uint8_t * answer);


void swapRows(uint8_t ** matrix, int n1, int m1, int rowIndex1, int rowIndex2);
void swapInNonZeroRow(uint8_t ** matrix, int n1,int m1, int rowIndex);
void subtractRow(uint8_t ** matrix, int n1, int m1, int rowIndexChanged, int rowIndex);

void printMatrixWithResults(uint8_t ** matrix, int n, int m);
void printMatrix (uint8_t ** matrix, int n, int m);

void getInvertibleMatrix (uint8_t ** matrix, int n, int m, uint8_t ** answer);
bool areEqualMatrix(uint8_t ** matrix1, int n, int m, uint8_t ** matrix2);

bool areResultOfMatrix(uint8_t ** matrix, int n, int m, uint8_t * answer);


uint8_t * newVector (int n);
void printVector(uint8_t * vector, int n);


#endif //MATRICES_RESOLVER_MATRIXLIBRARY_H
