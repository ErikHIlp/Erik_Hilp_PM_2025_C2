#include <stdio.h>

int main() {
    int N1, N2, a, b, r;
    printf("Ingrese dos numeros naturales: ");
    scanf("%d %d", &N1, &N2);

    a = N1;
    b = N2;
    while (b != 0)
    {
        r = a % b;
        a = b;
        b = r;
    }

    printf("El MCD de %d y %d es: %d\n", N1, N2, a);
    return 0;
}
