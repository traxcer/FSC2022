/* Pruebas de la práctica 0.1 */

// 1. Declaraciones correctas

#include <stdio.h>

int main(){
char cadena_a[]="Hola";
char cadena_b[5]={'H','o','l','a','\0'};
char cadena_c[4]="Hola"; //ERROR, asigna 4 posiciones de memoria y consume 5 +\0 (Se lo come)
char cadena_d[512]="Hola\0"; // Es correcta, desperdicia memoria pero trunca la cadena hasta el \0.
char *cadena_e="Hola"; //Es correcto, es un "string literal".
//char cadena_f='Hola'; //ERROR, pone comillas simples ademas que "cadena" es un caracter. (violación de segmento)

printf("a:%s\n",cadena_a);
printf("b:%s\n",cadena_b);
printf("c:%s\n",cadena_c);
printf("d:%s\n",cadena_d);
printf("e:%s\n",cadena_e);
//printf("f:%s\n",cadena_f);

return 0;
}