//
// Created by joaquin on 07/06/19.
//
#include <stdio.h>
#include <stdbool.h>
#include "secret_workflow.h"
#include <matrixLibrary.h>
#include "libraryMod251.h"
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

matrix gen_test_matrix_A(int m, int k){
    uint8_t ** aux_matrix = new_matrix(m, k);
    aux_matrix[0][0] = 3;
    aux_matrix[0][1] = 7;

    aux_matrix[1][0] = 6;
    aux_matrix[1][1] = 1;

    aux_matrix[2][0] = 2;
    aux_matrix[2][1] = 5;

    aux_matrix[3][0] = 6;
    aux_matrix[3][1] = 6;

    matrix A_matrix = build_matrix(m, k, A_MATRIX, aux_matrix);
    return A_matrix;
}

matrix gen_test_matrix_Xj(int k, int n){
    uint8_t ** aux_matrix = new_matrix(k, n);
    aux_matrix[0][0] = 9;
    aux_matrix[1][0] = 5;

    aux_matrix[0][1] = 4;
    aux_matrix[1][1] = 4;

    aux_matrix[0][2] = 9;
    aux_matrix[1][2] = 8;

    aux_matrix[0][3] = 3;
    aux_matrix[1][3] = 2;

    matrix Xj = build_matrix(k, n, X_VEC, aux_matrix);
    return Xj;
}

int linearly_independent(int row, int col, uint8_t ** matrix){
    int new_row = row, new_col = col;
    if(row < col){
        new_row = col;
        new_col = row;
    }

    uint8_t ** aux = new_matrix(new_row, new_col+1);
    if(row < col) {
        transpose_matrix(matrix, new_row, new_col, aux);
    }
    else{
        copyMatrix(matrix, new_row, new_col, aux);
    }
    //printMatrix(aux, new_row, new_col);
    for(int i=0; i<row; i++){
        aux[i][col] = 0;
    }
    uint8_t * result = newVector(new_col);
    matrixSolver(aux, new_row, new_col+1, result);
    //printMatrix(aux, new_row, new_col);
    int resp = 0;
    for(int i=0; i<new_row; i++){
        for(int j=0; j<new_col; j++){
            if(aux[i][j] != 0){
                resp++;
                if(resp == new_col){
                    return 1;
                }

                break;
            }
        }
    }
    return 0;
}

void modularizar(uint8_t * matrix, int n){
    for (int i = 0; i < n; i++) {
        matrix[i] = (matrix[i] % 251 + 251) % 251;
    }
}

matrix * get_S_matrices(int m, uint8_t * image, int n, int * size){
    (*size) = m / (n * n);
    matrix * s_matrices = malloc((*size) * sizeof(matrix));

    for(int i=0; i<(*size); i++){
        uint8_t ** aux_matrix = new_matrix(n ,n);
        matrix s = build_matrix(n, n ,S_MATRIX, aux_matrix);

        for(uint8_t j=0; j<n; j++){
            for(uint8_t l=0; l<n; l++){
                uint8_t selector = i * (n * n) + j + l;
                aux_matrix[j][l] = image[selector];
            }
        }
        s_matrices[i] = s;
        //printMatrix(s->matrix, s->height, s->width);
    }
    return s_matrices;
}

int share_construction(int k, int n, uint8_t ** secret_matrix, int m, matrix Rw_matrix, matrix * Shj, uint8_t ** watermark){

    /*if(!validate_parameters(k, n, secret_matrix)){
        //Error de parametros
        return PARAMETERS;
    }*/

    //Construyo matriz S
    matrix S_matrix = build_matrix(m, m, S_MATRIX, secret_matrix);
//    printf("Matriz S:\n");
//    printMatrix(S_matrix->matrix, S_matrix->height, S_matrix->width);

    //Construyo la matriz A random
    matrix A_matrix = gen_random_matrix(m, k);
    //matrix A_matrix = gen_test_matrix_A(m, k);
//    printf("Matriz A:\n");
//    printMatrix(A_matrix->matrix, A_matrix->height, A_matrix->width);

    //Calculo matriz s
    matrix s_matrix = build_s_matrix(m, k, A_matrix);
//    printf("Matriz s:\n");
//    printMatrix(s_matrix->matrix, s_matrix->height, s_matrix->width);

    //Calculo matriz R
    uint8_t ** R_aux = new_matrix(m, m);
    matrix R_matrix = build_matrix(m, m, R_MATRIX, R_aux);
    modulus_remainder(S_matrix, s_matrix, m, m, R_matrix);
//    printf("Matriz R:\n");
//    printMatrix(R_matrix->matrix, R_matrix->height, R_matrix->width);

    matrix Xj = build_Xj(k, n);
    //matrix Xj = gen_test_matrix_Xj(k, n);
//    printf("Matriz Xj:\n");
//    printMatrix(Xj->matrix, Xj->height, Xj->width);

    uint8_t * cj = build_cj(n);
//    printf("Vector Cj:\n");
//    printVector(cj, n);

    matrix Vj = build_Vj(A_matrix, m, k, Xj, k, n);
//    printf("Matriz Vj:\n");
//    printMatrix(Vj->matrix, Vj->height, Vj->width);

    matrix * Gj = build_Gj(k, n, m, R_matrix, cj);
//    for(int i=0; i<n; i++){
//        printf("Matriz gj:\n");
//        printMatrix(Gj[i]->matrix, Gj[i]->height, Gj[i]->width);
//    }

    //Buscar las watermark
    matrix W_matrix = get_watermark(m, watermark);
//    printf("Matriz W:\n");
//    printMatrix(W_matrix->matrix, W_matrix->height, W_matrix->width);

    modulus_remainder(W_matrix, s_matrix, m, m, Rw_matrix);
//    printf("Matriz Rw:\n");
//    printMatrix(Rw_matrix->matrix, Rw_matrix->height, Rw_matrix->width);

    //Construir las shares

    build_Shj(k ,n , m, Vj, Gj, Shj);
//    for(int i=0; i<n; i++){
//        printf("Matriz Shj:\n");
//        printMatrix(Shj[i]->matrix, Shj[i]->height, Shj[i]->width);
//    }
}

void share_reconstruction(int k, int m, matrix * Shj, matrix Rw_matrix, uint8_t ** S_matrix, uint8_t ** W_matrix){
    matrix B_matrix = build_B_matrix(m, k, Shj);
//    printf("Matriz B:\n\n");
//    printMatrix(B_matrix->matrix, B_matrix->height, B_matrix->width);

    matrix s_matrix = build_s_matrix(m, k, B_matrix);
//    printf("Matriz s:\n\n");
//    printMatrix(s_matrix->matrix, s_matrix->height, s_matrix->width);

    matrix * Gj = build_Gj_matrices(m, k, Shj);
//    for(int i=0; i<k; i++) {
//        printf("Matriz G%d:\n\n", i+1);
//        printMatrix(Gj[i]->matrix, Gj[i]->height, Gj[i]->width);
//    }

    matrix R_matrix = build_R_matrix(m, k, Gj);
//    printf("Matriz R:\n\n");
//    printMatrix(R_matrix->matrix, R_matrix->height, R_matrix->width);

    build_S_matrix(m, s_matrix, R_matrix, S_matrix);
//    printf("Matriz S:\n\n");
//    printMatrix(S_matrix, m, m);

    build_W_matrix(m, s_matrix, Rw_matrix, W_matrix);
//    printf("Matriz S:\n\n");
//    printMatrix(W_matrix, m, m);
}

void build_W_matrix(int m, matrix s_matrix, matrix Rw_matrix, uint8_t ** W_matrix){
    add_matrix(s_matrix->matrix, m, m, Rw_matrix->matrix, W_matrix);
}

void build_S_matrix(int m, matrix s_matrix, matrix R_matrix, uint8_t ** S_matrix){
    add_matrix(s_matrix->matrix, m, m, R_matrix->matrix, S_matrix);
}

matrix build_R_matrix(int m, int k, matrix * Gj){
    uint8_t ** aux_matrix = new_matrix(m, m);
    matrix R_matrix = build_matrix(m, m, R_MATRIX, aux_matrix);
    int nk = ceil(m/k);

    for(int x=0; x<m; x++){
        for(int y=0; y<nk; y++) {
            uint8_t ** matrix_to_solve = new_matrix(k, k + 1);
            for (int i = 0; i < k; i++) {
                for (int j = 0; j < k - 1; j++) {
                    matrix_to_solve[i][j] = 1;
                }
                matrix_to_solve[i][k - 1] = i + 1;
                matrix_to_solve[i][k] = Gj[i]->matrix[x][y];
            }
            uint8_t * result = newVector(k);
            matrixSolver(matrix_to_solve, k, k+1, result);
            for(int l=0; l<k; l++){
                aux_matrix[x][2*y+l] = result[l];
            }
        }
    }

    return R_matrix;
}

matrix * build_Gj_matrices(int m, int k, matrix * Shj){
    matrix * Gj = malloc(k * sizeof(matrix));

    for(int i=1; i<=k; i++){
        uint8_t ** aux_matrix = new_matrix(m, k);
        matrix gj = build_matrix(m, k, G_MATRIX, aux_matrix);

        for(int j=1; j<=m; j++){
            for(int l=1; l<=k; l++){
                aux_matrix[j-1][l-1] = Shj[i-1]->matrix[j-1][l];
            }
        }
        Gj[i-1] = gj;
    }

    return Gj;
}

matrix build_B_matrix(int m, int k, matrix * Shj){
    uint8_t ** aux_matrix = new_matrix(m, k);
    matrix B_matrix = build_matrix(m, k, B_MATRIX, aux_matrix);

    for(int i=1; i<=m; i++){
        for(int j=1; j<=k; j++){
            aux_matrix[i-1][j-1] = Shj[j-1]->matrix[i-1][0];
        }
    }

    return B_matrix;
}

matrix get_watermark(int m, uint8_t ** watermark) {
    //Buscar la matriz de alguna de las imagenes para watermark

    matrix W_matrix = build_matrix(m, m, W_MATRIX, watermark);

    return W_matrix;
}

matrix build_s_matrix(int m, int k, matrix matrix1){
    uint8_t ** aux_matrix = new_matrix(m, m);
    matrix s_matrix = build_matrix(m, m, s_MATRIX, aux_matrix);

    uint8_t ** matrix1_T = new_matrix(k, m);
    uint8_t ** mult1_matrix = new_matrix(k, k);
    uint8_t ** pow_matrix = new_matrix(k, k);
    uint8_t ** mult2_matrix = new_matrix(m, k);
    uint8_t ** mult3_matrix = new_matrix(m, m);

    transpose_matrix(matrix1->matrix, k, m, matrix1_T);
    multiply_matrix(matrix1_T, k, m, matrix1->matrix, m, k, mult1_matrix);
    getInvertibleMatrix(mult1_matrix, k, k, pow_matrix);
    multiply_matrix(matrix1->matrix, m, k, pow_matrix, k, k, mult2_matrix);
    multiply_matrix(mult2_matrix, m, k, matrix1_T, k, m, mult3_matrix);

    copyMatrix(mult3_matrix, m, m, aux_matrix);

    freeMatrix(matrix1_T, k, m);
    freeMatrix(mult1_matrix, k, k);
    freeMatrix(pow_matrix, k, k);
    freeMatrix(mult2_matrix, m, k);
    freeMatrix(mult3_matrix, m, m);

    return s_matrix;
}

matrix build_Xj(int k, int n){

    //Armo un nuevo vector de n vectores random kx1
    uint8_t ** aux_matrix = new_matrix(k, n);
    matrix xj = build_matrix(k, n, X_VEC, aux_matrix);
    do {
        for (int i = 0; i < get_matrix_height(xj); i++) {
            for (int j = 0; j < get_matrix_width(xj); j++) {
                aux_matrix[i][j] = gen_random_number(0, 251);
            }
        }
    }
    while (linearly_independent(k, n, aux_matrix) == 0);
    return xj;
}

uint8_t * build_cj(int n){

    //Armo un nuevo vector paralelo de constantes de 1 a n
    uint8_t * cj = newVector(n);
    for(int i=1; i<=n; i++){
        cj[i-1] = i;
    }
    return cj;
}

matrix build_Vj(matrix a_matrix, int m, int k1, matrix xj, int k2, int n){

    //Armo el share vector Vj
    uint8_t ** aux_matrix = new_matrix(m, n);
    matrix vj = build_matrix(m, n, V_VEC, aux_matrix);

    multiply_matrix(a_matrix->matrix, m, k1, xj->matrix, k2, n, vj->matrix);

    return vj;
}

matrix * build_Gj(int k, int n, int m, matrix r_matrix, uint8_t * cj){
    matrix * Gj = malloc(n * sizeof(matrix));
    int mk = ceil(m / k);

    for(int j=1; j<=n; j++){
        uint8_t ** aux_matrix = new_matrix(m, mk);

        matrix gj = build_matrix(m, mk, G_MATRIX, aux_matrix);
        for(int i=1; i<=m; i++){
            for(int t=1; t<=mk; t++){
                for(int k2=1; k2<=k; k2++){
                    int column_selector = k * (t - 1) + k2;
                    if(k2 == k){
                        uint8_t a = r_matrix->matrix[i-1][column_selector-1] * pow(cj[j-1], k-1);
                        aux_matrix[i-1][t-1] = suma_mod251(aux_matrix[i-1][t-1], a);
                    }
                    else{
                        uint8_t a = r_matrix->matrix[i-1][column_selector-1];
                        aux_matrix[i-1][t-1] = suma_mod251(aux_matrix[i-1][t-1], a);
                    }
                }
            }
        }
        Gj[j-1] = gj;
    }

    return Gj;
}

void build_Shj(int k, int n, int m, matrix Vj, matrix * Gj, matrix * Shj){
    int mk = ceil(m/k);

    for(int j=1; j<=n; j++){
        uint8_t ** aux_matrix = new_matrix(m, mk +1);
        matrix shj = build_matrix(m, mk+1, SH_MATRIX, aux_matrix);
        for(int i=1; i<=m; i++){
            aux_matrix[i-1][0] = Vj->matrix[i-1][j-1];
            for(int t=1; t<=mk; t++){
                aux_matrix[i-1][t] = Gj[j-1]->matrix[i-1][t-1];
            }
        }
        Shj[j-1] = shj;
    }
}

void modulus_remainder(matrix matrix_1, matrix matrix_2, int i, int j, matrix answer){
    substract_matrix(matrix_1->matrix, i, j, matrix_2->matrix, answer->matrix);
}

int validate_parameters(int k, int n, uint8_t ** matrix) {
    //Verificar que (k,n) sea una de las compatibles y que la matriz sea cuadrada
    int valid = 0;
    if(k == 2 && n == 4){
        valid = 1;
    } else if(k == 4 && n == 8){
        valid = 1;
    }

    if(squared(matrix))
        valid = 1;
    else
        valid = 0;

    return valid;
}

matrix gen_random_matrix(int m, int k) {
    //Devuelve NULL si es error
    //Verificar que cumpla rank k con m>2(k-1)-1
    if(m <= 2*(k-1)-1){
        return NULL;
    }
    uint8_t **aux_matrix = new_matrix(m, k);
    matrix partial_matrix = build_matrix(m, k, A_MATRIX, aux_matrix);
    do {
        //Calcular la matriz random mxk

        //LLeno la matriz con random
        for (int i = 0; i < get_matrix_height(partial_matrix); i++) {
            for (int j = 0; j < get_matrix_width(partial_matrix); j++) {
                aux_matrix[i][j] = gen_random_number(0, 251);
            }
        }
    }
    while (linearly_independent(partial_matrix->height, partial_matrix->width, partial_matrix->matrix) == 0);

    return partial_matrix;
}

int squared(uint8_t ** matrix1){
    //Se fija si la matriz es cuadrada
    if(get_matrix_width(matrix1) == get_matrix_height(matrix1))
        return 1;
    return 0;
}

uint8_t gen_random_number(uint8_t a, uint8_t b){
    //Devuelve un numero random entre a y b, sin incluir b
    uint8_t r = (rand() % b) + a;
    return r;
}

struct share_computation_vector_struct {
    int longitud;
    uint8_t* vector;
};