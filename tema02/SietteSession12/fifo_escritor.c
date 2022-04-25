#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define T 512

int main(){
    int fd= open("tuberia_fsc", O_WRONLY);
    if (fd<0){
        perror("open");
        exit(-1);
    }
    printf("Tras espererar al lector escribo escribo\n");
    char datos[T]="Esto es una prueba total\n";
    size_t bytes_a_escribir= strlen(datos);
    int escritos=write(fd,datos,bytes_a_escribir);
    if (escritos<bytes_a_escribir){
        perror("error en write");
        exit(-1);
    }
    if (close(fd)<0){
        perror("error en close");
        exit(-1);
    }
    return 0;
}