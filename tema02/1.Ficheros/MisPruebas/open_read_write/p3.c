#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// Pruebas con open / read / write
int main(){
    int fd_read;
    int fd_write;
    int fd_readwrite;
    int fd_append;
    char fichero[]="/etc/passwd";
    if((fd_read=open(fichero,O_RDONLY))<0){
        perror("open");
        exit(-1);
    }
    printf("Se abrío el fichero: %s correctamente\n",fichero);
    if ((close (fd_read))<0){
        perror("close");
        exit(-1);
    }
    printf("Se cerró el fichero: %s correctamente\n", fichero);
    memcpy(fichero,"run.log"+'\0',8);
    fd_write=open(fichero,O_WRONLY|O_TRUNC);
    if (fd_write<0){
        perror("open run.log");
        exit(-1);
    }
    printf("Se abrío el fichero: %s correctamente\n",fichero);
    if ((close (fd_write))<0){
        perror("close");
        exit(-1);
    }

}
