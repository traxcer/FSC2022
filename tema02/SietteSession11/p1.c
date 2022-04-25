#define _POSIX_SOURCE
/*
Implemente un programa en C que reciba dos argumentos de entrada: 
un número de segundos, s, y un un comando de Linux, cmd. 
Dicho programa ejecutará el comando cmd durante un máximo de s segundos. 
Si transcurrido ese tiempo cmd no ha finalizado, entonces lo mata enviándole 
SIGKILL.
El programa simulará a la función de biblioteca system(), que crea un hijo 
que mutará (usando alguna función de la familia exec()), para ejecutar una 
shell con /bin/bash -c cmd.

Nota: El uso de la función kill(pid_t pid, int signo) saca un warning, 
independientemente de las librerías incluidas. Hay dos formas de solucionarlo: 
compilar sin -std=c99, que es lo que se hace en Siette, o bien incluir el 
siguiente #define _POSIX_SOURCE al principio del código.
Prueba 1: Compilación
Prueba de funcionamiento
Algunas ejecuciones de ejemplo serían:
              >./timeout 2 "ls -l" 
que muestra el contenido del directorio actual en formato largo. 
Dado que ese comando tardará menos de 2 segundos, se muestra completamente. 
Sin embargo, la llamada
              >./timeout 1 "sleep 10" 
espera, durante 1 segundo, en lugar de dormirse durante 10. 
Pruebe a ejecutar "sleep 10" sin llamar a timeout para comprobar que, 
efectivamente, tarda de 10 segundos.
*/

#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>


int matoproceso= 0;

void manejadorAlarma(int sig){
    signal(sig,SIG_DFL);
    matoproceso=1;
}

int main (int argc, char *argv[]){
    int pid_id;

    if (argc !=3){
        printf("Usar (%d): %s <segundos> <comando>\n", argc,argv[0]);
        exit (-1);
    }
    if ((pid_id=fork())<0){ //Error al crear Hijo
        perror("fork");
        exit (-1);
    }
    if ((pid_id=fork())==0){ //Hijo
        if((execl ("/bin/sh",argv[2],NULL))<0){
            perror("execl");
            exit (-1);
    }
    
    }
    //padre
        //Activo temporizador
        struct itimerval temporizador;
        struct timeval tiempoini;
        struct timeval tiemporepe;
        tiempoini.tv_sec=atoi(argv[1]);
        tiempoini.tv_usec=0;
        tiemporepe.tv_sec=0;
        tiemporepe.tv_usec=0;
        temporizador.it_value=tiempoini;
        temporizador.it_interval=tiemporepe;
        signal(SIGALRM,manejadorAlarma); //armo la alarma
        setitimer(ITIMER_REAL,&temporizador,NULL); //activo el temporizador
        while (matoproceso==0);
        kill(pid_id, 9);
        wait(0);
}