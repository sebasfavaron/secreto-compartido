//
// Created by lococo on 07/06/19.
//

#include "libraryMod251.h"


bool inversoInitialized = false;

int suma_mod251(int num1, int num2)
{
    int n1, n2, ans;
    n1  = num1 % BASE;
    n2  = num2 % BASE;
    ans = n1 + n2;
    ans = ans % BASE;
    return ans;
}

int resta_mod251(int num1, int num2)
{
    int n1 = num1, n2 = num2, ans = 0;
    n1 = n1 % BASE;
    n2 = n2 % BASE;
    ans = n1 - n2;
    ans = ans % BASE;
    return ans;
}

int producto_mod251(int num1, int num2)
{
    int n1, n2, ans;
    n1  = num1 % BASE;
    n2  = num2 % BASE;
    ans = n1 * n2;
    ans = ans % BASE;
    return ans;
}

int dividir_mod251(int num1, int num2)
{
    int n1, n2;
    n1 = num1 % BASE;
    n2 = get_inverso_mod251(num2);
    return producto_mod251(n1, n2);
}

int get_inverso_mod251(int num)
{
    if (!inversoInitialized)
    {
        initialize_inverso();
    }
    return inversoMod251[num];
}

void initialize_inverso()
{
    for (int i = 0; i < 256; i++)
    {
        inversoMod251[i] = calcular_inverso_mod251(i);
    }
    inversoInitialized = true;
}

int calcular_inverso_mod251(int num)
{
    int dato;
    int var = COUNT_COLOR;
    int base = BASE;
    for (int inv = 0; inv  < var; inv++ ) {
        dato = num * inv;
        if ( (dato % base) == 1)
        {
            return inv;
        }
    }
    return 0;
}

int mod251(int num)
{
    if (isMod251(num))
    {
        return num;
    }
    if (num < 0)
    {
        while (!isMod251(num))
        {
            num = num + 251;
        }
        return num;
    }
    while (!isMod251(num))
    {
        num = num - 251;
    }
    return num;
}

bool isMod251(int num)
{
    return (num >= 0 && num < 251)? true : false;
}