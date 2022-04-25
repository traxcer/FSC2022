#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define T 32

int fin = 0;

void m(int signo) {
    write(1,"Catched!\n", 9);
    sleep(3);
    fin = 1;
    signal(signo, m);
}

void espera_hijos(int signo) {
    write(1,"Hijo terminado!\n", 16);
    wait(0);
    signal(signo, espera_hijos);
}


int main() {


    char b[T];
    int leidos;
    int numHijos = 0;

    if (signal(SIGCHLD, espera_hijos) == SIG_ERR) {
        perror("signal");
        exit(1);
    } //*/


    while (1) {
        printf("Esperando a cliente:\n");
        while ((leidos = read(0, b, T-1)) < 0) { //read es bloqueante, se queda aqui
            if (errno != EINTR) {
                perror("read");
                exit(1);
            }
        }
        b[leidos -1] = '\0';

        pid_t pid = fork();//crea el hijo
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            //código del hijo
            if (signal(SIGUSR1, m) == SIG_ERR) {
                perror("signal");
                exit(1);
            }
            
            while(fin == 0) {
                printf("Hijo %d (pid = %d) atendiendo al cliente con mensaje: %s\n", numHijos+1, getpid(), b);
                sleep(2);
            }
            exit(0);
        } else {
            numHijos++;
            //wait(0);
        }

    }


    return 0;
}