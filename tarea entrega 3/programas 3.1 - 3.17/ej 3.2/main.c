#include <stdio.h>
/* suma positivos.
el programa, al recibir como datos N numeros enteros
obtiene la suma de los enteros positivos.
I, N, NUM, SUM: son variables de numero entero.*/

void main(void)
{
    int I, N, NUM, SUM;
    SUM = 0;
    printf("Ingrese el numero de datos:\t");
    scanf("%d", &N);
    for (I = 1; I <= N; I++)
    {
            printf("Ingrese el dato numero %d", SUM);
            scanf("%d", &NUM);
            if (NUM > 0)
                SUM = SUM + NUM;
            }
            printf("\nLa suma de los numeros positivos es: %d", SUM);

}
