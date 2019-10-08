//
// Created by joaquin on 10/06/19.
//

#ifndef CRIPTO_SECRETO_COMPARTIDO_MATRIX_WRAPPER_H
#define CRIPTO_SECRETO_COMPARTIDO_MATRIX_WRAPPER_H

#include <stdint.h>

struct matrix_struct;
typedef struct matrix_struct* matrix;

enum types{S_MATRIX = 1, A_MATRIX, s_MATRIX, R_MATRIX, X_VEC, V_VEC, G_MATRIX, W_MATRIX, RW_MATRIX, SH_MATRIX, B_MATRIX};

typedef struct matrix_struct {
    enum types type;
    uint8_t ** matrix;
    int height;
    int width;
} matrix_struct;

enum operations{ADD,SUBS,MULT,SOLVE,POW,TRANS};

matrix build_matrix(int n, int k, enum types type, uint8_t ** matrix_values);
void delete_matrix(matrix matrix1);
void operation_wrap(int type, matrix matrix1, matrix matrix2, matrix answer);
void transformation_wrap(int type, matrix matrix1, matrix answer);

int get_matrix_height(matrix matrix1);
int get_matrix_width(matrix matrix1);

#endif //CRIPTO_SECRETO_COMPARTIDO_MATRIX_WRAPPER_H
