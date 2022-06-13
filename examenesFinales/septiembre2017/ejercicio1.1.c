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

//definicion de estados
#define estado_closed 0
#define estado_listen 1
#define estado_sync 2
#define estado_estab 3

//definicion de eventos
#define evento_timeout 0
#define evento_open 1
#define evento_syn 2
#define evento_ack 3
#define evento_fin 4

//Cabecera de las funciones
int espera_evento(int fd);


int main(int argc, char* argv[]){

    int evento= evento_open;
    int estado= estado_closed;

    while(1){
        switch (estado){

        case estado_closed:
            switch (evento){
                case evento_open:
                    printf("CLOSED--> LISTEN\n");
                    break;
                
                default:
                    break;
            }
            break;


        case estado_listen:
            switch (evento){
                case evento_timeout:
                    printf("LISTEN--> CLOSED\n");
                    estado= estado_closed;
                    break;
                
                case evento_syn:
                    printf("LISTEN--> SYNC\n");
                    estado= estado_sync;
                    break;
                
                default:
                    break;
            }

            break;


        case estado_sync:
            switch (evento){
                case evento_ack:
                    printf("SYNC--> ESTABILISHED\n");
                    estado= estado_estab;
                    break;
                
                default:
                    break;
            }
            break;


        case estado_estab:
            switch(evento){
                case evento_fin:
                    estado= estado_closed;
                    printf("ESTABILISHED--> CLOSED\n");
                    break;
                
                default:
                    break;

            }
            break;


        default:
            printf("Estado no esperado\n");
            break;
        }//Fin del switch

    }//Fin del while(1)


    return 0;
}


//Implementacion de las funciones

int espera_evento(int fd){

    int ok= 1;


    return ok;
}