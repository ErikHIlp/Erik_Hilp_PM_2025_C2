#include <stdio.h>
/* Promedio curso.
el programa, al recibir como dato el promedio de un alumno en un curso
universitario, escribe aprobado si su promedio es mayor o igual a 6, o
reprobado en el caso contrario. */

int main(void)
{
    float PRO;
    printf("ingrese el promedio del alumno: ");
    scanf("%f", &PRO);
    if (PRO >= 6.0)
        printf("\nAprobado");
    else
        printf("\nReprobado");
    return 0;
}
