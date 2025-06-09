#include <stdio.h>

int main()
{
    int N, i, j;

    printf("Ingrese un numero entero positivo (N): ");
    scanf("%d", &N);

    if (N <= 0)
    {
        printf("Por favor, ingrese un numero entero positivo.\n");
        return 1;
    }

    for (i = 1; i <= N; i++) {
        for (j = 1; j <= i; j++) {
            printf("%d ", j);
        }
        printf("\n");
    }

    for (i = N - 1; i >= 1; i--)
    {
        for (j = 1; j <= i; j++)
        {
            printf("%d ", j);
        }
        printf("\n");
    }

    return 0;
}
