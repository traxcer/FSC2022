#define _POSIX_SOURCE

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
#include <string.h> 

#define T 100
#define PUERTO 2119

//Cabecera de las funciones
void copia(fd_set* fds, fd_set* fds_copia, int max_mas_uno);
int max(int nuevo, int actual);

int main(int argc, char* argv[]){

    int r, enviados, recibidos, leidos;
    int sd;
    char buffer[T];
    struct sockaddr_in dirServer;
    fd_set fds, fds_copia;
    int maximo, seg, mic;
    uint8_t s;
    uint32_t ms;

    FD_ZERO(&fds);
    FD_SET(0, &fds);
    maximo= 0;


    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("error en el socket");
        exit(1);
    }

    FD_SET(sd, &fds);
    maximo= max(sd, maximo);

    uint32_t dir= inet_addr(argv[1]);
    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family  = AF_INET;
        dirServer.sin_port    = htons(PUERTO);
        memcpy(&dirServer.sin_addr, &dir, 4);

    r= connect(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
    if(r < 0){
        perror("Error en el connect");
        exit(1);
    }

    while(1){

        copia(&fds, &fds_copia, maximo+1);
        r= select(maximo+1, &fds_copia, NULL, NULL, NULL);
        if(r < 0){
            perror("Error en el select");
            exit(1);
        }

        if(FD_ISSET(0, &fds_copia)){
            leidos= scanf("%d", &seg);
            if(leidos < 0){
                perror("Error en la lectura por teclado de los segundos");
                exit(1);
            }
            printf("Introduce ahora los microsegundos\n");
            leidos= scanf("%d", &mic);
            if(leidos < 0){
                perror("Error en la lectura por teclado de los segundos");
                exit(1);
            }

            s= seg;
            ms= htonl(mic);

            if((seg== 0) && (mic == 0)){
                printf("Se va a proceder al cierre del socket....\n");
                if( (close(sd)) < 0){
                    perror("Error en el cierre de la conexion");
                    exit(1);
                }
                printf("Socket cerrado con exito!\n");
                exit(0);

            }

            enviados= write(sd, &s, sizeof(s));
            if( enviados < 0){
                perror("Error al enviar los segundos");
                exit(1);
            }
            printf("enviados %d bytes\n", enviados);
            enviados= write(sd, &ms, sizeof(ms));
            if( enviados < 0){
                perror("Error al enviar los microsegundos");
                exit(1);
            }
            printf("enviados %d bytres\n", enviados);
            
        }

        if( FD_ISSET(sd, &fds_copia)){
            printf("ff\n");
            recibidos= read(sd, buffer, T);
            if(recibidos < 0){
                perror("Error en la lectura");
                exit(1);
            }

            write(0, buffer, recibidos);
            printf("\n");
        }
    }
    
    return 0;
}

//Implementacion de funciones

void copia(fd_set* fds, fd_set* fds_copia, int max_mas_uno){
    FD_ZERO(fds_copia);
    for(int i=0; i< max_mas_uno; i++){
        if(FD_ISSET(i, fds)){
            FD_SET(i, fds_copia);
        }
    }
}

int max(int nuevo, int actual){
    if(nuevo > actual)
        return nuevo;
    else    
        return actual;
}