#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura para un estudiante
typedef struct Estudiante {
    int codigo;
    char nombre[60];
    char apellido[60];
    int edad;
    float nota;
    struct Estudiante* siguiente;
} Estudiante;

// Función para crear un nuevo estudiante
Estudiante* crearEstudiante(int codigo, const char* nombre, const char* apellido, int edad, float nota) {
    Estudiante* nuevo = (Estudiante*)malloc(sizeof(Estudiante));
    if (!nuevo) {
        printf("Error de memoria.\n");
        return NULL;
    }
    nuevo->codigo = codigo;
    strncpy(nuevo->nombre, nombre, 59);
    nuevo->nombre[59] = '\0';
    strncpy(nuevo->apellido, apellido, 59);
    nuevo->apellido[59] = '\0';
    nuevo->edad = edad;
    nuevo->nota = nota;
    nuevo->siguiente = NULL;
    return nuevo;
}

// Insertar estudiante al final de la lista
void agregarEstudiante(Estudiante** cabeza, Estudiante* nuevo) {
    if (*cabeza == NULL) {
        *cabeza = nuevo;
        return;
    }
    Estudiante* actual = *cabeza;
    while (actual->siguiente) {
        actual = actual->siguiente;
    }
    actual->siguiente = nuevo;
}

// Mostrar todos los estudiantes
void mostrarEstudiantes(Estudiante* cabeza) {
    if (!cabeza) {
        printf("No hay estudiantes registrados.\n");
        return;
    }
    printf("\n%-5s %-15s %-15s %-5s %-6s\n", "COD", "Nombre", "Apellido", "Edad", "Nota");
    Estudiante* actual = cabeza;
    while (actual) {
        printf("%-5d %-15s %-15s %-5d %-6.2f\n", actual->codigo, actual->nombre, actual->apellido, actual->edad, actual->nota);
        actual = actual->siguiente;
    }
}

// Guardar estudiantes en archivo binario
void guardarArchivo(Estudiante* cabeza, const char* archivo) {
    FILE* f = fopen(archivo, "wb");
    if (!f) {
        printf("No se pudo abrir el archivo para guardar.\n");
        return;
    }
    Estudiante* actual = cabeza;
    while (actual) {
        fwrite(&(actual->codigo), sizeof(int), 1, f);
        fwrite(actual->nombre, sizeof(char), 60, f);
        fwrite(actual->apellido, sizeof(char), 60, f);
        fwrite(&(actual->edad), sizeof(int), 1, f);
        fwrite(&(actual->nota), sizeof(float), 1, f);
        actual = actual->siguiente;
    }
    fclose(f);
    printf("Estudiantes guardados en '%s'.\n", archivo);
}

// Cargar estudiantes desde archivo binario
Estudiante* cargarArchivo(const char* archivo) {
    FILE* f = fopen(archivo, "rb");
    if (!f) {
        printf("No se pudo abrir el archivo para cargar.\n");
        return NULL;
    }
    Estudiante* cabeza = NULL;
    int codigo, edad;
    char nombre[60], apellido[60];
    float nota;
    while (fread(&codigo, sizeof(int), 1, f) == 1) {
        fread(nombre, sizeof(char), 60, f);
        fread(apellido, sizeof(char), 60, f);
        fread(&edad, sizeof(int), 1, f);
        fread(&nota, sizeof(float), 1, f);
        Estudiante* nuevo = crearEstudiante(codigo, nombre, apellido, edad, nota);
        if (nuevo) agregarEstudiante(&cabeza, nuevo);
    }
    fclose(f);
    printf("Datos cargados de '%s'.\n", archivo);
    return cabeza;
}

// Liberar memoria de la lista
void liberarLista(Estudiante* cabeza) {
    while (cabeza) {
        Estudiante* temp = cabeza;
        cabeza = cabeza->siguiente;
        free(temp);
    }
}

// Solicitar datos de un estudiante
void pedirDatos(int* codigo, char* nombre, char* apellido, int* edad, float* nota) {
    printf("Código: ");
    scanf("%d", codigo); getchar();
    printf("Nombre: ");
    fgets(nombre, 60, stdin); nombre[strcspn(nombre, "\n")] = '\0';
    printf("Apellido: ");
    fgets(apellido, 60, stdin); apellido[strcspn(apellido, "\n")] = '\0';
    printf("Edad: ");
    scanf("%d", edad);
    printf("Nota: ");
    scanf("%f", nota);
}

int main() {
    Estudiante* lista = NULL;
    int opcion;
    const char* archivo = "estudiantes.dat";
    do {
        printf("\n1. Agregar estudiante\n2. Mostrar estudiantes\n3. Guardar en archivo\n4. Cargar desde archivo\n5. Salir\nOpción: ");
        scanf("%d", &opcion);
        getchar();
        switch (opcion) {
            case 1: {
                int codigo, edad; char nombre[60], apellido[60]; float nota;
                pedirDatos(&codigo, nombre, apellido, &edad, &nota);
                Estudiante* nuevo = crearEstudiante(codigo, nombre, apellido, edad, nota);
                if (nuevo) agregarEstudiante(&lista, nuevo);
                break;
            }
            case 2:
                mostrarEstudiantes(lista);
                break;
            case 3:
                guardarArchivo(lista, archivo);
                break;
            case 4:
                liberarLista(lista);
                lista = cargarArchivo(archivo);
                break;
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 5);
    liberarLista(lista);
    return 0;
} 