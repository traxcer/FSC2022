#define _POSIX_SOURCE

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <signal.h>



//definicion de estados
#define esatado_IDLE 0
#define estado_WAITING 1
#define estado_CONECCTED 2

//definiciond e eventos
#define evento_connet_request 1
#define evento_connet_confirm 2
#define evento_reset 3
#define evento_timeout 4

int p[2];

//manejadores de la signal
void m1(int signo){
    int evento= evento_connet_request;
    write(p[1], &evento, 4);
    signal(SIGUSR1, m1);
}

void m2( int signo){
    int evento= evento_timeout;
    write(p[1], &evento, 4);
    signal(SIGALRM, m2);
}

void m3(int signo){
    int evento= evento_connet_confirm;
    write(p[1], &evento, 4);
    signal(SIGUSR2, m3);
}




//definicion de funciones auxiliares
int espera_eventos(int pipe, int fifo);
void copia(fd_set* fds, fd_set* fds_copia, int max_mas1);

int main(int argc, char* argv[]){

    int r;
    int estado = esatado_IDLE;
    int evento;
    int fd_fifo;

    struct itimerval temporizador;
    struct timeval inicio;
    struct timeval intervalo;

    inicio.tv_sec= 2;
    inicio.tv_usec= 0;
    intervalo.tv_sec= 0;
    intervalo.tv_usec= 0;

    temporizador.it_value= inicio;
    temporizador.it_interval= intervalo;

    


    if((signal(SIGUSR1, m1))== SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }

    if(signal(SIGALRM, m2)== SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }

    if((signal(SIGUSR2, m3)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }

    r= pipe(p);
    if(r < 0){
        perror("Error al crear la pipe");
        exit(1);
    }

    fd_fifo= open("fsc_maquina", O_RDONLY);
    if(fd_fifo < 0){
        perror("Error al abir la fifo");
        exit(1);
    }

    int pid= getpid();
    printf("El pid es: %d \n",pid);

    while(1){

        evento= espera_eventos(p[0], fd_fifo);

        if(evento < 0){
            perror("error en el espera eventos");
            close(p[1]);
            close(p[0]);
            close(fd_fifo);
            exit(1);
        }

        switch (estado){
            case esatado_IDLE:
                switch (evento){
                    case evento_connet_request:
                        r= setitimer(ITIMER_REAL, &temporizador, NULL);
                        if( r< 0){
                            if(errno == EINTR){
                                errno= 0;
                                continue;
                            }
                            else{
                                perror("Error en el setitimer");
                                exit(1);
                            }
                        }
                        estado= estado_WAITING;
                        printf("Estado IDLE --> estado WAITING\n");
                        break;   

                    default:
                        break;
                }
                
                break;
            
            case estado_WAITING:
                switch (evento){
                    case evento_timeout:
                        estado= esatado_IDLE;
                        printf("Estado WAITING --> estado IDLE\n");           
                        break;

                    case evento_connet_confirm:
                        estado= estado_CONECCTED;
                        printf("Estado WAITING--> Estado CONNECTED\n");
                        break;
                    
                    default:
                        break;
                }

                break;
            
            case estado_CONECCTED:
                switch (evento){
                    case evento_reset:
                        estado= esatado_IDLE;
                        printf("de coneccted --> IDLE\n");
                        break;
                    
                    default:
                        break;
                }
                break;
            
            default:
                printf("Estado no esperado\n");
                break;

        }//fin maquina de estados

    }//fin while

    return 0;
}


//implementacion de funciones

int espera_eventos(int pipe, int fifo){

    int evento_recibido;
    fd_set fds, fds_copia;
    int maximo;
    int r;
    char caracter;
    int leidos;

    FD_ZERO(&fds);
    FD_SET(pipe, &fds);
    maximo= pipe;

    FD_SET(fifo, &fds);
    if(fifo > maximo)
        maximo = fifo;
    
    copia(&fds, &fds_copia, maximo+1);
    printf("----\n");

    
    r= select(maximo+1, &fds_copia, NULL, NULL, NULL);
    if(r < 0){
        if(errno == EINTR){
            errno= 0;
            select(maximo+1, &fds_copia, NULL, NULL, NULL);
        }
        else
            return -1;
                  
    }

    if( FD_ISSET(fifo, &fds_copia)){
        leidos= read(fifo, &caracter, 1);
        printf("Leidos %d bytes \n", leidos);
        if(leidos < 0){
            return -1;
        }
        else
            return evento_reset;
    }

    if( FD_ISSET(pipe, &fds_copia) ){
            printf("Se le\n");
        leidos= read(pipe, &evento_recibido, sizeof(evento_recibido)) ;
        printf("Leidos %d bytes \n", leidos);
        if(leidos < 0){
            return -1;
        }
        else{
            return evento_recibido;
    }   }

  return 0;
}


void copia(fd_set* fds, fd_set* fds_copia, int max_mas1){

    FD_ZERO(fds_copia);
    for(int i=0; i< max_mas1; i++){
        if(FD_ISSET(i, fds))
            FD_SET(i, fds_copia);
    }
}