#define _POSIX_SOURCE_ 
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


//Estados
#define estado_esperando 0
#define estado_fich1 1
#define estado_fich2 2

//Eventos
#define evento1 1
#define evento2 2
#define evento_fin 0

//variables globales
int ha_llegado_SIGUSR1= 0;
int ha_llegado_SIGUSR2= 0;
int ha_llegado_SIGINT = 0;  


//Manejadores de la señal
void m1(int signo){

    ha_llegado_SIGUSR1= 1;
    signal(SIGUSR1, m1);
}

void m2(int signo){

    ha_llegado_SIGUSR2= 1;
    signal(SIGUSR2, m2);
}

void m3(int signo){

    ha_llegado_SIGINT= 1;
    signal(SIGINT, m3);
}

//Funciones aufiliares
int espera_eventos(int fd){
    char b;
    int leidos= read(fd, &b,1);
    if(leidos < 0){
        perror("Error al leer de la pipe");
        return -1;
    }

    return atoi(&b);
}





int main(int argc, char* argv[]){

    //variables
    pid_t pid_hijo;
    int p[2];

    //pipe
    if ( (pipe(p) ) < 0 ){
        perror("Error al crear la pipe");
        exit(1);
    }


    //Manejadores de signals
    if( (signal(SIGUSR1, m1)) < 0){
        perror("Error alllegar la signal");
        exit(1);
    }

    if( (signal(SIGUSR2, m2)) < 0){
        perror("Error alllegar la signal");
        exit(1);
    }

    if( (signal(SIGINT, m3)) < 0){
        perror("Error alllegar la signal");
        exit(1);
    }


    pid_hijo= fork();
    if(pid_hijo < 0){ //Errores
        perror("Error el engendrar el hijo");
        exit(1);
    }

    //CODIGO DEL HIJO
    else if(pid_hijo  > 0){ 

        int estado= 0;
        int evento;

        if( (close(p[1])) < 0){
            perror("Error la cerrar el extremo del hijo");
            exit(1);
        }

        //Máquina de estados
        while(1){

            switch (estado){
            case estado_esperando:
                switch (evento){
                case evento1:
                    printf("cambio de espera -> fich1\n");
                    estado= estado_fich1;
                    break;
                
                default:
                    break;
                }
                
                break;
            
            case estado_fich1:
                switch (evento){
                case evento2:
                    printf("Cambiando de fich1 -> fich2\n");
                    estado= estado_fich2;
                    break;
                
                default:
                    break;
                }

                break;
            
            case estado_fich2:
                switch (evento){
                case evento_fin:
                    printf("Fin de la maquina de estados\n");
                    close(p[0]);
                    exit(0);
                    break;
                
                default:
                    break;
                }
            
            default:
                break;
            }

            evento= espera_eventos(p[0]);

        }//Fin de la maquina de estado



    }

    //CODIGO DEL PADRE
    else{ 
        int escritos;
        int pid_padre= getpid();
        printf("El PID del padre es: %d \n", pid_padre); 

        if( (close(p[0])) < 0){
            perror("Error la cerrar el extremo del PADRE");
            exit(1);
        }

        while(1){
            pause();

            if(ha_llegado_SIGUSR1){
                escritos= write(p[1], "1", 1);
                if(escritos != 1){
                    perror("Error al escribir de la pipe");
                    close(p[1]);
                    exit(1);
                }
                ha_llegado_SIGUSR1= 0;
            }

            if(ha_llegado_SIGUSR2){
                escritos= write(p[1], "2", 1);
                if(escritos != 1){
                    perror("Error al escribir en la pipe");
                    close(p[1]);
                    exit(1);
                }
                ha_llegado_SIGUSR2= 0;
            }

            if(ha_llegado_SIGINT){
                escritos= write(p[1], "0", 1);
                if(escritos != 1){
                    perror("Error al escribir en la pipe");
                    close(p[1]);
                    exit(1);
                }
                ha_llegado_SIGINT= 0;
                wait(0);
                printf("Fin del programa de ejecucion del padre\n");
                exit(0);
            }


        }


    }










    return 0;
}