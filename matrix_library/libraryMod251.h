#ifndef MATRICES_RESOLVER_LIBRARYMOD251_H
#define MATRICES_RESOLVER_LIBRARYMOD251_H

#include <stdbool.h>
#include <stdint.h>

#define BASE 251;
#define COUNT_COLOR 256;


uint8_t inversoMod251[256];

uint8_t suma_mod251(uint8_t num1, uint8_t num2);
uint8_t resta_mod251(uint8_t num1, uint8_t num2);
uint8_t producto_mod251(uint8_t num1, uint8_t num2);
uint8_t dividir_mod251(uint8_t num1, uint8_t num2);
uint8_t get_inverso_mod251(uint8_t num);
void initialize_inverso();
uint8_t calcular_inverso_mod251(uint8_t num);
bool isMod251(uint8_t num);
uint8_t mod251(uint8_t num);

#endif //MATRICES_RESOLVER_LIBRARYMOD251_H
