#include <stdio.h>
/* Billete de ferrocarril.
el programa calcula el costo de un billete de ferrocarril teniendo en
cuenta la distancia entre las dos ciudades y el tiempo de permanencia
del pasajero.

DIS y TIE: variables de tipo entero.
BIL: variable de tipo real.*/

int main()
{
int DIS, TIE;
float BIL;
printf("ingrese la distancia entre ciudades y el tiempo de distancia:");
scanf("%d %d", &DIS, &TIE);
if ((DIS*2 > 500) && (TIE > 10))
    BIL = DIS * 2 * 0.19 * 0.8;
else
    BIL = DIS * 2 * 0.19;
    printf("\n\nCosto del billete: %7.f",BIL);
}
