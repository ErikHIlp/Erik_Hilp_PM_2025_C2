#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>

int main()
{
    int NUM, numero, divisor, esPrimo;
    printf("BUSCADOR DE NUMEROS PRIMOS\n\n");
    printf("Ingrese un numero entero positivo (NUM): ");
    scanf("%d", &NUM);

    if (NUM <= 1) {
        printf("\nNo hay numeros primos menores a %d.\n", NUM);
        return 0;
    }

    printf("\nNumeros primos menores a %d:\n", NUM);

    for (numero = 2; numero < NUM; numero++) {
        esPrimo = 1;

        for (divisor = 2; divisor < numero && esPrimo == 1; divisor++) {
            if (numero % divisor == 0) {
                esPrimo = 0;
            }
        }

        if (esPrimo == 1) {
            printf("%d ", numero);
        }
    }

    printf("\n\nBUSQUEDA COMPLETADA\n");

    return 0;
}
