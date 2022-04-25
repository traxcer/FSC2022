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

#define PUERTO 4950
#define T 512


int main(int argc, char* argv[]){

    if(argc < 2){
        perror("Argumentos insuficientes");
        exit(1);
    }

    int sd;
    struct sockaddr_in dirServer;
    char buffer[T];
    int ok= 1;
    uint16_t cabecera_bigEndian;
    int cabecera, leidos, enviados, recibidos;
    int r;


    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error en el socket");
        exit(1);
    }

    uint32_t dir= inet_addr( argv[1] );
    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family      = AF_INET;
        dirServer.sin_port        = htons(PUERTO);
        memcpy(&dirServer.sin_addr, &dir, 4);
    
    r= connect(sd,(struct sockaddr*) &dirServer, sizeof(dirServer));
    if(r < 0){
        perror("Error en el connect");
        exit(1);
    }

    while(ok){

        printf("Escriba una cadena a codificar: \n");
        leidos= read(0, buffer, T);
        buffer[leidos-1]= '\0';

        if( !(strcmp("fin", buffer)) ){
            ok= 0;
            printf("Saliendo del rograma...\n");
            if ((close(sd) ) < 0){
                perror("Errror al cerrar el sd");
                exit(1);
            }
            exit(0);
        }

        cabecera= strlen(buffer);
        cabecera_bigEndian= htons(cabecera);

        enviados= write(sd, &cabecera_bigEndian, sizeof(cabecera_bigEndian));
        if(enviados < 0){
            perror("Error al enviar la cabecera al server");
                exit(1);
            
        }
        printf("Enviados %d Bytes\n", enviados);

        enviados= write(sd, buffer, cabecera);
        if(enviados < 0){
            perror("Error al enviar la cadena al server");
                exit(1);
            
        }
        printf("Enviados %d Bytes\n", enviados);

        recibidos= read(sd, &cabecera_bigEndian, sizeof(cabecera_bigEndian));
        if(recibidos < 0){
            perror("Error al recibir la cabecera del server");
                exit(1);
            
        }
        printf("Recibidos %d Bytes\n", recibidos);
        cabecera= ntohs(cabecera_bigEndian);

        recibidos= read(sd, buffer, cabecera);
        if(recibidos < 0){
            perror("Error al recibir la cadena del server");
                exit(1);
            
        } 
        printf("Recibidos %d Bytes\n", recibidos);

        write(1, buffer, cabecera);
        printf("\n");


    }


    return 0;

}