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

//variables globales
int p[2];
uint8_t ev;

//Cabecera de las funciones y manejadores
int espera_evento();
ssize_t read_n(int fd, void* b, ssize_t n);

//Manejadores de segniales
void m1(int signo){
    ev= evento_open;
    write(p[1], &ev, 1);
    signal(SIGTERM, m1);
}
void m2(int signo){
    ev= evento_timeout;
    write(p[1], &ev, 1);
    signal(SIGALRM, m2);
}
void m3(int signo){
    ev= evento_syn;
    write(p[1], &ev, 1);
    signal(SIGINT,  m3);
}
void m4(int signo){
    ev= evento_ack;
    write(p[1], &ev, 1);
    signal(SIGUSR1, m4);
}
void m5(int signo){
    ev= evento_fin;
    write(p[1], &ev, 1);
    signal(SIGUSR2, m5);
}


int main(int argc, char* argv[]){

    //Definiendo las segniales
    if( (signal(SIGTERM, m1)) == SIG_ERR){
        perror("Error en el manejador");
        exit(1);
    }
    if( (signal(SIGALRM, m2)) == SIG_ERR){
        perror("Error en el manejador");
        exit(1);
    }
    if( (signal(SIGINT,  m3)) == SIG_ERR){
        perror("Error en el manejador");
        exit(1);
    }
    if( (signal(SIGUSR1, m4)) == SIG_ERR){
        perror("Error en el manejador");
        exit(1);
    }
    if( (signal(SIGUSR2, m5)) == SIG_ERR){
        perror("Error en el manejador");
        exit(1);
    }

    if( (pipe(p)) < 0 ){
         perror("Error al crear la  pipe");
         exit(1);
     }

     struct itimerval temporizador;
     struct timeval inicio, intervalo;
    
    intervalo.tv_sec= 0;
    intervalo.tv_usec= 0;
    temporizador.it_interval= intervalo;


    int evento;
    int estado= estado_closed;
    int pid= getpid();
    int r;
    printf("El PID es: %d\n", pid);
     

    while(1){

        evento= espera_evento();

        switch (estado){

        case estado_closed:
            switch (evento){
                case evento_open:
                    inicio.tv_sec= 5;
                    inicio.tv_usec= 300;
                    temporizador.it_value= inicio;

                    r= setitimer(ITIMER_REAL, &temporizador, NULL);
                    if(r < 0){
                        perror("Error en el temporizador");
                        exit(1);
                    }
                    printf("CLOSED--> LISTEN\n");
                    estado= estado_listen;
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
                    inicio.tv_sec= 0;
                    inicio.tv_usec= 0;
                    temporizador.it_value= inicio;

                    r= setitimer(ITIMER_REAL, &temporizador, NULL);
                    if(r < 0){
                        perror("Error en el temporizador");
                        exit(1);
                    }

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

int espera_evento( ){
    uint8_t evto;
    int evto_int;
    int leidos= read_n(p[0], &evto, sizeof(evto));
    if(leidos < 0){
        perror("Error en la lectura de la pipe");
        exit(1);
    }
    evto_int= evto;
    return evto_int;
}

ssize_t read_n( int fd, void* b, ssize_t n){

    size_t a_leer= n;
    ssize_t leidos;
    ssize_t total_leidos= 0;

    do{
        errno= 0;
        leidos= read(fd, b+ total_leidos, a_leer);
        if(leidos >= 0){
            total_leidos += leidos;
            a_leer       -= leidos;
        }
    }
    while( ((leidos > 0) && (total_leidos < n)) || (errno == EINTR) );

    if(leidos > 0)
        return total_leidos;
    else
        return leidos;
}
