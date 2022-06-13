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
#define PUERTO 2119
#define MAX_CONEXIONES 1

//Cabecera de las funciones
ssize_t write_n(int fd, void* b, size_t n);
ssize_t read_n(int fd, void* b, size_t n);


int main(int argc, char* argv[]){

    int sd, sd_cliente, r, ack;
    int leidos, enviados;
    struct sockaddr_in dirCliente;
    struct sockaddr_in dirServer;
    socklen_t longitudDirCliente= sizeof(dirCliente);
    uint32_t pid_bigEndian;
    uint8_t  signal_bigEndian;

    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error al crear el socket");
        exit(-1);
    }

    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family        = AF_INET;
        dirServer.sin_port          =  htons(PUERTO);
        dirServer.sin_addr.s_addr   = INADDR_ANY; 
    
    r= bind(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
    if( r < 0){
        perror("Error en el bind");
        exit(1);
    }

    r= listen(sd, MAX_CONEXIONES);
    if( r < 0){
        perror("Error en el listen");
        exit(1);
    }

    int pid;
    int senial;

    while(1){

        printf("Esperando conexion entrante...\n");

        sd_cliente= accept(sd, (struct sockaddr*) &dirCliente, &longitudDirCliente );
         if(r < 0){
            perror("Error en el accept");
            exit(1);
        }
        printf("Conexion exitosa con el cliente\n");

        leidos= read(sd_cliente, &pid_bigEndian, sizeof(pid_bigEndian));
        if(leidos != sizeof(pid_bigEndian)){
            perror("Error al leer el pid");
            exit(1);
        }
        printf("Leidos %d Bytes\n", leidos);

        leidos= read(sd_cliente, &signal_bigEndian, sizeof(signal_bigEndian));
        if(leidos != sizeof(signal_bigEndian)){
            perror("Error al leer el codigo de la signal");
            exit(1);
        }
        printf("Leidos %d Bytes\n", leidos);


        pid= htonl(pid_bigEndian);
        senial= signal_bigEndian;
        uint32_t retorno_bigEndian;

        ack= kill(pid, senial);
        retorno_bigEndian= htonl(ack);

        enviados= write(sd_cliente, &retorno_bigEndian, sizeof(retorno_bigEndian));
        if(enviados < 0){
            perror("Error al mandar el ack");
            exit(1);
        }
        printf("Enviados %d Bytes\n", enviados);

        if( (close(sd_cliente)) < 0){
            perror("Error al cerrar el socket");
            exit(1);
        }
        
    
    }


    return 0;
}

ssize_t write_n(int fd, void* b, size_t n){

    size_t a_escribir= n;
    ssize_t escritos;
    ssize_t total_escritos= 0;

    do{
        errno= 0;
        escritos= write(fd, b+total_escritos, a_escribir);
        if(escritos > 0){
            total_escritos += escritos;
            a_escribir     -= escritos;
        }
    }
    while(( (escritos > 0)  && (total_escritos < n) ) || (errno== EINTR));

    if(escritos > 0)
        return total_escritos;
    else
        return escritos;
}

ssize_t read_n(int fd, void* b, size_t n){

    size_t a_leer= n;
    ssize_t leidos;
    ssize_t total_leidos= 0;

    do{
        errno= 0;
        leidos= read(fd, b+total_leidos, a_leer);
        if(leidos > 0){
            total_leidos += leidos;
            a_leer       -= leidos;
        }
    }
    while( ((leidos > 0) && (total_leidos < n)) || (errno== EINTR));

    if(leidos > 0)
        return total_leidos;
    else
        return leidos;
}