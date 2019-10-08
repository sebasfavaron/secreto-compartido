//
// Created by joaquin on 07/06/19.
//

#ifndef CRIPTO_SECRETO_COMPARTIDO_SECRET_WORKFLOW_H
#define CRIPTO_SECRETO_COMPARTIDO_SECRET_WORKFLOW_H

#include "matrix_wrapper.h"

enum errors{PARAMETERS = 1};

struct share_computation_vector_struct;
typedef struct share_computation_vector_struct* share_computation_vector;

struct random_vector_struct;
typedef struct random_vector_struct* random_vector;

struct final_share_vector_struct;
typedef struct final_share_vector_struct* final_share_vector;

int linearly_independent(int row, int col, uint8_t ** matrix);

void modularizar(uint8_t * matrix, int n);

matrix * get_S_matrices(int m, uint8_t * image, int n, int * size);

int share_construction(int k, int n, uint8_t ** secret_matrix, int m, matrix Rw_matrix, matrix * Shj, uint8_t ** watermark);

void share_reconstruction(int k, int n, matrix * Shj, matrix Rw_matrix, uint8_t ** S_matrix, uint8_t ** W_matrix);

matrix build_B_matrix(int n, int k, matrix * Shj);

matrix * build_Gj_matrices(int n, int k, matrix * Shj);

matrix build_R_matrix(int n, int k, matrix * Gj);

void build_W_matrix(int m, matrix s_matrix, matrix Rw_matrix, uint8_t ** W_matrix);

void build_S_matrix(int m, matrix s_matrix, matrix R_matrix, uint8_t ** S_matrix);

matrix get_watermark(int m, uint8_t ** watermark);

matrix build_s_matrix(int m, int k, matrix matrix1);

matrix build_Xj(int k, int n);

uint8_t * build_cj(int k);

matrix build_Vj(matrix a_matrix,int m,int k1, matrix xj, int k2, int n);

matrix * build_Gj(int k, int n, int m, matrix matrix_R, uint8_t * cj);

void build_Shj(int k, int n, int m, matrix Vj, matrix * Gj, matrix * Shj);

uint8_t gen_random_number(uint8_t a, uint8_t b);

void modulus_remainder(matrix matrix_1, matrix matrix_2, int i, int j, matrix answer);

matrix gen_random_matrix(int m, int k);

int validate_parameters(int k, int n, uint8_t ** matrix);

int squared(uint8_t ** matrix1);

#endif //CRIPTO_SECRETO_COMPARTIDO_SECRET_WORKFLOW_H
