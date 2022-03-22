/*
Siette Sesión1 p1
Escriba un programa en C implemente la siguiente función:

   #define T 21
   int sumaMenores(int n, int a[T], int size); 

que devuelve la suma de los n menores valores del array a que se le pasa como argumento. 
Este array sólo debe contener enteros no negativos. En caso de que n sea mayor que size 
o que a almacene algún número negativo, la función devuelve -1. Implemente también el 
programa main necesario para probar su funcionamiento.

Algoritmo que voy a usar:

Localizo los n menores elementos
    bucle
        localizo el menor
    bucle

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h> // Usa el reloj actual como semilla

#define T 12

void ordenaArray(int a[],int size);

int sumaMenores (int n, int a[],int size){
    ordenaArray(a,size);
    int suma=0; //inicializo suma
    if (n>size){
        printf("\nn=%i>size=%i",n,size);       
        return -1;
    }
    for (int i=0;i<size;i++){ //recorro array
            if (a[i]<0) //si elemento negativo devuelve -1
                return -1;
            if (i<n) //solo suma los n primeros
                suma+=a[i];
        }
    return suma; //devuelvo la suma
}

void ordenaArray(int a[],int size){
    int temp=0;
    for (int i=0;i<size;i++){
        for (int j=i+1;j<size;j++){
            if (a[i]>a[j]){
                temp= a[i];
                a[i]= a[j];
                a[j]= temp;
            }
        }
    }
}

int main(){
    srand( (unsigned)time( NULL ) );// Inicializar número aleatorio
    int i, a[T];
    printf( "Array Inicial:\n");
    for (i=0;i<T;i++){
        a[i]=rand() % 255;
        printf("%i ",a[i]);
    }
    printf( "\n-----------------------\n");
    ordenaArray(a,T);
    printf( "\nArray Ordenado:\n");
   for (i=0;i<T;i++){
        printf("%i ",a[i]);
    }
   printf( "\n-----------------------\n");
   
   int prueba=rand() % T;
   printf("\nLa suma de los %i primeros es %i.\n",prueba, sumaMenores(prueba,a,T));
}