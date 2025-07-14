#include <stdio.h>
#include <math.h>
/* serie de ULAM.
el programa al recibir como un dato entero positivo, obtiene y escribe
la serie ULAM.

NUM: variable de tipo entero. */

int main(void)
{
    int NUM;
    printf("Ingresa el numero para calcular la serie: ");
    scanf("%d", &NUM);
    if (NUM > 0)
    {
        printf("\nSerie de ULAM\n");
        printf("%d \t", NUM);
        while(NUM != 1)
        {
            if (pow(-1, NUM) > 0)
                NUM = NUM / 2;
            else
                NUM = NUM * 3 + 1;
            printf("%d \t", NUM);
        }
    }
    else
        printf("\n NUM debe ser entero positivo");
    return 0;
}
