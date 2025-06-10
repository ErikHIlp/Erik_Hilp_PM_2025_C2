#include <stdio.h>

/* Funciones y parámetros. */

int z, y;               /* Variables globales. */

int F1(float);
void F2(float, int *);  /* Prototipos de funciones. */

int main(void)
{
    int w;
    float x;
    z = 5;
    y = 7;
    w = 2;
    x = (float)y / z;
    printf("\nPrograma Principal: %d %d %.2f %d", z, y, x, w);
    F2(x, &w);
    printf("\nPrograma Principal: %d %d %.2f %d", z, y, x, w);
    return 0;
}

int F1(float x)
{
    int k;
    if (x != 0)
    {
        k = z - y;
        x++;
    }
    else
        k = z + y;
    printf("\nF1: %d %d %.2f %d", z, y, x, k);
    return k;
}

void F2(float t, int *r)
{
    int l;
    l = F1(t);
    if (l != 0)
        *r = l;
    else
        *r = z + y;
    printf("\nF2: %d %d %.2f %d", z, y, t, *r);
}
