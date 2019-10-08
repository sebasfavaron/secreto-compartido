//
// Created by joaquin on 10/06/19.
//

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix_wrapper.h"
#include "matrixLibrary.h"


matrix build_matrix(int n, int k, enum types type, uint8_t ** matrix_values){
    matrix matrix_build = malloc(sizeof(matrix_struct));
    if (matrix_build == NULL){
        printf("No se pudo inicializar la matriz \n");
        exit(0);
    }

    if(type > B_MATRIX){
        printf("Tipo de matriz incorrecto \n");
        exit(0);
    }

    matrix_build->type = type;
    matrix_build->matrix = matrix_values;
    matrix_build->height = n;
    matrix_build->width = k;

    return matrix_build;
}

void delete_matrix(matrix matrix1){
    freeMatrix(matrix1->matrix, matrix1->height, matrix1->width);
    free(matrix1);
}

int get_matrix_height(matrix matrix1) {
    return matrix1->height;
}
int get_matrix_width(matrix matrix1) {
    return matrix1->width;
}

void operation_wrap(int type, matrix matrix1, matrix matrix2, matrix answer){
    switch(type){
        case ADD:
            add_matrix(matrix1->matrix, get_matrix_height(matrix1), get_matrix_width(matrix2), matrix2->matrix, answer->matrix);
            break;
        case SUBS:
            substract_matrix(matrix1->matrix, get_matrix_height(matrix1), get_matrix_width(matrix2), matrix2->matrix, answer->matrix);
            break;
        case MULT:
            multiply_matrix(matrix1->matrix, get_matrix_height(matrix1), get_matrix_width(matrix1), matrix2->matrix, get_matrix_height(matrix2), get_matrix_width(matrix2), answer->matrix);
            break;
        case TRANS:
            transpose_matrix(matrix1->matrix, get_matrix_height(matrix1), get_matrix_width(matrix1), answer->matrix);
            break;

    }
}

void solver_wrap(matrix matrix1, int* answer) {
            matrixSolver(matrix1->matrix, get_matrix_height(matrix1), get_matrix_width(matrix1), answer);
}

void power_wrap(matrix matrix1, int power, matrix answer){
    power_matrix(matrix1->matrix, get_matrix_height(matrix1), get_matrix_width(matrix1), power, answer->matrix);
}

