#include <stdio.h>

/* Conflicto de variables con el mismo nombre. */

void f1(void);        /* Prototipo de funcion. */
int K = 5;            /* Variable global. */

int main(void)
{
    int I;
    for (I = 1; I <= 3; I++)
        f1();
    return 0;
}

void f1(void)
{
    /* La funcion utiliza tanto la variable local I como la variable global I. */
    {
        int Kl = 2;     /* Variable local. */
        Kl += Kl;
        printf("\n\nEl valor de la variable local es: %d", Kl);
        K = K + Kl;     /* Uso de ambas variables. */
        printf("\nEl valor de la variable global es: %d", K);
    }
}
