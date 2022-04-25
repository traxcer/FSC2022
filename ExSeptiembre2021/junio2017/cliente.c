#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#define PUERTO 4950
#define T 512




int main( int argc, char* argv[]){ 

    if(argc < 2){
        perror("NUMERO DE ARGUMENTOS INSUFICIENTE");
        exit(1);
    }

    //Variables del programa
    int sd;
    struct sockaddr_in dirServer;
    char buffer[T];
    int recibidos, enviados, leidos, r;
    int cabecera;
    uint16_t cabeceraBigEndian;


    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error en el socket");
        exit(1);
    }


    uint32_t dir= inet_addr(argv[1]);
    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family      = AF_INET;
        dirServer.sin_port        = htons(PUERTO);
        memcpy(&dirServer.sin_addr, &dir, 4);
    
    r= connect(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
    if(r < 0){
        perror("Error en el connect");
        exit(1);
    }

    while(1){

        printf("Introduce una ip para el dns server:\n");
        leidos= read(0, buffer, T);

        buffer[leidos-1]= '\0';
        if( !(strcmp("fin", buffer))){
            printf("Fin del programa\n");
            close(sd);
            exit(0);
        }

        cabecera= strlen(buffer);
        cabeceraBigEndian= htons(cabecera);

        enviados= write(sd, &cabeceraBigEndian, sizeof(cabeceraBigEndian));
        if( enviados < 0){
            perror("Error al enviar la cabecera");
            exit(1);
        }

        enviados= write(sd, buffer, cabecera);
        if(enviados != cabecera){
            perror("Error al mandar la direccion ip");
            exit(1);
        }

        recibidos= read(sd, &cabeceraBigEndian, sizeof(cabeceraBigEndian));
        if( recibidos < 0){
            perror("Error al leer la cabecera");
            exit(1);
        }
        cabecera= ntohs(cabeceraBigEndian);

        recibidos= read(sd, buffer, cabecera);
        if(recibidos != cabecera){
            perror("Error al  leer el nombre trascrito");
            exit(1);
        }

        buffer[recibidos]= '\0';

        if( !(strcmp( "0.0.0.0", buffer)) ){
            printf("Nombre de dominio desconocido\n");
        }
        else{
            printf("Dominio: %s \n", buffer);
        }        
    }



    return 0;

}