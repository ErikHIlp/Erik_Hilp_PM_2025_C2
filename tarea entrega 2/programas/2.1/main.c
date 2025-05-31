#include <stdio.h>
/*promedio del curso.
el programa, al recibir como dato el promedio de un alumno en un curso
universitario escribe si este aprueva si su promerio es mayor o igaul a 6. */
void main(void)
{
    float PRO;
    printf("ingrese el promedio del alumno: ");
    scanf("%f" , &PRO);
    if (PRO >= 6)
        printf("\nAprobado");

}
