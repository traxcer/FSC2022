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
#define MAX_CONEXIONES 10
#define T 512


int main(int argc, char* argv[]){

    if(argc < 2){
        perror("Argumentos insuficientes");
        exit(1);
    }

    int sd, sd_cliente;
    int recibidos, enviados, r;
    struct sockaddr_in dirServer;
    struct sockaddr_in dirCliente;
    socklen_t long_dirCliente= sizeof(long_dirCliente);
    uint16_t cabecera_bigEndian;
    int cabecera;
    char buffer[T];
    int ok;


    int desplazamiento= atoi(argv[1]);

    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error en el socket");
        exit(1);
    }

    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family      = AF_INET;
        dirServer.sin_port        = htons(PUERTO);
        dirServer.sin_addr.s_addr = INADDR_ANY; 

    r= bind(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
    if(r < 0){
        perror("Error en el bind");
        exit(1);
    }

    r= listen(sd, MAX_CONEXIONES);
    if(r < 0){
        perror("Error en el listen");
        exit(1);
    }

    
    while(1){

        printf("Esperando conexion entrante...\n");

        sd_cliente= accept(sd, (struct sockaddr*) &dirCliente, &long_dirCliente);
        if(sd_cliente <0){
            perror("Error al aceptar el cliente");
            exit(1);
        }
        printf("Conexion realizada con exito\n");


        ok= 1;
        while(ok){

            //Recibir la cabecera
            recibidos= read(sd_cliente, &cabecera_bigEndian, sizeof(cabecera_bigEndian) );
            if(recibidos < 0){
                perror("Error al recibir el desplazamiento");
                exit(1);
            }
            printf("Recibidos %d Bytes\n", recibidos);

            if(recibidos == 0){
                ok= 0;
                close(sd_cliente);
                break;
            }
            cabecera= ntohs(cabecera_bigEndian);

            //Recibe la cadna de caracteres
            recibidos= read(sd_cliente, buffer, cabecera );
            if(recibidos < 0){
                perror("Error al recibir el desplazamiento");
                exit(1);
            }
            printf("Recibidos %d Bytes\n", recibidos);

            buffer[recibidos]= '\0';

            for(int i=0; i< cabecera; i++){
                buffer[i]= buffer[i] + desplazamiento;
            }
            strcat(buffer, "123");

            cabecera= strlen(buffer);
            cabecera_bigEndian= htons(cabecera);

            //Envio de la cabecera al cliente
            enviados= write(sd_cliente, &cabecera_bigEndian, sizeof(cabecera_bigEndian));
            if(enviados < 0){
                perror("Error en el envio de la cabecera");
                exit(1);
            }
            printf("Enviados %d Bytes\n", enviados);

            //Envio de la cadena al cliente
            enviados= write(sd_cliente, buffer, cabecera);
            if(enviados < 0){
                perror("Error al enviar la cadena de caracteres");
                exit(1);
            }
            printf("Enviados %d Bytes\n", enviados);
        }//While de conexion con cliente.


    }//While(1) del servidor


    return 0;
}