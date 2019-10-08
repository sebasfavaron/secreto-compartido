//
// Created by sebas on 06/06/19.
//

#ifndef CRIPTO_SECRETO_COMPARTIDO_STEG_H
#define CRIPTO_SECRETO_COMPARTIDO_STEG_H

#include <stdint.h>

void duplicate_file(char* input, char* output);
void encode(char* input, char* secret, char* output);
void decode(char* input);
void example(char *argv[]);


#endif //CRIPTO_SECRETO_COMPARTIDO_STEG_H
