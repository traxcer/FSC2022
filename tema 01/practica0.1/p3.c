/* 
3. Asigne el puntero char *p a la primera posición de la cadena anterior.
   ¿Cómo se hace?
*/
#include <stdio.h>

int main(){
    char cadena[256]="Hola Mundo";
    char *p;
    //p= *cadena; //Error asigna a p el contenido de la primera posición de la cadena
    //p=cadena[]; //Error corchetes vacios
    //p=cadena[0]; //Error asigna al puntero el contenido de cadena en la posición 0.
    //p=&cadena; //Error, asigna a p una dirección de un puntero
    p=cadena;
    printf("e:%s\n",p);
    p=&cadena[0];
    printf("f:%s\n",p);
    //p=*cadena[0]; //Error, el contenido de elcontenido cadena[0]
    return 0;
}
