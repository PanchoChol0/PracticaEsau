#include <stdio.h>
#include <iostream>
#include <fstream>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <sys/time.h>
#define N 1000
// La estructura para representar un punto en un plano en 2D
struct Point
{
    int x, y;
};

//Empieza Regresion Polinomial
using namespace std;
void regresionPoli (double x[], double y[], int n) {
    const int grado = 2;

    double matrix[grado + 1][grado + 1];
    double solutions[grado + 1];

    double sumX = 0;
    double sumXY = 0;

    printf("-----------SISTEMA DE ECUACIONES-----------\n");

    for (int i = 0; i < grado + 1; ++i) {
        sumXY = 0;
        for (int k = 0; k < n; ++k) {
            sumXY += pow(x[k], i) * y[k];
        }
        solutions[i] = sumXY;
        for (int j = 0; j < grado + 1; ++j) {
            if (i == 0 && j == 0) {
                matrix[i][j] = n;
            } else {
                sumX = 0;
                for (int k = 0; k < n; ++k) {
                    sumX += pow(x[k], (j + i));
                }
                matrix[i][j] = sumX;
            }
            cout << matrix[i][j] << ", ";
        }
        cout << "= " << solutions[i] << endl;
    }
    printf("---------------GAUSS SEIDEL----------------\n");
    int size = grado + 1;
    double Epsilon = 0.0000001;

    double S[size];
    double OS[size];
    double E[size];
    for (int i = 0; i < size; ++i) {
        S[i] = 0;
        OS[i] = 0;

        E[i] = 0;
    }
    bool errorCheck = false;
    int counter = 0;
    while (!errorCheck) {
        for (int i = 0; i < size; ++i) {
            double sum = 0;
            for (int j = 0; j < size; ++j) {
                if (j != i) {
                    sum += matrix[i][j] * S[j];
                }
            }
            OS[i] = S[i];  //The old value is the actual value
            S[i] = (solutions[i] - sum) / matrix[i][i];  //The actual value changes to the substitution of the variable

            E[i] = (S[i] - OS[i]);  //Error iterativo absoluto
            //E[i] = (S[i]-OS[i])/S[i];  // Error iterativo relativo
            //E[i] = ((S[i]-OS[i])/S[i])*100;  // Error iterativo porcentual
        }
        for (int i = 0; i < size; ++i)
        {
            if(abs(E[i]) > Epsilon){
                break;
            }
            errorCheck = true;
        }
        counter++;
    }
    for (int i = 0; i < size; ++i)
    {
        cout  << " a" << i << ": " << S[i] << endl;
    }
    cout << "it took " << counter << " times to terminate" << endl;
    //-------------------Aqui termina GaussSeidel------------------------
    printf("------------------POLINOMIO----------------\n");
    for (int i = size-1; i >= 1; --i)
    {
        cout << S[i] << "X^"<< i << " + ";
    }
    cout << S[0] << endl;

    printf("------------------ERRORES------------------\n");
    double e2[n]; //Calcula el error para cada X
    for (int i = 0; i < n; ++i)
    {
        double Ycalculada = 0;
        for (int j = size-1; j >= 1; --j)
        {
            Ycalculada += S[j]*(pow(x[i], j));
        }
        Ycalculada += S[0];
        e2[i] = pow(y[i]-Ycalculada,2);
    }
    double sr = 0;
    double sumY = solutions[0];
    double st = 0;
    for (int i = 0; i < n; ++i)
    {
        sr += e2[i];
        st += pow(y[i] - (sumY/n),2);
    }
    cout << "Sr: " << sr << endl;

    double sxy = sqrt(sr/(n-size));
    cout << "St: " << st << endl;

    double r2 = (st-sr)/st;
    double r = sqrt(r2);
    cout << "Error Estandar (Sx/y): " << sxy << endl;
    cout << "Coeficiente de determinacion (r2): " << r2 << endl;
    cout << "Coeficiente de correlacion (r): " << r << endl;
    cout << endl;
}
// Termina Regresion Polinomial

/*Las siguientes 2 funciones son necesarias para la funcion qsort().
 Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/
 Ordena el arreglo de puntos acordes a la cordenada x*/
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
    double sum = 0;
    double add = 1;

    static  unsigned long pre_time;
    static unsigned long pre_secs;
    struct timespec ts;
    unsigned long long tiempo;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    pre_time = ts.tv_nsec;
    pre_secs = ts.tv_sec;

    int iterations = 1000*1000*1000;
    for (int i = 0; i < iterations; ++i) {
        sum += add;
        add /= 2.0;
    }

    Point P[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
    double x[] = {2,12,40,5,12,3};
    double y[] = {3,30,50,1,10,4};
    int n = sizeof(P) / sizeof(P[0]);
    int dataNum = 6;

    regresionPoli(x,y,dataNum);
    printf("La distancia mas pequenia es: %f ", closest(P, n));

    clock_gettime(CLOCK_MONOTONIC, &ts);
    tiempo = (ts.tv_sec - pre_secs) * 1000000000 + ts.tv_nsec - pre_time;
    printf("\nTiempo medido: %llu", tiempo);

    return 0;
}

