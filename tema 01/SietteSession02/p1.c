/* Escriba un programa en C implemente la siguiente función:
#define T 5
struct PDU {
   size_t length;
   int * payload;
};
size_t memoria(struct PDU a[T]);
que devuelve el tamaño que ocupa en memoria, en bytes, un array de PDUs (Protocol Data Units o mensajes de un protocolo), 
que se le pasa como argumento. Implemente también el main() necesario para probar su funcionamiento. 
A la hora de realizar el ejercicio, ¿es necesario inicializar la estructura para saber su tamaño en memoria?
Prueba 1: Compilación
Prueba de funcionamiento: A continuación se muestran algunos ejemplos del funcionamiento de la función:
#define T 5
 siendo a = [
                 a[0] = {length = 18, payload = [7,0,8,6,8,5,0,1,8,2,9,7,9,7,5,9,5,9]},
                 a[1] = {length = 14, payload = [6,3,9,4,1,4,0,5,2,4,7,1,1,9]},
                 a[2] = {length = 2, payload = [9,9]},
                 a[3] = {length = 7, payload = [1,0,7,3,1,6,5]},
                 a[4] = {length = 19, payload = [3,4,5,2,9,2,7,8,6,0,2,6,6,6,1,3,9,4,5]}
               ];
size_t t = memoria(a);
// se espera que t = 280;
Las pruebas se realizan mediante la invocación de la función memoria(), que deben tener el prototipo indicado o, en otro caso, 
habrá un error de compilación.
*/
#include <stdio.h>
#include <stdlib.h>

#define T 5

typedef struct PDU {
   size_t length;
   int * payload;
}pdu;

/*
   size_t memoria(struct PDU a){
   }
*/

int main(){
   // Defino el Array de Estructuras PDU con (T) elementos
   pdu* a=(pdu*) malloc (T*sizeof(pdu)); //defino y asigno memoria al array y apunto el puntero

   //asigno valores
   a[0].length=18;
   a[0].payload=(int*)malloc(a[0].length*sizeof(int)*a[0].length); //Asigno un puntero con espacio para almacenar el payload
   a[0].payload[]= {7,0,8,6,8,5,0,1,8,2,9,7,9,7,5,9,5,9};

  printf("\nPrueba: a[0].length=%ld\n",a[0].length);
  printf("Prueba: a[1].length=%ld\n",a[1].length);

}