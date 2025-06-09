#include <stdio.h>

int main()
{
    float L[7];
    int CLA, CAN, boletos[7] = {0};
    float total = 0, monto;

    printf("Ingrese los precios:\n");
    for (int i = 1; i <= 6; i++)
        scanf("%f", &L[i]);

    printf("\nIngrese las ventas. Use 0 0 para terminar:\n");
    while (1)
    {
        scanf("%d %d", &CLA, &CAN);
        if (CLA == 0 && CAN == 0)
            break;
        monto = L[CLA] * CAN;
        printf("Venta: $%.2f\n", monto);
        boletos[CLA] += CAN;
        total += monto;
    }

    printf("\nBoletos vendidos por localidad:\n");
    for (int i = 1; i <= 6; i++)
        printf("L%d: %d boletos\n", i, boletos[i]);

    printf("\nRecaudacion total: $%.2f\n", total);
    return 0;
}
