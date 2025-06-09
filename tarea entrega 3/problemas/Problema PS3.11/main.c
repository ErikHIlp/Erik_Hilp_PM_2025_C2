#include <stdio.h>

int main() {

    int N, i;

    float vin[4];
    float totalTipo1, totalTipo2, totalTipo3, totalTipo4, totalAnual;
    totalTipo1 = totalTipo2 = totalTipo3 = totalTipo4 = 0;

    printf("SISTEMA DE PRODUCCION DE VINOS - BODEGA TARIJA\n\n");
    printf("Ingrese el numero de años (N): ");
    scanf("%d", &N);


    if (N <= 0)
    {
        printf("Error: El numero de años debe ser mayor que 0.\n");
        return 1;
    }

    printf("\nIngrese la produccion de vinos (en litros):\n");
    printf("Formato: Tipo1 Tipo2 Tipo3 Tipo4\n\n");
    for (i = 1; i <= N; i++)
    {
        printf("Año %d: ", i);
        scanf("%f %f %f %f", &vin[0], &vin[1], &vin[2], &vin[3]);

        totalAnual = vin[0] + vin[1] + vin[2] + vin[3];
        printf("  -> Total producido en el año %d: %.2f litros\n", i, totalAnual);

        totalTipo1 += vin[0];
        totalTipo2 += vin[1];
        totalTipo3 += vin[2];
        totalTipo4 += vin[3];
    }

    printf("\nRESUMEN DE PRODUCCION\n");
    printf("\nTOTAL PRODUCIDO POR TIPO DE VINO:\n");
    printf("Tipo 1: %.2f litros\n", totalTipo1);
    printf("Tipo 2: %.2f litros\n", totalTipo2);
    printf("Tipo 3: %.2f litros\n", totalTipo3);
    printf("Tipo 4: %.2f litros\n", totalTipo4);

    printf("\nTOTAL GENERAL: %.2f litros\n", totalTipo1 + totalTipo2 + totalTipo3 + totalTipo4);

    return 0;
}
