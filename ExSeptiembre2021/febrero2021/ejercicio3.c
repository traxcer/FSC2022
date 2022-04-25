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
char c[30];

int ha_llegado_sigchld= 0;

void m1(int signo){
    ha_llegado_sigchld= 1;
    signal(SIGCHLD, m1);
}


// estados
#define estado_idle 0
#define estado_ejecutando 1
#define estado_esperando 2

//eventos de la maquina de estados
#define evento_stop 0
#define evento_cont 1
#define evento_fin 2
#define evento_comando 3
#define evento_fin_ejec 4

//Cabeceras de las funciones
ssize_t read_n(int fd, char* b, size_t n);
ssize_t write_n(int fd, char* b, size_t n);
int espera_eventos(int fd);


int main(int argc, char* argv[]){

    int estado;
    int evento;
    int r, fd;
    int pid;

    if( (signal(SIGCHLD, m1)) == SIG_ERR){
        perror("Error al registar el manejador se signal");
        exit(1);
    }

    fd= open("fifo_fsc", O_RDONLY);
    if(fd < 0){
        perror("Error al abir la fifo");
        exit(1);
    }
    else
        printf("Fifo abierta correctamente\n");


    evento= evento_fin;
    estado= estado_idle;

    while(1){

        switch (estado){
        case estado_idle:
            switch (evento){
                case evento_fin:
                  
                    break;
                case evento_comando:
                    estado= estado_ejecutando;
                    printf("IDLE --> EJECUTANDO\n");
                    break;
                
                default:
                break;
            }
            break;

        case estado_ejecutando:
            switch (evento){
            case evento_comando:

                pid= fork();
                if(pid < 0){
                    perror("Error al crear el hijo");
                    exit(1);
                }
                //CODIGO DEL HIJO
                else if(pid == 0){
                    r= execl("/bin/bash", "sh","-c", c, NULL);
                    if(r< 0){
                        perror("Error  en la mutacion del programa");
                        exit(1);
                    }



                }
                //CODIGO DEL PADRE
                else{

                   // if(ha_llegado_sigchld){
                        wait(0);
                        estado= estado_idle;
                        evento= evento_fin;
                        ha_llegado_sigchld= 0;
                        printf("EJECUTANDO --> IDLE\n");
                    

                }   
                break;


            case evento_stop:
                estado= estado_esperando;
                printf("EJECUTANDO --> ESPERANDO\n");
                break;
            
            default:
                break;
            }
            break;

        case estado_esperando:
            switch (evento){
                case evento_stop:
                    kill(pid, SIGSTOP);
                    break;

                case evento_cont:
                    kill(pid, SIGCONT);
                    estado= estado_ejecutando;
                    printf("ESPERANDO --> EJECUTANDO\n");
                    break;

                case evento_fin:
                    kill(pid, SIGKILL);
                    estado= estado_idle;
                    printf("ESPERANDO --> IDLE\n"); 

                default:
                    break;
            }
            break;
        
        default:
            break;
        }

        evento= espera_eventos(fd);

    }



    return 0;
}

//definicion de las funciones

int espera_eventos(int fd){

    int leidos;
    //char buffer[T];
    
    leidos= read(fd, c, T);
    if(leidos < 0){
        perror("Error al leer de la fifo");
        return -1;
    }
    c[leidos-1]= '\0';

    if( !(strcmp("/stop", c)) )
        return evento_stop;
    
    else if( !(strcmp("/cont", c)) )
        return evento_cont;
    
    else if( !(strcmp("/fin", c)) )
        return evento_fin;
    
    else{
       // memcpy(c, buffer, leidos);
        return evento_comando;
    }

}

ssize_t read_n(int fd, char* b, size_t n){

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
    while(((leidos > 0) && (total_leidos < n)) || (errno == EINTR));

    if(leidos > 0)
        return total_leidos;
    else
        return leidos;
}

ssize_t write_n(int fd, char* b, size_t n){

    size_t a_escribir= n;
    ssize_t escritos;
    ssize_t total_escritos= 0;

    do{
        errno= 0;
        escritos= write(fd, b+total_escritos, a_escribir);
        if(escritos > 0){
            total_escritos += escritos;
            a_escribir     -= escritos;
        }
    }
    while(((escritos > 0) && (total_escritos < n)) || (errno == EINTR));

    if(escritos > 0)
        return total_escritos;
    else
        return escritos;
}