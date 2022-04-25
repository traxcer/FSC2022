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

#define T 100
#define BLOQUE 25
#define PUERTO 4950
#define MAX_CONEXIONES 10
#define MAX_FICHEROS 15


int main(int argc, char* argv[]){

    //Variables del programa
    int sd, sd_cliente, fd;
    struct sockaddr_in dirServer;
    struct sockaddr_in dirCliente;
    socklen_t long_liente= sizeof(dirCliente);
    int r, escritos, recibidos;
    char buffer[T];
    char nombre[T];
    int desplazamiento= atoi(argv[1]);
    uint16_t cabeceraBigEndian;
    int cabecera;

    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error al hacer el socket");
        exit(1);
    }

    if(argc < 2){
        perror("Numeros de argumentos insuficiente");
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
    if( r< 0){
        perror("Error en el listen");
        exit(1);
    }

  

    for(int i=0; i< MAX_FICHEROS; i++){
        printf("Esperando conexion entrante...\n");
        sd_cliente= accept(sd, (struct sockaddr*) &dirCliente, &long_liente);
        if(sd_cliente < 0){
            perror("Error  al aceptar un cliente");
            exit(1);
        }
        else
            printf("Conexion aceptada con exito!\n");
        
        r= sprintf(nombre, "file%d.dat", i);
        if(r< 0){
            perror("Error en el printf");
            exit(1);
        }

        fd= creat(nombre, 0600);
        if(fd < 0){
            perror("Error al crear el archivo");
            exit(1);
        }


        do{
            recibidos= read(sd_cliente, &cabeceraBigEndian, sizeof(cabeceraBigEndian));
            if(recibidos < 0){
                perror("Error al recibir la cabecera");
                exit(1);
            }

            cabecera= ntohs(cabeceraBigEndian);
            recibidos= read(sd_cliente, buffer, cabecera);
            if(recibidos != cabecera){
                perror("Error al leer el bloque");
                exit(1);
            }

            //Decodificacion
            for(int j=0; j<recibidos; j++){
                buffer[j]= buffer[j] - desplazamiento;
            }

            escritos= write(fd, buffer, recibidos);
            if(escritos != recibidos){
                perror("Error al escribir en el archibo");
                exit(1);
            }

        }
        while(cabecera > 0);

        if( (close(fd)) < 0){
            perror("Error al cerrar el archivo");
            exit(1);
        }

    }
    

    return 0;
}