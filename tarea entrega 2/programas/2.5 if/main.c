#include <stdio.h>
#include <math.h>

int main(void)
{
int OP, T;
float RES;
    printf("Ingrese la opcion del calculo y el valor entero: ");
    scanf("%d %d", &OP, &T);

    if (OP == 1)
    {
        RES = 6 * T / 2;
    }
    else if (OP == 2)
    {
        RES = pow(T,T);
    }
    else if  (OP == 3 || OP == 4)
    {
        RES = 1;
    }
    else
    {
        RES = 1;
    }

    printf("\nResutado: %7.2f" , RES);

    return 0;


}
