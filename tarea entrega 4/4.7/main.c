#include <stdio.h>

/* Prueba de par�metros por valor. */

int f1(int);    /* Prototipo de funci�n. El par�metro es por valor y de tipo entero. */

void main(void)
{
    int I, K = 4;
    for (I = 1; I <= 3; I++)
    {
        printf("\n\nValor de K antes de llamar a la funci�n: %8d", ++K);
        printf("\nValor de K despu�s de llamar a la funci�n: %d", f1(K));
        /* Llamada a la funci�n f1. Se pasa una copia de la variable K. */
    }
}

int f1(int R)
/* La funci�n f1 recibe un par�metro por valor de tipo entero. */
{
    R += R;
    return (R);
}
