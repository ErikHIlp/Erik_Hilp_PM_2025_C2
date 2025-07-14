#include <stdio.h>
#include <ctype.h>
#include <string.h>
/* Reemplaza palabras.
El programa lee cadenas de caracteres de un archivo y cada que vez que
encuentra la palabra Mexico escrita en forma incorrecta la primera con
minuscula la reemplaza por su forma correcta y escribe la cadena en otro
archivo. */
void cambia(FILE *, FILE *); /* Prototipo de funcion. Se pasan dos archivos como parametros. */

void main(void)
{
    FILE *ar;
    FILE *ap;
    ar = fopen("arc.txt", "r"); /* Se abre el archivo arc.txt para lectura. */
    ap = fopen("arc1.txt", "w"); /* Se abre el archivo arc1.txt para escritura. */
    if ((ar != NULL) && (ap != NULL))
    {
        cambia(ar, ap);
        fclose(ar);
        fclose(ap);
    }
    else
        printf("No se pueden abrir los archivos");
}

void cambia(FILE *ap1, FILE *ap2)
{
    /* Esta funcion reemplaza en la cadena de caracteres la palabra mexico escrita
    con minusculas la primera letra por su forma correcta y escribe la cadena
    de caracteres en un nuevo archivo. */
    int i, j, k;
    char cad[30], cad1[30], aux[30], *cad2;

    while (!feof(ap1))
    {
        fgets(cad, 30, ap1);
        strcpy(cad1, cad);
        cad2 = strstr(cad1, "mexico"); /* Localiza la subcadena mexico en cad1. */
        while (cad2 != NULL)
        {
            cad2[0] = 'M'; /* Reemplaza la letra minuscula por la mayuscula. */
            i = strlen(cad1);
            j = strlen(cad2);
            k = i - j; /* Diferencia de longitudes de cad1 y cad2. */
            if (k)
            {
                strncpy(aux, cad1, k);
                aux[k] = '\0';
                strcat(aux, cad2);
                strcpy(cad1, aux);
            }
            else
                strcpy(cad1, cad2);
            cad2 = strstr(cad1, "mexico");
        }
        fputs(cad1, ap2); /* Se escribe la cadena correcta en el archivo ap2. */
    }
}
