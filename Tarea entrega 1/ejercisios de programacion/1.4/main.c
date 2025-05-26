#include <stdio.h>

void main(void)
{
    float BAS, ALT, SUP;
    printf("ingrese la base y l aaltura de un triangulo:");
    scanf("%f %f" , &BAS, &ALT);
    SUP = BAS * ALT / 2;
    printf("\nLa superficie del triangulo es: %5.2f ", SUP);

}
