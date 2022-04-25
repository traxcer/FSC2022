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

#define PUERTO 2119

int main(int argc, char* argv[]){

    //Variables
    int sd, r, senial;
    int enviados, leidos;
    struct sockaddr_in dirServer;
    uint32_t ack;

    if(argc < 4){
        perror("Numero de argumentos insuficiente");
        exit(1);
    }

    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error al crar el socket");
        exit(1);
    }

    uint32_t dir= inet_addr(argv[1]);
    memset(&dirServer, 0,sizeof(dirServer));
        dirServer.sin_family  = AF_INET;
        dirServer.sin_port    = htons(PUERTO);
        memcpy(&dirServer.sin_addr, &dir, 4);

    r= connect(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
    if(r < 0){
        perror("Error en el conecct");
        exit(1);
    }
    else
    printf("Conexion exitosa con el servidor\n");

    uint32_t pid_bigEndian= htonl( atoi(argv[2]) );
    uint8_t signal_bigEndian;
    if( !(memcmp( argv[3], "SIGUSR1", 8)) ){
        senial= 10;
    }
    if( !(memcmp( argv[3], "SIGUSR2", 8)) ){
        senial= 12;
    }
    signal_bigEndian= senial;


    enviados= write(sd, &pid_bigEndian, sizeof(pid_bigEndian));
    if(enviados != sizeof(pid_bigEndian)){
        perror("Error al enviar el pid");
        exit(1);
    }
    printf("Enviados %d Bytes\n", enviados);

    enviados=  write(sd, &signal_bigEndian, sizeof(signal_bigEndian));
    if(enviados != sizeof(signal_bigEndian)){
        perror("Error al mandar el codigo de la signal");
        exit(1);
    }
    printf("Enviados %d Bytes\n", enviados);

    leidos= read(sd, &ack, 4);
    if(leidos != 4){
        perror("Error al recibir el ack");
        exit(1);
    }
    printf("Leidos %d Bytes\n", leidos);
    
    int ackn= ntohl(ack);
    if( ackn < 0){
        printf("La senial no se ha enviado correctamente\n");
    }

    if(ackn == 0){
        printf("La senial se ha enviado correctamente\n");
    }


    if( (close(sd)) < 0){
        perror("Error al cerrar el sd");
        exit(1);
    }

    return 0;
}