#include <stdio.h>
/* ventas descendientes.
el programa, al recibir como datos 3 valores que representan las ventas
de los vendedores de una tienda de discos, escribe las ventas en orden descendente.*/

int main(void)
{
    float P, S, R;
    printf("\n\nIngrese las ventas de los 3 vendedores: ");
    scanf("%f %f %f", &P, &S, &R);
    if (P > S && P > R)
    {
        if (S > R)
            printf("\n\n El orden es P, S y R: %8.2f %8.2f %8.2f", P, S, R);
        else
            printf("\n\n El orden es P, R y S: %8.2f %8.2f %8.2f", P, R, S);
    }
    else if (S > P && S > R)
    {
        if (P > R)
            printf("\n\n El orden es S, P y R: %8.2f %8.2f %8.2f", S, P, R);
        else
            printf("\n\n El orden es S, R y P: %8.2f %8.2f %8.2f", S, R, P);
    }
    else
    {
        if (P > S)
            printf("\n\n El orden es R, P y S: %8.2f %8.2f %8.2f", R, P, S);
        else
            printf("\n\n El orden es R, S y P: %8.2f %8.2f %8.2f", R, S, P);
    }
    return 0;
}
