#include <stdio.h>

/* Calcula longitud en forma recursiva.
El programa calcula de manera recursiva la longitud de la cadena sin utilizar
la funcion strlen. */

int cuenta(char *);    /* Prototipo de funcion. */

void main(void)
{
    int i;
    char cad[50];
    printf("\nIngrese la cadena de caracteres: ");
    gets(cad);
    i = cuenta(cad);
    printf("\nLongitud de la cadena: %d", i);
}

int cuenta(char *cadena)
/* Esta funcion calcula la longitud de la cadena en forma recursiva. Es
importante tener conocimientos tanto de pilas como de recursividad para
comprender la solucion propuesta, aunque esta sea muy simple. Observa que
mientras no lleguemos al ultimo caracter de la cadena, incrementamos la
cuenta en uno y llamamos a la funcion con el siguiente caracter. */
{
    if (cadena[0] == '\0')
        return 0;
    else
        return (1 + cuenta(&cadena[1]));
}
