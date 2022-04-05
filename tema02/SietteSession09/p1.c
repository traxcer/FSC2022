/*
Implementar un programa en C que reciba como argumento dos nombres de fichero, uno de origen y otro
de destino, y copie el contenido del primero en el segundo. 
La particularidad de este ejercicio es que vamos a asumir que el sistema de ficheros es muy lento y 
sólo permitirá el envío de bloques de T bytes (T = 8) bajo demanda. 
Intervienen, por tanto, dos agentes en la copia: nuestro programa, que es el encargado de leer el 
fichero de origen y escribir el destino, y el sistema de ficheros", que simularemos nosotros, que es 
el que sabe cuando se puede recibir el siguiente bloque de información. 
Una vez que el sistema está preparado, envía una señal a nuestro programa, que inicia una espera de S 
segundos antes de la escritura en disco. El protocolo de copia detallado sería:
El programa abre los ficheros origen y destino, lee el primer bloque de información de origen y espera
a que el sistema de ficheros solicite la primera copia de datos.
En algún instante posterior, el sistema de ficheros (nosotros) envía una señal SIGUSR1 al proceso de 
copiaTemporizada, que indica que, en S segundos, estará disponible para aceptar un bloque de información en 
el fichero destino. Para aligerar las pruebas, asegurad que S = 1 .
Mientras transcurren esos S segundos, el sistema de ficheros, debido a condiciones de carrera, puede 
enviar varias señales SIGUSR1 que deberán ser ignoradas ya que sólo está preparado para procesar un bloque de datos.
Una vez realizada la copia en destino, el proceso copiaTemporizada vuelve a esperar a que llegue SIGUSR1 
para proceder con el siguiente bloque de datos.
El proceso continua hasta que el fichero se copia completamente.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#define _POSIX_SOURCE
#define T 8
#define S 1

void alarma(int signo){
    printf("A saltado la ALARMA para escribir\n");
    signo++;
}

void m(int signo){//Manejadora señal SIGUSR1
    signo++;
    printf("Ha llegado la señal SIGUSR\n");
    signal(SIGUSR1,SIG_IGN);
    signal(SIGALRM,alarma);
}

int main(int argc, char *argv[]){
    int fdr, fdw, leido, escrito;
    char buffer[T];

    if (argc < 2){
        printf("Usar: %s <origen> <destino>\n",argv[0]);
        return 0;
    }
    printf("Mi pid:%d\n",getpid());
    if ((fdr= open(argv[1],O_RDONLY))<0){ //Error en la apertura del fichero origen
        perror("open lectura");
        exit (-1);
    }
    if ((fdw= open(argv[2],O_WRONLY | O_TRUNC | O_CREAT, 0664))<0){ //Error en la apertura del fichero destino
        perror("open escritura");
        exit (-1);
    }
    while((leido=read(fdr,buffer,T))>0){ //Lee datos
        if (signal(SIGUSR1,m) == SIG_ERR) { //Captura de señal
            perror("SEÑAL");
            exit(-1);
            }
        printf("==> Leido (%d) bytes: %s\n",leido,buffer);
        pause(); //espera la señal SIGUSR1
        alarm(S);
        pause(); //espera la alarma
        if ((escrito=write(fdw,buffer,leido))<0){
            perror("write");
            exit(-1);
        }
        printf("<== Escrito (%d) bytes: %s\n",escrito,buffer);
    }
    close(fdr);
    close(fdw);
}