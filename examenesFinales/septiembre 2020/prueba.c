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

int ha_llegado_sig1= 0;
int ha_llegado_sig2= 0;

void m1(int signo){

    ha_llegado_sig1= 1;
    signal(SIGUSR1, m1);
}

void m2(int signo){

    ha_llegado_sig2= 1;
    signal(SIGUSR2, m2);
}




int main(){

    if( (signal(SIGUSR1, m1)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }
    if( (signal(SIGUSR2, m2)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }

    int pid= getpid();
    printf("El PID es: %d\n", pid);

    while(1){

        pause();

        if(ha_llegado_sig1){
            printf("Ha llehado SIGUSR1\n");
            ha_llegado_sig1= 0;
        }

        if(ha_llegado_sig2){
            printf("Ha llehado SIGUSR2\n");
            ha_llegado_sig2= 0;
        }
    }

    return 0;
}