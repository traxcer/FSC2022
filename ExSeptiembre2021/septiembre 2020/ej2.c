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


//Variables globales
int ha_llegado_SIGUSR1= 1;


//Cabeceras de las funciones
int fsc_cron(char* c, int s);
ssize_t read_n(int fd, void* b, size_t n);
size_t write_n(int fd, void* b, size_t n);


//Manejadores de la signal
void m1(int signo){

    ha_llegado_SIGUSR1= 0;
    signal(SIGUSR1, m1);
}

void m2(int signo){

    signal(SIGALRM, m2);
}

void m3(int signo){

    wait(0);
    signal(SIGCHLD, m3);
}



int main(int argc, char* argv[]){

    int r;

    if ( (signal(SIGUSR1, m1)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }
    if ( (signal(SIGALRM, m2)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }
    if ( (signal(SIGCHLD, m3)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }


    r= fsc_cron( "ls", 2);
    if( r < 0){
        perror("Error al ejecutar la funcion");
        exit(1);
    }


    return 0;
}



//Implementacion de funciones

int fsc_cron(char* c, int s){

    pid_t pid;
    int r;

    struct itimerval temporizador;
    struct timeval inicio, intervalo;

    inicio.tv_sec= s;
    inicio.tv_usec= 0;
    intervalo.tv_sec= 0;
    intervalo.tv_usec= 0;

    temporizador.it_value= inicio;
    temporizador.it_interval= intervalo;

    int pid_padre;
    pid_padre= getpid();
    printf("El PID del padre es: %d\n", pid_padre);

    while(ha_llegado_SIGUSR1){

        r= setitimer(ITIMER_REAL, &temporizador, NULL);
        if( r< 0){
            printf("Error al en e temporizador\n");
            return -1;
        }

        pause();

        pid= fork();
        if(pid < 0){
            printf("Error al engendrar el hijo\n");
            return -1;
        }
        //CODIGO DEL HIJO
        else if(pid == 0){
            
            r= execl("/bin/bash", "sh","-c", c, NULL);
            printf(" 1\n");
            if( r < 0){
                printf("Error al mutar el programa\n");
                return -1;
            }
        }
        //CODIGO DEL PADRE
        else{
            wait(0);
        }

        if( ha_llegado_SIGUSR1 == 0){
            printf("Programa finalizado con exito\n");
        }

    }

    return 0;

}