#include <stdio.h>
/* Alumnos.
El programa pregunta al usuario el numero de registro que desea
modificar, obtiene el nuevo promedio del alumno y modifica tanto el
registro como el archivo correspondiente. */
typedef struct /* Declaracion de la estructura alumno. */
{
    int matricula;
    char nombre[20];
    int carrera;
    float promedio;
} alumno;
void modifica(FILE *); /* Prototipo de funcion. */
void main(void)
{
    FILE *ar;
    if ((ar = fopen("ad1.dat", "r+")) != NULL)
    {
        modifica(ar);
        fclose(ar);
    }
    else
        printf("\nEl archivo no se puede abrir");
}
void modifica(FILE *ap)
/* Esta funcion se utiliza para modificar el promedio de un alumno. */
{
    int d;
    alumno alu;
    printf("\nIngrese el numero de registro que desea modificar: ");
    /* Observa que el lenguaje C almacena el primer registro en la
    posicion cero. Por lo tanto, si desea modificar el registro n,
    debe buscarlo en la posicion n-1. */
    scanf("%d", &d);
    fseek(ap, (d-1)*sizeof(alumno), 0);
    /* Observa que la instruccion fseek tiene tres argumentos. El primero
    indica que el apuntador se debe posicionar al inicio del FILE.
    El segundo senala el numero de bloques que debe moverse, en terminos
    de bytes, para llegar al registro correspondiente. Nota que el
    primer registro ocupa la posicion 0. Finalmente, el tercer argumento
    muestra a partir de que posicion se debe mover el bloque de bytes:
    se utiliza el 0 para indicar el inicio del archivo, 1 para expresar
    que se debe mover a partir de la posicion en la que actualmente se
    encuentra y 2 para indicar que el movimiento es a partir del fin del
    archivo. */
    fread(&alu, sizeof(alumno), 1, ap);
    /* Luego de posicionarnos en el registro que nos interesa, lo
    leemos. */
    printf("\nIngrese el promedio correcto del alumno: ");
    scanf("%f", &alu.promedio); /* Modificamos el registro con el
    nuevo promedio. */
    fseek(ap, (d-1)*sizeof(alumno), 0);
    /* Nos tenemos que posicionar nuevamente en el lugar correcto para
    escribir el registro modificado. Observe que si no hacemos este
    reposicionamiento, escribiriamos el registro actualizado en la
    siguiente posicion. */
    fwrite(&alu, sizeof(alumno), 1, ap);
}
