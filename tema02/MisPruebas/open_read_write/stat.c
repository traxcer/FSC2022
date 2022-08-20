/*
PRUEBA del uso de STAT

Málaga a 20 de Agosto 2022
Eloy Gutiérrez
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char* argv[]){
    if (argc < 2){
        printf("Usar: %s nombre_fichero\n", argv[0]);
        exit (1);
    }
    printf("Muestra del Uso de stat()\n");
    printf("Fichero a analizar: %s\n", argv[1]);

    struct stat estado;

    if( (stat(argv[1], &estado))<0 ){
        perror("stat");
        exit(1);
    }

    if (S_ISDIR(estado.st_mode))
        printf("Es un directorio\n");
    if (S_ISREG(estado.st_mode))
        printf("Es un fichero regular\n");
    
    printf("ID del Dispositivo: %li\n",estado.st_dev);
    printf("ID del Usuario Propietario: %i\n",estado.st_uid);
    printf("Tamaño del fichero en Bytes: %li\n",estado.st_size);
    return 0;
}