#include <stdio.h>

int main()
{
    int N, fila, col, espacios;


    printf("GENERADOR DE FIGURA NUMERICA SIMETRICA\n\n");
    printf("Ingrese un numero entero positivo (N): ");
    scanf("%d", &N);

    if (N <= 0) {
        printf("\nError: El numero debe ser entero positivo.\n");
        return 1;
    }

    printf("\nFigura generada para N = %d:\n\n", N);

    for (fila = 1; fila <= N; fila++)
    {


        for (col = 1; col <= fila; col++)
        {
            printf("%d ", col);
        }

        espacios = 2 * (N - fila) + 1;
        for (col = 1; col <= espacios; col++)
        {
            printf(" ");
        }

        if (fila > 1)
        {
            for (col = fila; col >= 1; col--) {
                printf("%d ", col);
            }
        } else
        {

            printf("1");
        }

        printf("\n");
    }

    printf("\nFIGURA\n");

    return 0;
}
