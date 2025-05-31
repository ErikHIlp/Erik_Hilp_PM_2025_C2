#include <stdio.h>
#include <math.h>
/* Funcion matematica.
el programa obtiene el resultado de una funcion

OP y T son variales de tipo entero.
RES es una variable de tipo real. */

void main(void)
{
    int OP, T;
    float RES;
    printf("Ingrese la opción del cálculo y el valor entero: ");
     scanf("%d %d", &OP, &T);
    switch(OP)
    {
        case 1:
             RES = 6 * T/2;
             break;
        case 2:
             RES = pow(T,T);
             break;
        case 3:
        case 4:
             RES = 1;
             break;
        default: RES = 1;
     }
      printf("\nResulado:     %7.2f" , RES);

}
