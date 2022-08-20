#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Pruebas con open / read / write
int main(){
    int fd = open("data.dat",O_RDONLY);
    if (fd<0){
        perror("Error abriendo fichero");
        exit (-1);
    }
}
