#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


int main (int argc, char* argv[]){
    if (argc < 2){
        printf("Usar: %s <puerto>\n",argv[0]);
        return 0; //no es un error
    }
    
    //VARIABLES
    int fd, fd2,puerto;
    puerto=atoi(argv[1]);
    printf("Arrancando el servidor...\n");

    //ESTRUCTURAS SERVIDOR Y CLIENTE
    struct sockaddr_in server;
    struct sockaddr_in client;
    printf("Declaradas estructuras; sockaddr_in server y sockaddr_in cliente\n");

    //CONFICURACION SERVIDOR
    server.sin_family=AF_INET;
    server.sin_port=htons(puerto);
    server.sin_addr.s_addr = htons(INADDR_ANY); //cualquier cliente puede conectar
    bzero(&(server.sin_zero),0); //rellenamos con 0.
    printf("Configuración del Servidor: Asigno Puerto %i y acepto conexión de INADDR_ANY\n",puerto);

    //DEFINICION DEL SOCKET
    if((fd=socket(AF_INET,SOCK_STREAM,0))< 0){
        perror("Error de apertura del socket\n");
        exit(-1);
    }
    printf("Defino el socket=%i\n",fd);

    //AVISAMOS AL SISTEMA DEL SOCKET
    if (bind(fd,(struct sockaddr *)&server, sizeof(server))==-1){
        perror("Error en bind\n");
        exit(-1);
    }

    //ESTABLECER SOCKET EN MODO ESCUCHA
    if( listen(fd,5)==-1){
        perror("Error en listen\n");
        exit(-1);
    }

    //ESPERANDO CONEXIONES
    printf("Esperando conexiones...\n");
    
    while(1){ //bucle sin fin
        if((fd2 = accept(fd,(struct sockaddr *)&client,NULL))==-1){
            perror("Accept\n");
            exit(-1);
        }
        send(fd2,"Bienvenido al servidor.\n",24,0); //envia al cliente conectado
        close(fd2);
    }
    close(fd);
return 0;
}