#include <stdio.h>
#include <math.h>

int main() {
    int T, P, R;
    int limit = 10;
    printf("Valores positivos de T, P y R que satisfacen la expresion 7*T^4 - 6*P^3 + 12*R^5 < 5850:\n");
    for (T = 1; T <= limit; T++)
    {
        for (P = 1; P <= limit; P++)
        {
            for (R = 1; R <= limit; R++)
            {
                // Calcular el valor de la expresión
                double resultado = 7 * pow(T, 4) - 6 * pow(P, 3) + 12 * pow(R, 5);

                // Verificar si la expresión es menor que 5850
                if (resultado < 5850)
                {
                    printf("T = %d, P = %d, R = %d\n", T, P, R);
                }
            }
        }
    }

    return 0;
}
