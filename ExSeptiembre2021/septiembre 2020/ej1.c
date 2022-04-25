#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>


#define T 100


//Funciones auxiliares
ssize_t write_n(int fd, void* b, size_t n);
ssize_t read_n(int fd, void* b, ssize_t n);
void copiar(fd_set* conjunto, fd_set* copia, int maxfd_mas_1);
int maximo(int nuevo, int maximo);

int main(int argc, char* argv[]){

    int r;
    char buffer[T];
    int f1, f2;
    fd_set fds, fds_copia;
    int max_fd, lectura, salir;
    int leidos;

    lectura= 1;
    salir= 1;

    f1= open("/tmp/f1", O_RDONLY);
    if(f1 < 0){
        perror("Error al abrir la fifo 1");
        exit(1);
    }

    f2= open("/tmp/f2", O_RDONLY);
    printf(" sdsdsdsdsd 1!\n");
    
    if(f2 < 0){
        perror("Error al abrir la fifo 2");
        exit(1);
    }

    printf("Las fifos se han abierto con exito!\n");

    FD_ZERO( &fds);
    FD_SET( 0, &fds);
    max_fd= 0;

    FD_SET(f1, &fds);
    max_fd= maximo(f1, max_fd);

    FD_SET(f2, &fds);
    max_fd= maximo(f2, max_fd);

    while(salir){

        copiar(&fds, &fds_copia, max_fd+1);
        r= select(max_fd+1, &fds_copia, NULL, NULL, NULL);
        if(r < 0){
            perror("Error en el select");
            exit(1);
        }

        if(FD_ISSET(0, &fds_copia)){
            leidos= read(0, buffer, T);
            if(leidos < 0){
                perror("Error al leer de pantalla");
                exit(1);
            }
            buffer[leidos-1]= '\0';

            if( !(strcmp("/on", buffer)) ){
                lectura= 1;
                printf("Lectura activada\n");
            }
            else if( !(strcmp("/off", buffer)) ){
                lectura= 0;
                printf("Lectura desactivada\n");
            }
            else if( !(strcmp("/exit", buffer))){
                salir= 0;
                printf("Saliendo del programa...\n");
            }
            else{
                write(1, buffer, leidos);
                printf("\n");
            }
        }

        if( (FD_ISSET(f1, &fds_copia)) &&  (lectura) ){
            leidos= read(f1, buffer, T);
            if(leidos < 0){
                perror("Error al leer de la fifo f1");
                exit(1);
            }
            write_n(1, buffer, leidos);
            printf("\n");

        }

        if( (FD_ISSET(f2, &fds_copia)) &&  (lectura) ){
            leidos= read(f2, buffer, T);
            if(leidos < 0){
                perror("Error al leer de la fifo f2");
                exit(1);
            }
            write(1, buffer, leidos);
            printf("\n");
        }

    }

    if( (close(f1)) < 0){
        perror("Error al cerrar la fifo1");
        exit(1);
    }
        if( (close(f2)) < 0){
        perror("Error al cerrar la fifo1");
        exit(1);
    }

    FD_ZERO( &fds);
    printf("Programa finalizado con exito!\n");


    return 0;
}



//Implementacion de funciones
ssize_t write_n(int fd, void* b, size_t n){
    
    size_t a_escribir= n;
    ssize_t escritos;
    ssize_t total_escritos= 0;
    do{
        errno= 0;
        escritos= write(fd, b+total_escritos, a_escribir);
        if(escritos > 0){
            total_escritos += escritos;
            a_escribir -= escritos;
        }
    }
    while(((escritos > 0) && (total_escritos < n) ) || (errno== EINTR));

    if(escritos > 0)
        return total_escritos;
    else
        return escritos;

}

ssize_t read_n(int fd, void* b, ssize_t n){

    size_t a_leer= n;
    ssize_t leidos;
    ssize_t total_leidos= 0;

    do{
        errno= 0;
        leidos= read(fd, b+total_leidos, a_leer);
        if(leidos > 0){
            total_leidos += leidos;
            a_leer       -= leidos;
        }
    }
    while( ((leidos > 0) && (total_leidos < n)) || (errno == EINTR));

    if(leidos > 0)
        return total_leidos; 
    else   
        return leidos;
}

void copiar(fd_set* conjunto, fd_set* copia, int maxfd_mas_1){

    FD_ZERO(copia);
    for(int i=0; i<  maxfd_mas_1; i++){
        if(FD_ISSET(i, conjunto) ){
            FD_SET(i, copia);
        }
    }
}


int maximo(int nuevo, int maximo){
    if(nuevo > maximo)
        return nuevo;
    else
        return maximo;
}