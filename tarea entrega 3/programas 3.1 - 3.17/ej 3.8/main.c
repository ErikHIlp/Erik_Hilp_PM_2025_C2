#include <stdio.h>
/*factorial.
el programa calcula el factorial de un numero entero.

FAC, I, NUM: son variables de tipo entero. */

void main(void)
{
    int I, NUM;
    long FAC;
    printf("\nIngrese el numero: ");
    scanf("%d", &NUM);
    if (NUM >= 0)
    {
        FAC = 1;
        for (I = 1; I <= NUM; I++)
            FAC *= I;

        printf("\nEl factorial de %d es %ld\n", NUM, FAC);
        }
        else
            printf("\nError en el dato ingresado");

}
