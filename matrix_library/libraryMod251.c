#include "libraryMod251.h"


bool inversoInitialized = false;

uint8_t suma_mod251(uint8_t num1, uint8_t num2)
{
    uint8_t n1, n2, ans;
    n1  = num1 % BASE;
    n2  = num2 % BASE;
    ans = n1 + n2;
    ans = ans % BASE;
    return ans;
}

uint8_t resta_mod251(uint8_t num1, uint8_t num2)
{
    uint8_t n1 = num1, n2 = num2, ans = 0;
    n1 = n1 % BASE;
    n2 = n2 % BASE;
    ans = n1 - n2;
    ans += BASE;
    ans = ans % BASE;

    return ans;
}

uint8_t producto_mod251(uint8_t num1, uint8_t num2)
{
    uint8_t n1, n2, ans;
    n1  = num1 % BASE;
    n2  = num2 % BASE;
    ans = n1 * n2;
    ans = ans % BASE;
    return ans;
}

uint8_t dividir_mod251(uint8_t num1, uint8_t num2)
{
    uint8_t n1, n2;
    n1 = num1 % BASE;
    n2 = get_inverso_mod251(num2);
    return producto_mod251(n1, n2);
}

uint8_t get_inverso_mod251(uint8_t num)
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

uint8_t calcular_inverso_mod251(uint8_t num)
{
    uint8_t dato;
    uint8_t var = COUNT_COLOR;
    uint8_t base = BASE;
    for (int inv = 0; inv  < var; inv++ ) {
        dato = num * inv;
        if ( (dato % base) == 1)
        {
            return inv;
        }
    }
    return 0;
}

uint8_t mod251(uint8_t num)
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

bool isMod251(uint8_t num)
{
    return (num >= 0 && num < 251)? true : false;
}