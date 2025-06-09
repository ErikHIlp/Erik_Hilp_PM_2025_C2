#include <stdio.h>


int esPrimo(int num)
{
    int divisor;
    if (num < 2) return 0;
    for (divisor = 2; divisor < num; divisor++)
    {
        if (num % divisor == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{

    int N1, N2, numero, temp, encontrados;
    printf("BUSCADOR DE PRIMOS GEMELOS\n\n");
    printf("Ingrese el primer numero entero positivo (N1): ");
    scanf("%d", &N1);
    printf("Ingrese el segundo numero entero positivo (N2): ");
    scanf("%d", &N2);


    if (N1 <= 0 || N2 <= 0)
    {
        printf("\nError: Ambos numeros deben ser enteros positivos.\n");
        return 1;
    }

    if (N1 > N2)
    {
        temp = N1;
        N1 = N2;
        N2 = temp;
        printf("\nNota: Los numeros han sido intercambiados para el orden correcto.\n");
    }

    printf("\nPrimos gemelos entre %d y %d:\n", N1, N2);
    encontrados = 0;

    for (numero = N1; numero <= N2 - 2; numero++)
    {

        if (esPrimo(numero) && esPrimo(numero + 2))
        {

            if (numero >= N1 && (numero + 2) <= N2)
            {
                printf("(%d, %d) ", numero, numero + 2);
                encontrados = 1;
            }
        }
    }

    if (!encontrados)
    {
        printf("No se encontraron primos gemelos en el rango especificado.");
    }

    printf("\n\nBUSQUEDA COMPLETADA\n");

    return 0;
}
