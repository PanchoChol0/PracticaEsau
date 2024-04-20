//
// Created by 52552 on 19/04/2024.
//
#include <stdio.h>
#include <iostream>
#include <float.h>
#include <stdlib.h>
#include <cmath>
#include <sys/time.h>
#define N 1000

// La estructura para representar un punto en un plano en 2D
struct Point
{
    int x, y;
};

/* Ordena el arreglo de puntos acordes a la cordenada x*/
int compareX(const void* a, const void* b)
{
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return (p1->x - p2->x);
}
// Ordena el arreglo de puntos acordes a la coordenada y
int compareY(const void* a, const void* b) {
    Point *p1 = (Point *) a, *p2 = (Point *) b;
    return (p1->y - p2->y);
}
// Una funcion de utilidad para encontrar la distancia entre los 2 puntos
float dist(Point p1, Point p2)
{
    return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
                 (p1.y - p2.y)*(p1.y - p2.y)
    );
}
/* Un metodo de fuerza bruta para regresar la distancia mas pequeña entre 2 puntos
  en P[] de tamaño n */
float bruteForce(Point P[], int n)
{
    float min = FLT_MAX;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}

// Una funcion de utilidad para encontrar el minimo de 2 valores flotantes
float min(float x, float y)
{
    return (x < y)? x : y;
}

/* Una funcion de utilidad para encontrar la distancia entre los puntos de banda de un tamaño dado. Todos los puntos en strip[]
 * son ordenados acorde a la coordenada y. Todos ellos tiene un limite superior en una minima distancia como d.
 * Nota que este metodo parece ser O(n^2), pero es un metodo O(n) como el bucle interno corre al menos 6 veces */
float stripClosest(Point strip[], int size, float d)
{
    float min = d; // Inicializa la minima distancia como d

    qsort(strip, size, sizeof(Point), compareY);
    /* Toma todos los puntos de uno en uno y prueba los puntos siguientes hasta que la diferencia entre
     * las coordenadas y sea mas pequeña que d, Este es un hecho probado que este bucle empieza a correr al menos 6 veces */

    for (int i = 0; i < size; ++i)
        for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i],strip[j]) < min)
                min = dist(strip[i], strip[j]);

    return min;
}

/* Una funcion recursiva para encontrar la distancia mas pequeña, El arreglo P contiene
 * todos los puntos ordenados acordes para las coordenadas x */
float closestUtil(Point P[], int n)
{
    // si hay 2 o 3 puntos, entonces se usa fuerza bruta
    if (n <= 3)
        return bruteForce(P, n);

    // Encuentra el punto medio

    int mid = n/2;
    Point midPoint = P[mid];

    /* Considere la linea vertical pasando a traves del punto medio calculando la
     * distancia mas pequeña dl sobre izquierda del punto medio y dr sobre el lado derecho */
    float dl = closestUtil(P, mid);
    float dr = closestUtil(P + mid, n-mid);

    // Encuentre el mas pequeño de 2 distancias
    float d = min(dl, dr);

    // Construye un arreglo strip[] que contenga los puntos mas cercanos (mas cerca que d)
    // a la linea pasando a traves del punto medio
    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(P[i].x - midPoint.x) < d)
            strip[j] = P[i], j++;

    // Encuentre los puntos mas cercanos en strip. Regresa la minima de d y la mas distancia mas
    // cerca es strip
    return min(d, stripClosest(strip, j, d) );
}

/* La principal funcion que encuentra la distancia mas pequeña este metodo principalmente
 * usa closestUtil() */
float closest(Point P[], int n)
{
    qsort(P, n, sizeof(Point), compareX);
    // Usa la funcion recursiva closestUtil() para encontrar la distancia mas pequeña
    return closestUtil(P, n);
}

int main()
{
    float m, b, sumax, sumay, sumaxy, sumax2;
    Point P[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
    int n = sizeof(P) / sizeof(P[0]);
    float x[N] = {2,12,40,5,12,3};
    float y[N] = {3,30,50,1,10,4};;
    double sum = 0;
    double add = 1;

    static  unsigned long pre_time;
    static unsigned long pre_secs;
    struct timespec ts;
    unsigned long long tiempo;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    pre_time = ts.tv_nsec;
    pre_secs = ts.tv_sec;

    sumax = sumay = sumaxy = sumax2 = 0;
    for (int i = 0; i < n; ++i) {
        /* suma de productos */
        sumaxy += x[i]*y[i];
        /* Suma de valores de x^2 */
        sumax2 += x[i]*x[i];
        /* Suma de valores de x */
        sumax += x[i];
        /* Suma de valores de y */
        sumay += y[i];
    }
    /* Calulamos la pendiente (m) y la interseccion (b) */
    m = (n * sumaxy - sumax * sumay) / (n * sumax2 - sumax * sumax);
    b = (sumay - m*sumax) / n;

    /* Se muestran los valores de la pendiente y de la interseccion */
    printf("Pendiente(m) = %f\n",m);
    printf("Interseccion(b) = %f\n",b);

    /* La formula para calcular la variable dependiente(Y) es: */
    printf("Valor de la variable dependeinte es y = %fx + (%f)\n", m, b);

    int iterations = 1000*1000*1000;
    for (int i = 0; i < iterations; ++i) {
        sum += add;
        add /= 2.0;
    }

    printf("La distancia mas pequenia es: %f ", closest(P, n));

    clock_gettime(CLOCK_MONOTONIC, &ts);
    tiempo = (ts.tv_sec - pre_secs) * 1000000000 + ts.tv_nsec - pre_time;
    printf("\nTiempo medido: %llu\n", tiempo);

    system("pause");
    return 0;
}

