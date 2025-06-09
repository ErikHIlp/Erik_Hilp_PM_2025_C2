#include <stdio.h>
/*suma pagos
El programa obtiene el suma de los pagos realizados el ulyimo mes.
PAG y SPA: son variables de tipo real*/
void main(void)
{
    float PAG, SPA = 0;
    printf("ingrese el primer pago:\t");
    scanf("%f", &PAG);
    /*Observa que utilizar la estructura do-while al menos se necesita un pago*/
    do
     {
         SPA = SPA + PAG;
         printf("Ingrese el siguiente pago -0 para terminar-:\t");
         scanf("%f", &PAG);
       }
       while (PAG);
       printf("\nEl total de pagos del mes es: %.2f", SPA);


}
