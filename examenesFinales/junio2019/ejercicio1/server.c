#define _POSIX_SOURCE_ 
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



//variables globales

#define PUERTO 4950
#define T 400
#define BLOQUE 50
#define MAX_CONEXIONES 10

//Funciones auxiliares
ssize_t read_n(int fd, void* b, size_t n);
ssize_t write_n(int fd, void* b, size_t n);
int enviarFichero(int sd, char* filename);


int main(int argc, char* argv[]){

    int sd, sd_client;
    struct sockaddr_in dirSever;
    struct sockaddr_in dirClient;
    socklen_t long_dir = sizeof(dirClient);
    int r;

    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error al crear el socket");
        exit(-1);
        }
    
     memset(&dirSever, 0, sizeof(dirSever));
        dirSever.sin_family      = AF_INET;
        dirSever.sin_port        = htons(PUERTO);
        dirSever.sin_addr.s_addr = INADDR_ANY;
    
    r= bind(sd, (struct sockaddr*) &dirSever, sizeof(dirSever));
    if(r< 0){
        perror("Error en el bind");
        close(sd);
        exit(1);
    }

    r= listen(sd, MAX_CONEXIONES);
    if(r < 0){
        perror("Error en el listen");
        close(sd);
        exit(1);
    }

    while(1){
        printf("Eserando conexion entrante...\n");

        sd_client= accept(sd, (struct sockaddr*) &dirClient, &long_dir);
        if(sd_client < 0){
            perror("Error al aceptar el cliente");
            close(sd);
            exit(1);
        }
        else
            printf("Cliente aceptado con exito!\n");



        //Envio del primer fichero
        r= enviarFichero( sd_client, "cam1.jpg");
        if(r < 0){
            perror("Error al enviar fichero cam 1...\n");
            close(sd);
            close(sd_client);
            exit(1);
        }

        //Envio del segundo fichero
        r= enviarFichero( sd_client, "cam2.jpg");
        if(r < 0){
            perror("Error al enviar fichero cam 2...\n");
            close(sd);
            close(sd_client);
            exit(1);
        }


        if(close(sd_client) < 0){
            perror("Error al cerrar el sd del cliente");
            close(sd);
            exit(1);
        }

    }
    

    return 0;
}




//IMPLEMENTACION DE FUNCIONES
ssize_t read_n(int fd, void* b, size_t n){
    size_t a_leer= n;
    ssize_t leidos;
    ssize_t total_leidos= 0;

    do{
        errno= 0;
        leidos= read(fd, b+total_leidos, a_leer);
        if(leidos >= 0){
            total_leidos += leidos;
            a_leer       -= leidos;
        }
    }
    while(( (leidos > 0) && ( total_leidos < n) ) || (errno == EINTR) );

    if(leidos > 0)
        return total_leidos;
    else
        return leidos;

}


ssize_t write_n(int fd, void* b, size_t n){

    size_t a_escribir = n;
    ssize_t escritos;
    ssize_t total_escritos = 0;

    do{
        errno= 0;
        escritos= write(fd, b+ total_escritos, a_escribir);
        if(escritos > 0){
            total_escritos += escritos;
            a_escribir     -= escritos;
        }
    }
    while(((escritos > 0 )&& (total_escritos < n) ) || (errno == EINTR));
    
    if(escritos > 0)
        return total_escritos;
    else
        return escritos;
}


int enviarFichero(int sd, char* filename){

    int ok= 0;
    int fail= -1;
    int fd, leidos, enviados;
    uint16_t leidos_bigEndian;
    char buffer[T];


    fd= open(filename, O_RDONLY);
    if(fd < 0){
        printf("Error al abrir el arvhivo\n");
        close(fd);
        return fail;
    }

    do{
        //Lee un bloque del archivo
        leidos= read(fd, buffer, BLOQUE);
        if(leidos < 0){
            printf("Error al leer el archivo\n");
            close(fd);
            return fail;
        }
        
        //manda la cabecera
        leidos_bigEndian= htons(leidos);
        enviados= write_n(sd, &leidos_bigEndian, sizeof(leidos_bigEndian));
        if(enviados !=  sizeof(leidos_bigEndian)){
            printf("Error al mandar la cabecera\n");
            close(fd);
            return fail;
        }

        //manda el fragmento del archivo
        if(leidos > 0){
            enviados= write_n(sd, buffer, leidos);
            if(enviados != leidos){
                printf("Error al mandar bloque de datos\n");
                close(fd);
                return fail;
            }
            printf("Se han mandado %d bytes\n", enviados);
        }

    
        if(leidos == 0){
            printf("Archivo transmitido con exito!\n");

            if( (close (fd)) < 0){
                printf("Error al cerrar el descriptor del fichero\n");
                return fail;
            } 
            else
                return ok;
        }
    

    } while(leidos != 0);
    
    return ok;
}