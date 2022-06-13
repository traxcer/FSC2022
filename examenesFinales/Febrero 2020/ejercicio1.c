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

#define T 100

//Variables globales
int ha_llegado_sigusr1= 0;
int ha_llegado_sigint= 0;
int ha_llegado_datos= 0;
int ha_llegado_sigterm= 0;

//Estados
#define estado_idle 1
#define estado_ciph1 2
#define estado_ciph2 3


//Eventos
#define evento_sigusr1 0
#define evento_sigint 1
#define evento_datos 2
#define evento_sigterm 3

//Manejadores de señal
void m1(int signo){
    ha_llegado_sigusr1= 1;
    signal(SIGUSR1, m1);
}
void m2(int signo){
    ha_llegado_sigterm= 1;
    signal(SIGTERM, m2);
}
void m3( int signo){
    ha_llegado_sigint= 1;
    signal(SIGINT, m3);
}

void m4(int signo){
    wait(0);
    signal(SIGCHLD, m4);
}

//Cabeceras de las funcioens
int maximo(int nuevo, int viejo);
void copiar(fd_set fds, fd_set fds_copia, int max_mas_1);
ssize_t write_n(int fd,void* b, size_t n);
ssize_t read_n(int fd, void* b, size_t n);



int main(int argc, char* argv[]){

    //Variables del programa
    int p[2];
    int fifo;
    int r, estado, evento;
    int pid, enviados, recibidos;
    char buffer[T];

    r= pipe(p);
    if(p < 0){
        perror("Error al crear la pipe");
        exit(1);
    }

    fifo= open("fifo_fsc", O_RDONLY);
    if(fifo < 0){
        perror("Error al abrir la fifo");
        exit(1);
    }

    //Señales
    if( (signal(SIGUSR1, m1)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }
    if( (signal(SIGTERM, m2)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }
    if( (signal(SIGINT, m3)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }
    if( (signal(SIGCHLD, m4)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }

    int pid_padre= getpid();
    printf("El PID del padre es: %d\n", pid_padre);

    pid= fork();
    if(pid < 0){
        perror("Error al crear el hijo");
        exit(1);
    }

    //CODIGO DEL HIJO
    else if(pid == 0){
        if( (close(p[0])) < 0){
            perror("Error al cerrar la pipe");
            exit(1);
        }

        while(1){

            if(ha_llegado_sigusr1){
                enviados= write(p[1], "0", 1);
                if(enviados != 1){
                    perror("Error al mandar por la pipe");
                    exit(1);
                }
                else{
                    printf("Se han escrito %d bytes en la pipe\n", enviados);
                }
                ha_llegado_sigusr1= 0;
            }
            if(ha_llegado_sigint){
                enviados= write(p[1], "1", 1);
                if(enviados != 1){
                    perror("Error al mandar por la pipe");
                    exit(1);
                }
                else{
                    printf("Se han escrito %d bytes en la pipe\n", enviados);
                }
                ha_llegado_sigint= 0;
            }
            if(ha_llegado_sigterm){
                enviados= write(p[1], "3", 1); 
                if(enviados != 1){
                    perror("Error al mandar por la pipe");
                    exit(1);
                }
                else{
                    printf("Se han escrito %d bytes en la pipe\n", enviados);
                }
                ha_llegado_sigusr1= 0;
            }
            if(ha_llegado_datos){
                enviados= write(p[1], "2", 1); 
                if(enviados != 1){
                    perror("Error al mandar por la pipe");
                    exit(1);
                }
                else{
                    printf("Se han escrito %d bytes en la pipe\n", enviados);
                }
                ha_llegado_datos= 0;
            }
        }
    }

    //CODIGO DEL PADRE
    else{
        fd_set fds, fds_copia;
        int max;
        char b;

        if( (close(p[1])) < 0){
            perror("Error al cerrar la pipe");
            exit(1);
        }

        FD_ZERO(&fds);
        FD_SET(p[0], &fds);
        max= p[0];

        FD_SET(fifo, &fds);
        max= maximo(fifo, max);
        estado= estado_idle;
        evento= 5;


        //Máquina de estados
        while(1){
            

            switch(estado){
                case estado_idle:
                    switch (evento){
                        case evento_sigusr1:
                            estado= estado_ciph1;
                            printf("IDLE --> CIPH1\n");
                            break;
                        
                        case evento_sigterm:
                            exit(0);
                            break;

                        case evento_datos:
                            write(1, buffer, T);
                            printf("Holi\n");
                            break;
                        
                        case evento_sigint:
                            printf("IDLE --> IDLE\n");
                        
                        default:
                            break;
                    } 
                    break;

                case estado_ciph1:
                    switch (evento){
                        case evento_sigint:
                            estado= estado_idle;
                            printf("CIPH1 --> IDLE\n");
                            break;

                        case evento_sigusr1:
                            estado= estado_ciph2;
                            printf("CIPH1 --> CIPH2\n");
                            break;
                        
                        case evento_sigterm:
                            exit(0);
                            break;
                        
                        case evento_datos:
                            write(1, buffer, T);
                            printf("Holi\n");

                            break;
                        
                        default:
                            break;
                        
                    
                    }
                    break;

                case estado_ciph2:
                    switch (evento){
                        case evento_sigint:
                            estado= estado_idle;
                            printf("CIPH2 --> IDLE\n");
                            break;

                        case evento_sigusr1:
                            estado= estado_ciph1;
                            printf("CIPH2 --> CIPH1\n");
                            break;
                        
                        case evento_sigterm:
                            exit(0);
                            break;
                        
                        case evento_datos:
                            write(1, buffer, T);
                            printf("Holi\n");

                            break;
                        
                        default:
                            break;
                    }
                    break;
                
                default:
                    break;
            }//Fin de la maquina de estados

        
        //selector-----------------------------------------
        copiar(fds, fds_copia, max+1);
        pid_padre= getpid();
        printf("El PID del padre es: %d\n", pid_padre);

        r= select(max+1, &fds_copia, NULL, NULL, NULL);
        if(r < 0){
            if(errno == EINTR){
                errno= 0;
            }
            else{
                perror("Error en el select");
                exit(1);
            }
            
        }

        if( FD_ISSET(p[0], &fds_copia) ){
            recibidos= read_n(p[0], &b, 1);
            if(recibidos != 1){
                perror("Error al leer de la pipe");
                exit(1);
            }
            int num= atoi(&b);

            if( num == 0 ){
                evento= evento_sigusr1;
            }
            if( num == 1 ){
                evento= evento_sigint;
            }
            if( num == 2 ){
                evento= evento_datos;
            } 
            if( num == 3 ){
                evento= evento_sigterm;
            }
            
                 
                
        }

        if( FD_ISSET(fifo, &fds_copia) ){
            recibidos= read_n(fifo, buffer, T);
            buffer[recibidos]= '\0';
            if(recibidos != 1){
                perror("Error al leer de la fifo");
                exit(1);
            }
            if(recibidos == 0) //Cierre del canal
                evento= evento_sigterm;
            else
                evento= evento_datos;
        }  
        //-------------------------------------------------
        }//Fin del while(1)

    }//fin del padre





    return 0;
}//Fin main


//---------------------Implementacion de las funciones---------------------------------

ssize_t write_n(int fd, void* b, size_t n){

    size_t a_escribir= n;
    ssize_t escritos;
    ssize_t total_escritos= 0;

    do{
        errno = 0;
        escritos= write(fd, b+total_escritos, a_escribir);
        if(escritos > 0){
            total_escritos += escritos;
            a_escribir     -= escritos;
        }
    }
    while( ((escritos > 0) && (total_escritos < n)) || (errno == EINTR) );

    if(escritos > 0)
        return total_escritos;
    else
        return escritos;
}

ssize_t read_n(int fd, void* b, size_t n){

    size_t a_leer= n;
    ssize_t leidos;
    ssize_t total_leidos= 0;

    do{
        errno = 0;
        leidos= read(fd, b+total_leidos, a_leer);
        if(leidos >0){
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

int maximo(int nuevo, int viejo){
    if(nuevo > viejo)
        return nuevo;
    else
        return viejo;
}

void copiar(fd_set fds, fd_set fds_copia, int max_mas_1){

    FD_ZERO(&fds_copia);
    for(int i=0; i< max_mas_1; i++){
        if(FD_ISSET(i, &fds) ){
            FD_SET(i, &fds_copia);
        }
    }
}