/* 
Desarrolle un programa que lea de teclado una cierta cantidad de segundos y 
muestre su equivalente en semanas, días, horas, minutos y segundos, según el 
siguiente formato

9127145 segundos equivalen a [ 15] semanas, 0 dias, 15:19:05 

2178585 segundos equivalen a [ 3] semanas, 4 dias, 05:09:45 
*/
#include <stdio.h>

int main(){
    int segundos, horas, minutos;
    int dias=0;
    int semanas=0;
    int resto=0;

    printf("Introduce un cierto numero de segundos:");
    scanf("%d", &segundos);
    if (segundos<0){
        printf("Se espera un número mayor o igual a cero\n");
        return 0;
    }
    semanas=(segundos/(3600*7*24));
    resto=(segundos % (3600*7*24));
    dias=(resto/(3600*24));
    resto=(resto % (3600*24));
    horas=(resto/3600);
    resto= segundos % 3600;
    minutos=(resto / 60);
    resto = resto % 60;

    printf("%d segundos equivalen a [ %d] semanas, %0d dias, %02d:%02d:%02d\n",segundos,semanas,dias, horas,minutos, resto);
    return 0;
}