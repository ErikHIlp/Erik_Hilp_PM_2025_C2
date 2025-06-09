#include <stdio.h>

int main()
{
    int N, fila, col, numero, espacios;
    printf("GENERADOR DE PIRAMIDE NUMERICA\n\n");
    printf("Ingrese el numero de filas (N): ");
    scanf("%d", &N);

    if (N <= 0) {
        printf("\nError: El numero de filas debe ser positivo.\n");
        return 1;
    }

    if (N > 10) {
        printf("\nAdvertencia: Para N > 10, los números pueden no alinearse correctamente.\n");
    }

    printf("\nPiramide numerica para N = %d:\n\n", N);

    for (fila = 1; fila <= N; fila++)
    {

        espacios = N - fila;
        for (col = 1; col <= espacios; col++) {
            printf(" ");
        }

        numero = fila;
        for (col = 1; col <= fila; col++)
        {
            printf("%d", numero % 10);
            numero++;
        }


        if (fila > 1)
        {
            numero = fila + fila - 2;
            for (col = 1; col <= fila - 1; col++)
            {
                printf("%d", numero % 10);
                numero--;
            }
        }

        printf("\n");
    }

    printf("\nPIRAMIDE\n");

    return 0;
}
