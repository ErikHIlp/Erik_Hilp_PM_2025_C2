#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOMBRE 90
#define ARCHIVO_ESTUDIANTES "estudiantes.dat"
#define PROMEDIO_MIN 0.0
#define PROMEDIO_MAX 100.0

typedef struct {
    char nombre[MAX_NOMBRE];
    int id;
    float promedio;
} Estudiante;

void guardarEstudiante(Estudiante e) {
    FILE *archivo = fopen(ARCHIVO_ESTUDIANTES, "ab");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo '%s' para guardar.\n", ARCHIVO_ESTUDIANTES);
        return;
    }
    fwrite(&e, sizeof(Estudiante), 1, archivo);
    fclose(archivo);
    printf("Estudiante guardado exitosamente con ID: %d\n", e.id);
}

int cargarEstudiantes(Estudiante **estudiantes) {
    FILE *archivo = fopen(ARCHIVO_ESTUDIANTES, "rb");
    if (archivo == NULL) {
        *estudiantes = NULL;
        return 0;
    }

    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);

    int total_estudiantes = tamano / sizeof(Estudiante);
    *estudiantes = (Estudiante *)malloc(total_estudiantes * sizeof(Estudiante));

    if (*estudiantes == NULL) {
        printf("Error: No hay memoria suficiente para cargar estudiantes.\n");
        fclose(archivo);
        return 0;
    }

    fread(*estudiantes, sizeof(Estudiante), total_estudiantes, archivo);
    fclose(archivo);
    return total_estudiantes;
}

void reescribirEstudiantes(Estudiante arr[], int n) {
    FILE *archivo = fopen(ARCHIVO_ESTUDIANTES, "wb");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo '%s' para reescribir.\n", ARCHIVO_ESTUDIANTES);
        return;
    }
    fwrite(arr, sizeof(Estudiante), n, archivo);
    fclose(archivo);
}

int obtenerProximoID() {
    FILE *archivo = fopen(ARCHIVO_ESTUDIANTES, "rb");
    if (archivo == NULL) {
        return 1;
    }

    int maxID = 0;
    Estudiante temp;
    while (fread(&temp, sizeof(Estudiante), 1, archivo)) {
        if (temp.id > maxID) {
            maxID = temp.id;
        }
    }
    fclose(archivo);
    return maxID + 1;
}

void bubbleSortPorNombre(Estudiante arr[], int n) {
    printf("Ordenando por nombre...\n");
    Estudiante temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j].nombre, arr[j + 1].nombre) > 0) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void bubbleSortPorPromedio(Estudiante arr[], int n) {
    printf("Ordenando por promedio...\n");
    Estudiante temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].promedio > arr[j + 1].promedio) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

float leerPromedioValido() {
    float promedio;
    int entradaValida;
    do {
        printf("Promedio (%.1f - %.1f): ", PROMEDIO_MIN, PROMEDIO_MAX);
        entradaValida = scanf("%f", &promedio);
        limpiarBuffer();
        if (entradaValida != 1) {
            printf("Error: Entrada no valida. Por favor, ingrese un numero.\n");
        } else if (promedio < PROMEDIO_MIN || promedio > PROMEDIO_MAX) {
            printf("Error: El promedio debe estar entre %.1f y %.1f\n", PROMEDIO_MIN, PROMEDIO_MAX);
        }
    } while (entradaValida != 1 || promedio < PROMEDIO_MIN || promedio > PROMEDIO_MAX);
    return promedio;
}

void anadirEstudiante() {
    Estudiante e;
    printf("\n ANADIR NUEVO ESTUDIANTE \n");
    e.id = obtenerProximoID();
    printf("El ID sera generado automaticamente: %d\n", e.id);
    printf("Nombre: ");
    fgets(e.nombre, sizeof(e.nombre), stdin);
    e.nombre[strcspn(e.nombre, "\n")] = 0;
    e.promedio = leerPromedioValido();
    guardarEstudiante(e);
}

void imprimirEstudiantes(Estudiante arr[], int n) {
    if (n == 0) {
        printf("No hay estudiantes para mostrar.\n");
        return;
    }
    printf("\n LISTA DE ESTUDIANTES \n");
    printf("%-5s %-25s %-10s\n", "ID", "NOMBRE", "PROMEDIO");
    printf("-\n");
    for (int i = 0; i < n; i++) {
        printf("%-5d %-25s %-10.2f\n", arr[i].id, arr[i].nombre, arr[i].promedio);
    }
    printf("-\n");
    printf("Total de estudiantes: %d\n", n);
}

void mostrarEstudiantesOrdenadosPorNombre() {
    Estudiante *lista;
    int n = cargarEstudiantes(&lista);
    if (n > 0) {
        bubbleSortPorNombre(lista, n);
        imprimirEstudiantes(lista, n);
        free(lista);
    } else {
        printf("No hay estudiantes para mostrar.\n");
    }
}

void mostrarEstudiantesOrdenadosPorPromedio() {
    Estudiante *lista;
    int n = cargarEstudiantes(&lista);
    if (n > 0) {
        bubbleSortPorPromedio(lista, n);
        imprimirEstudiantes(lista, n);
        free(lista);
    } else {
        printf("No hay estudiantes para mostrar.\n");
    }
}

void borrarEstudiante() {
    int id_a_borrar;
    printf("\n BORRAR ESTUDIANTE \n");
    printf("Ingrese el ID del estudiante a borrar: ");
    int entradaValida = scanf("%d", &id_a_borrar);
    limpiarBuffer();

    if (entradaValida != 1) {
        printf("Error: ID no valido. Por favor, ingrese un numero.\n");
        return;
    }

    Estudiante *lista_original = NULL;
    int n_original = cargarEstudiantes(&lista_original);

    if (n_original == 0) {
        printf("No hay estudiantes para borrar.\n");
        return;
    }

    Estudiante *lista_temporal = NULL;
    int n_temporal = 0;
    int encontrado = 0;

    lista_temporal = (Estudiante *)malloc(n_original * sizeof(Estudiante));
    if (lista_temporal == NULL) {
        printf("Error: No hay memoria suficiente para procesar la eliminacion.\n");
        free(lista_original);
        return;
    }

    for (int i = 0; i < n_original; i++) {
        if (lista_original[i].id == id_a_borrar) {
            encontrado = 1;
        } else {
            lista_temporal[n_temporal++] = lista_original[i];
        }
    }

    free(lista_original);

    if (!encontrado) {
        printf("No se encontro un estudiante con ID %d.\n", id_a_borrar);
        free(lista_temporal);
        return;
    }

    reescribirEstudiantes(lista_temporal, n_temporal);
    printf("Estudiante con ID %d borrado exitosamente.\n", id_a_borrar);

    free(lista_temporal);
}

void mostrarMenu() {
    printf("\n_________________________________________\n");
    printf("         SISTEMA DE GESTION DE ESTUDIANTES\n");
    printf("_________________________________________\n");
    printf(" 1. Anadir nuevo estudiante\n");
    printf(" 2. Mostrar lista por orden alfabetico (A-Z)\n");
    printf(" 3. Mostrar lista por promedio (De menor a mayor)\n");
    printf(" 4. Borrar estudiante por ID\n");
    printf(" 5. Salir del programa\n");
    printf("_________________________________________\n");
    printf("Ingrese su opcion: ");
}

int main() {
    srand(time(NULL));
    int opcion;
    printf("Bienvenido al Sistema de Gestion de Estudiantes!\n");

    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
            printf("Error: Entrada no valida. Por favor, ingrese un numero.\n");
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1:
                anadirEstudiante();
                break;
            case 2:
                mostrarEstudiantesOrdenadosPorNombre();
                break;
            case 3:
                mostrarEstudiantesOrdenadosPorPromedio();
                break;
            case 4:
                borrarEstudiante();
                break;
            case 5:
                printf("\nGracias por usar el sistema! Hasta luego!\n");
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion del 1 al 5.\n");
        }

        if (opcion != 5) {
            printf("\nPresione Enter para continuar...");
            getchar();
        }
    } while (opcion != 5);
}
