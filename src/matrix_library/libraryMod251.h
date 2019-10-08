//
// Created by lococo on 07/06/19.
//

#ifndef MATRICES_RESOLVER_LIBRARYMOD251_H
#define MATRICES_RESOLVER_LIBRARYMOD251_H

#include <stdbool.h>

#define BASE 251;
#define COUNT_COLOR 256;


int inversoMod251[256];

int suma_mod251(int num1, int num2);
int resta_mod251(int num1, int num2);
int producto_mod251(int num1, int num2);
int dividir_mod251(int num1, int num2);
int get_inverso_mod251(int num);
void initialize_inverso();
int calcular_inverso_mod251(int num);
bool isMod251(int num);
int mod251(int num);

#endif //MATRICES_RESOLVER_LIBRARYMOD251_H
