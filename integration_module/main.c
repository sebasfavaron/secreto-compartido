//
// Created by sebas on 20/06/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <secret_workflow.h>
#include <matrixLibrary.h>
#include <math.h>
#include <steg.h>
#include "bmp.h"

enum mode{DISTRIBUTION, RETRIEVAL};
typedef struct {
    enum mode _mode;
    char *secret_image;
    char *watermark;
    int k;
    int n;
    char *directory;
} parameters;

void printParameters(parameters *params){
    printf("/-----------------------------------------------------\\\n");
    printf("|\tParameters:\n"
           "|\t\tWatermark: %s\n"
           "|\t\tSecret: %s\n"
           "|\t\tk: %d\n"
           "|\t\tn: %d\n"
           "|\t\tDir: %s\n"
           "|\t\tMode: %d\n", params->watermark, params->secret_image, params->k, params->n, params->directory, params->_mode);
    printf("\\-----------------------------------------------------/\n");
}

void encrypt(parameters *p){

}

void decrypt(){

}

void encrypt_decrypt_test(){
    int k = 2;//params->k;
    int n = 4;//params->n;
    int m = 4;

    char * name = "../../bmp_library/images/Secreto.bmp";
    FILE * file_pointer = fopen(name, "rb");
    char * secreto_char = (char*)get_pixelarray(get_header(file_pointer), file_pointer);
    int secreto_size = get_header(file_pointer)->bitmapinfoheader.imagesize;
    uint8_t * secreto = newVector(secreto_size);
    for(int i=0; i<secreto_size; i++){
        secreto[i] = (uint8_t)secreto_char[i];
    }

    modularizar(secreto, secreto_size);

    char * name2 = "../../bmp_library/images/Marca.bmp";
    FILE * file_pointer2 = fopen(name2, "rb");
    char * marca_char = (char*)get_pixelarray(get_header(file_pointer2), file_pointer2);
    int marca_size = get_header(file_pointer2)->bitmapinfoheader.imagesize;
    uint8_t * marca = newVector(marca_size);
    for(int i=0; i<marca_size; i++){
        marca[i] = (uint8_t)marca_char[i];
    }

    modularizar(marca, marca_size);


    int size;
    matrix * secretos = get_S_matrices(secreto_size, secreto, n, &size);

    matrix * marcas = get_S_matrices(secreto_size, marca, n, &size);

    matrix * Rw = malloc(size * sizeof(matrix));

    matrix ** Shj = malloc(size * sizeof(matrix*));

    uint8_t *** new_secretos = malloc(size * sizeof(uint8_t **));
    uint8_t *** new_marcas = malloc(size * sizeof(uint8_t **));

    uint8_t * new_secreto = newVector(secreto_size);

    for(int i=0; i<size; i++){
        uint8_t ** aux_matrix = new_matrix(n,n);
        Rw[i] = build_matrix(n,n,RW_MATRIX, aux_matrix);
        Shj[i] = malloc(n * sizeof(matrix));
        share_construction(k, n, secretos[i]->matrix, n, Rw[i], Shj[i], marcas[i]->matrix);
        new_secretos[i] = new_matrix(n, n);
        new_marcas[i] = new_matrix(n, n);
        share_reconstruction(k, n, Shj[i], Rw[i], new_secretos[i], new_marcas[i]);
        for(int j=0; j<n; j++){
            for(int l=0; l<n; l++){
                int selector = i * (n * n) + n * j + l;
                if(selector == 2400){
                    printMatrix(secretos[i-1]->matrix, secretos[i-1]->height, secretos[i]->width);
                    printMatrix(new_secretos[i-1], n, n);
                    printf("%d", i);
                }
                new_secreto[selector] = new_secretos[i][j][l];
            }
        }
    }

//    for(int i=0; i<secreto_size/16; i++){
//        printf("%d ", secreto[i]);
//    }
//    printf("\n");
//    for(int i=0; i<secreto_size/16; i++){
//        printf("%d ", new_secreto[i]);
//    }

    for(int i=0; i<secreto_size; i++){
        if(secreto[i] != new_secreto[i]){
            printf("Fallo en %d: %d %d", i, secreto[i], new_secreto[i]);
            break;
        }
    }
    printf("\n");
    for(int i=0; i<secreto_size; i++){
        printf("%c ", new_secreto[i]);
    }

    FILE *p_sherlock = fopen("../../bmp_library/images/shares/sherlockshare.bmp", "rb");
    bitmap *sherlock_bitmap = get_header(p_sherlock);
    char *new_secreto_name = "new_secreto.bmp";
    create_bmp_file(new_secreto_name, sherlock_bitmap->bitmapinfoheader.width, sherlock_bitmap->bitmapinfoheader.height, 8);
    fclose(p_sherlock);

    replace_pixelarray(new_secreto_name, new_secreto);
}

void matrix_test() {
    matrix m = build_matrix(4, 3, S_MATRIX, new_matrix(3,4));

    for(int y=0; y<m->height; y++){
        for(int x=0; x<m->width; x++){
            m->matrix[x][y] = rand()%256;
            printf("%d ", m->matrix[x][y]);
        }
        printf("\n");
    }


}

int main (int argc , char *argv[]) {

    /*int opt;
    static struct option long_options[] = {
            { "dir", required_argument, NULL, 'z' },
            { NULL, 0, NULL, 0 }
    };

    parameters *params = (parameters*)calloc(1, sizeof(parameters));
    int long_index = 0;
    while ((opt = getopt_long(argc, argv, "drs:m:k:n:z:", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'd':
                params->_mode = DISTRIBUTION;
                break;
            case 'r':
                params->_mode = RETRIEVAL;
                break;
            case 's':
                params->secret_image = optarg;
                break;
            case 'm':
                params->watermark = optarg;
                break;
            case 'k':
                params->k = atoi(optarg);
                if(params->k != 2 && params->k != 4){
                    fprintf(stderr, "k must be 2 or 4\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'n':
                params->n = atoi(optarg);
                if(params->n != 4 && params->n != 8){
                    fprintf(stderr, "n must be 4 or 8\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'z':
                // --dir option
                params->directory = optarg;
                break;
            case '?':
                break;
            default:
                fprintf(stderr, "Usage: %s [-drsmkn|--dir]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    printParameters(params);


    if(!((k == 2 && n == 4) || (k == 4 && n == 8))){
        fprintf(stderr, "Scheme must be (2,4) or (4,8)\n");
        exit(EXIT_FAILURE);
    }*/

    encrypt_decrypt_test();
}