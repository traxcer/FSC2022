/* Primera prueba con signal */

#define _POSIX_SOURCE //para evitar warning
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char* argv[]){

    if (argc<2){
        printf("Utilidad que envia una señal SIGTERM al proceso <PID>\n");
        //SIGTERM: El S.O. solicita que el proceso termine
        printf("Uso: %s <PID>\n",argv[0]);
        exit (1);
    }
    pid_t pid = atoi (argv[1]); //pasa a entero el argumento pasado
    if ( (kill (pid,SIGTERM)<0)){
        perror("kill");
        exit (-1);
    }
    return 0;
}