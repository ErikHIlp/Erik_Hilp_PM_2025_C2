#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include "stb_image_write.h"

unsigned char a_gris(unsigned char r, unsigned char g, unsigned char b) {
    return (unsigned char)(0.3 * r + 0.59 * g + 0.11 * b);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s archivo.jpg\n", argv[0]);
        return 1;
    }

    int ancho, alto, canales;
    unsigned char *imagen = stbi_load(argv[1], &ancho, &alto, &canales, 3);
    if (!imagen) {
        printf("No se pudo cargar la imagen: %s\n", argv[1]);
        return 1;
    }

    printf("Imagen cargada: %dx%d píxeles, %d canales.\n", ancho, alto, canales);
    unsigned char *imagen_gris = malloc(ancho * alto);
    if (!imagen_gris) {
        printf("Error al asignar memoria.\n");
        stbi_image_free(imagen);
        return 1;
    }

    for (int i = 0; i < ancho * alto; i++) {
        unsigned char r = imagen[i * 3 + 0];
        unsigned char g = imagen[i * 3 + 1];
        unsigned char b = imagen[i * 3 + 2];
        imagen_gris[i] = a_gris(r, g, b);
    }

    if (stbi_write_png("imagen_gris.png", ancho, alto, 1, imagen_gris, ancho)) {
        printf("Imagen en escala de grises guardada como imagen_gris.png\n");
    } else {
        printf("Error al guardar la imagen.\n");
    }

    stbi_image_free(imagen);
    free(imagen_gris);

    return 0;
}