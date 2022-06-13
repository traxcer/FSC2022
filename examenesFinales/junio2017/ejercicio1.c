#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



#define T 128

//Cabecera de las funciones
int espera_evento(int fd);

//Definicion de estados
#define estado_SS 1
#define estado_CA 2

//Definicion de eventos
#define evento_ACK 0
#define evento_DUPACK 1

int main(int argc, char* argv[]){

    int fd;

    //vlores de la maquina
    double cwnd   = 1.0;
    double umbral = 4.0;
    int dupACKS   = 0;

    int estado= estado_SS;
    int evento;

    fd= open("congestionTCP", O_RDONLY);
    if(fd < 0){
        perror("Error al abrir la fifo");
        exit(1);
    }


    while(1){

        evento= espera_evento(fd);

        switch (estado){
            case estado_SS:
                switch (evento){
                    case evento_ACK:
                        cwnd = cwnd + 1.0;
                        dupACKS = 0;

                        if(cwnd >= umbral){
                            estado= estado_CA;
                            printf("estado SS --> estado CA\n");
                        }                      
                        break;

                    case evento_DUPACK:
                        dupACKS ++;
                        if(dupACKS >= 3){
                            cwnd   = 1.0;
                            dupACKS= 0;
                        }
                        break;

                    default:
                        printf("Evento no esperado\n");
                        if ( (close(fd)) < 0){
                            perror("Error al cerrar el fd");
                            exit(1);
                        }
                        printf("Maquina finalizada con exito\n");
                        exit(0);
                        break;
                }
                break;
            
            case estado_CA:
                switch (evento){
                    case evento_ACK:
                        cwnd = cwnd + (1.0/cwnd);
                        dupACKS = 0;
                        break;
                        
                    case evento_DUPACK:
                        dupACKS ++;
                        if(dupACKS >= 3){
                            cwnd    = 1.0;
                            dupACKS = 0;
                            umbral  = (cwnd/2.0);
                            estado= estado_SS;
                            printf("estado CA --> estado SS\n");
                        }
                        break;

                    default:
                        printf("Evento no esperado\n");
                        if ( (close(fd)) < 0){
                            perror("Error al cerrar el fd");
                            exit(1);
                        }
                        printf("Maquina finalizada con exito\n");
                        exit(0);
                        break;
                }
                break;
            
            default:
                break;

        }//fin maquina

        
    }//fin del while

    return 0;
}


//Implementacuion de las funciones
int espera_evento(int fd){

    char buffer[T];
    int leidos=

    leidos= read(fd, buffer, T);
    if(leidos < 0){
        perror("Error la leer por la pipe");
        exit(1);
    }

    buffer[leidos-1]= '\0';
    printf("Evento recibido: %s\n", buffer);

    if( !(strcmp("ACK", buffer)))
        return evento_ACK;
    
    else if( !(strcmp("DCK", buffer)))
        return evento_DUPACK;
    
    else
        return -1;

}