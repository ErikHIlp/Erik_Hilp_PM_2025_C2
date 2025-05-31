#include <stdio.h>
/* Pomedio curso.
el programa, al recibir como dato el promedio el promedio de un alumno en un curso
universitario, escribe aprovado si su promedio es mayor o igual a, o
reprobado en el caso contrario. */

void main(void)
{
    float PRO;
    printf("ingrese el promedio del alumno: ");
    scanf("%f", &PRO);
    if (PRO >= 6.0)
        printf("\nAaprovado");
    else
        printf("\nREprobado");

}
