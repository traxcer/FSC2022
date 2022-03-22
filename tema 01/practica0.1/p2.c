// 2. ¿Cómo se accede al segundo elemento de una cadena definida como char cadena[256]?

#include <stdio.h>

int main(){
    char cadena[256]="Hola Mundo";
    //printf("a:%c\n",cadena[]+2);  ERROR, corchetes vacios
    printf("b:%c\n",cadena[1]);   //Perfect
    printf("c:%c\n",*(cadena+1)); //Perfect
    printf("d:%c\n",*(cadena+2)); // ERROR, se refiere al 3er Elemento
    printf("c:%c\n",*cadena+1);   // ERROR, al contenido de cadena le suma 1 (HI<- muestra I)
    return 0;
}