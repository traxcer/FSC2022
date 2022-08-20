#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Pruebas con open / read / write
int main(){
    int fd;
    char fichero[]="ejemplo.dat";
    if((fd=open(fichero,O_WRONLY|O_TRUNC|O_CREAT, 0666))<0){
        perror("open");
        exit(-1);
    }
    printf("Se abrío el fichero: %s correctamente\n",fichero);
    if ((close (fd))<0){
        perror("close");
        exit(-1);
    }
    printf("Se cerró el fichero: %s correctamente\n", fichero);
}
