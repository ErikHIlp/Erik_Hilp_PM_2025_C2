#include <stdio.h>

/* Empresa textil.
El programa, al recibir un arreglo tridimensional que contiene informacion
sobre las ventas mensuales de tres departamentos en los ultimos ocho anos,
genera informacion estadistica valiosa para la empresa. */

#define MES 12
#define DEP 3
#define ANO 8

void Lectura(float [][DEP][ANO], int, int, int);
void Funcion1(float [][DEP][ANO], int, int, int);
void Funcion2(float [][DEP][ANO], int, int, int);
void Funcion3(float [][DEP][ANO], int, int, int);/* Prototipos
 de funciones. */

int main(void)
{
    float PRO[MES][DEP][ANO];
    Lectura(PRO, MES, DEP, ANO);
    Funcion1(PRO, MES, DEP, 2);
    Funcion2(PRO, MES, DEP, ANO);
    Funcion3(PRO, MES, DEP, ANO);

}

void Lectura(float A[][DEP][ANO], int F, int C, int P)
/* La funcion Lectura se utiliza para leer un arreglo tridimensional de tipo
real de F filas, C columnas y P planos de profundidad. */
{
    int K, I, J;
    for (K = 0; K < P; K++)
        for (I = 0; I < F; I++)
            for (J = 0; J < C; J++)
            {
                printf("Ano: %d\tMes: %d\tDepartamento: %d ", K+1, I+1, J+1);
                scanf("%f", &A[I][J][K]);
            }
}

void Funcion1(float A[][DEP][ANO], int F, int C, int P)
/* Esta funcion se utiliza para obtener las ventas totales de la empresa
en el segundo ano. */
{
    int I, J;
    float SUM = 0.0;
    for (I = 0; I < F; I++)
        for (J = 0; J < C; J++)
            SUM += A[I][J][P-1];
    printf("\n\nVentas totales de la empresa en el segundo ano: %.2f", SUM);
}

void Funcion2(float A[][DEP][ANO], int F, int C, int P)
/* Esta funcion se utiliza para obtener el departamento que tuvo las mayores
ventas en el ultimo ano. Genera ademas el importe de las ventas. */
{
    int I, J;
    float SUM1 = 0, SUM2 = 0, SUM3 = 0;
    for (I = 0; I < F; I++)
        for (J = 0; J < C; J++)
            switch (J+1)
            {
                case 1: SUM1 += A[I][J][P-1]; break;
                case 2: SUM2 += A[I][J][P-1]; break;
                case 3: SUM3 += A[I][J][P-1]; break;
            }
    if (SUM1 > SUM2 && SUM1 > SUM3)
    {
        printf("\n\nDepartamento con mayores ventas en el ultimo ano: Hilos");
        printf("\nVentas: %.2f", SUM1);
    }
    else if (SUM2 > SUM3)
    {
        printf("\n\nDepartamento con mayores ventas en el ultimo ano: Lanas");
        printf("\nVentas: %.2f", SUM2);
    }
    else
    {
        printf("\n\nDepartamento con mayores ventas en el ultimo ano: Licra");
        printf("\nVentas: %.2f", SUM3);
    }
}

void Funcion3(float A[][DEP][ANO], int F, int C, int P)
/* Esta funcion se utiliza para obtener el departamento, mes y ano con la mayor
venta. Escribe tambien el monto de las ventas. */
{
    int K, I, J, DE = 0, ME = 0, AN = 0;
    float VEN = A[0][0][0];
    for (K = 0; K < P; K++)
        for (I = 0; I < F; I++)
            for (J = 0; J < C; J++)
                if (A[I][J][K] > VEN)
                {
                    VEN = A[I][J][K];
                    DE = J;
                    ME = I;
                    AN = K;
                }
    printf("\n\nDepartamento: %d\tMes: %d\tAno: %d", DE+1, ME+1, AN+1);
    printf("\nVentas: %.2f", VEN);
}
