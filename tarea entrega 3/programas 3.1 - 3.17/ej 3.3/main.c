#include <stdio.h>
/* suma de pagos.
El programa, obtiene al recibir como datos un conjunto de pagos realizados en el ultimo mes,
obtiene la suma de los mismos.

PAG y SPA: variables de tipo real*/

void main(void)
{
    float PAG, SPA;
    SPA = 0;
    printf("Ingrese el primer pago:\t");
    scanf("%f", &PAG);
    while (PAG)
    /* observa que la condicion es verdadera mientras el pago es diferente de 0.*/
  {
    SPA = SPA + PAG;
    printf("ingrese el siguiente pago:\t");
    scanf("%f", &PAG);
    /*Observa que la proposicion que modifica la condicion es una lectura. */
  }
    printf("\nEl total de pagos del mes es: %.2f", SPA);


}
