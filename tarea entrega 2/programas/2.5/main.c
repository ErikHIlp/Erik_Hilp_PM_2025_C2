#include <stdio.h>
#include <math.h>
/* Funcion matematica.
el programa obtiene el resultado de una funcion

OP y T son variables de tipo entero.
RES es una variable de tipo real. */

int main(void)
{
    int OP, T;
    float RES;
    printf("Ingrese la opcion del calculo y el valor entero: ");
    scanf("%d %d", &OP, &T);
    switch(OP)
    {
        case 1:
            RES = 6 * T / 2;
            break;
        case 2:
            RES = pow(T, T);
            break;
        case 3:
        case 4:
            RES = 1;
            break;
        default: 
            RES = 1;
    }
    printf("\nResultado: %7.2f", RES);
    return 0;
}
