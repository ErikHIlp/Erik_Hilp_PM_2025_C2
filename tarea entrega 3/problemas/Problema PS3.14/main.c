#include <stdio.h>
#include <math.h>

int main() {
    int x, n = 0, signo = 1;
    float term, sen = 0.0, anterior;

    printf("Ingrese el valor de x:");
    scanf("%d", &x);

    float rad = x * M_PI / 180.0;
    do {
        anterior = sen;
        term = signo * pow(rad, 2 * n + 1) / tgamma(2 * n + 2);
        sen += term;
        signo *= -1;
        n++;
    } while (fabs(sen - anterior) > 0.001);

    printf("sen(%d) ≈ %.4f\n", x, sen);
    printf("Terminos requeridos: %d\n", n);
    return 0;
}
