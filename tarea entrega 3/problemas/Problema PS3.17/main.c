#include <stdio.h>

int main()
{
    int N;
    int i, j, k;

    printf("Ingrese un numero entero positivo (N): ");
    scanf("%d", &N);

    if (N <= 0) {
        printf("Error: Por favor, ingrese un numero entero positivo.\n");
        return 1;
    }

    for (i = 1; i <= N; i++) {
        for (j = 1; j <= i; j++) {
            printf("%d ", j);
        }

        for (k = i - 1; k >= 1; k--) {
            printf("%d ", k);
        }

        printf("\n");
    }

    return 0;
}
