#include <stdio.h>
/* calificaciones.
El programa al recibir un grupo de calificaciones de un alumno obtiene el promedio
de calificaciones de cada uno de ellos, ademas, los alumnos con el mejor
y peor promedio.

I, MAT, MAMAT, MEMAT: variables de tipo entero.
CAL, SUM, MAPRO, MEPRO, y PRO: variables de tipo real*/

int main(void)
{
    int I, MAT, MAMAT, MEMAT;
    float SUM, PRO, CAL, MAPRO = 0.0, MEPRO = 11.0;
    printf("Ingrese la matricula del primer alumno:\t");
    scanf("%d", &MAT);
    while (MAT)
    {
        SUM = 0;
        for (I = 1; I <= 5; I++)
        {
            printf("\tIngrese la calificacion del alumno: ");
            scanf("%f", &CAL);
            SUM += CAL;
        }
        PRO = SUM / 5;
        printf("\nMatricula:%d\tpromedio:%5.2f", MAT, PRO);
        if (PRO > MAPRO)
        {
            MAPRO = PRO;
            MAMAT = MAT;
        }
        if (PRO < MEPRO)
        {
            MEPRO = PRO;
            MEMAT = MAT;
        }
        printf("\n\nIngrese la matricula del siguiente alumno: ");
        scanf("%d", &MAT);
    }
    printf("\n\nAlumno con mejor promedio:\t%d\t%5.2f", MAMAT, MAPRO);
    printf("\nAlumno con peor promedio:\t%d\t%5.2f\n", MEMAT, MEPRO);
    return 0;
}
