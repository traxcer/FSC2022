/*
Escriba una función en C que tome como argumento un nombre de fichero, 
y tres enteros pasados por dirección como argumentos de salida, y cuente 
el número de caracteres en minúscula, en mayúscula y dígitos que aparecen 
en el fichero. La función debe tener el siguiente prototipo:

       void contar(char * filename, int * minusculas, int * mayusculas, int * digitos);

Implemente también el programa main necesario para probar su funcionamiento.
Prueba 1: Compilación
Prueba de funcionamiento
Dada un fichero regular en disco llamado "datos.txt" con el siguiente contenido:
           aAbBcCdD1233.;-fgh
la función tenga devuelva los siguientes valores:
           //llamada a la función
           int minusculas, mayusculas, digitos;
          contar("datos.txt", &minusculas, &mayusculas, &digitos);
           //se espera que minusculas = 7; mayusculas = 4; digitos = 4;
Nótese que este programa no espera salida alguna, y que las comprobaciones se realizarán con llamadas a función correspondiente, que deben tener el prototipo indicado.
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//Prototipos de funciones
void contar(char * filename, int * minusculas, int * mayusculas, int * digitos);

int main(int argc,char * argv[]){
    if (argc<2){
        printf("Debe usar %s nombfichero",argv[0]);
        return 0;
    }
    int minusculas, mayusculas, digitos;
    contar("datos.txt", &minusculas, &mayusculas, &digitos);
    printf("Minusculas: %d", minusculas);
}
    


void contar(char * filename, int * minusculas, int * mayusculas, int * digitos){
    char buf[255];
    int fd; 
    ssize_t leido;

    if((fd=open(filename,O_RDONLY))<0){
        perror("open");
        exit(-1);
    }
    do{
        leido = read(fd,buf,sizeof(buf));
    }while(leido==0);
    for (int i=0;i<255;i++){
        if (buf[i]>='a' && buf[i]<='z') *minusculas=*minusculas+1;
    }

}