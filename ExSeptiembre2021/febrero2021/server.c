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

#define T 100
#define PUERTO 2119
#define MAX_CONEXIONES 10

int ha_llehado_alarma= 0;

void m1(int signo){

    ha_llehado_alarma= 1;
    signal(SIGALRM, m1);
}

void m2(int signo){
    wait(0);
    signal(SIGCLD, m2);
}

//cabecera de las funciones
void copia(fd_set* fds, fd_set* fds_copia, int max_mas_uno);


int main(int argc, char* argv[]){

    //variables del programa
    int sd, sd_cliente, r, enviados, recibidos;
    struct sockaddr_in dirServer, dirCliente;
    char buffer[T];
    uint8_t s;
    uint32_t ms;
    socklen_t lon_dirCliente= sizeof(dirCliente);

    if( (signal(SIGALRM, m1)) == SIG_ERR){
        perror("Error en el manejador de la alarma");
        exit(1);
    }
    if( (signal(SIGCLD, m2)) == SIG_ERR){
        perror("Error en el manejador de la alarma");
        exit(1);
    }
    

    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("error en el socket");
        exit(-1);
    }

    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family      = AF_INET;
        dirServer.sin_port        = htons(PUERTO);
        dirServer.sin_addr.s_addr = INADDR_ANY;
    
    r= bind(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
    if(r< 0){
        perror("Error en el bind");
        exit(1);
    }

    r= listen(sd, MAX_CONEXIONES);
    if(r < 0){
        perror("Error en el listen");
        exit(1);
    }

    struct timeval inicio;
    int timeout;
    pid_t pid;


    while(1){
        printf("Esperando conexion entrante...\n");
        sd_cliente= accept(sd, (struct sockaddr*) &dirCliente, &lon_dirCliente);
        if(sd_cliente < 0){
            perror("Error al aceptar el cliente");
            exit(1);
        }
        printf("Conexion realizada con exito, esperando datos de tiempo\n");

        recibidos= read(sd_cliente, &s, sizeof(s));
            if(recibidos < sizeof(s)){
                perror("Error al recibir los sengudos");
                exit(1);
            }
        printf("recibidos %d Bytes\n", recibidos);
        

        recibidos= read(sd_cliente, &ms, sizeof(ms));
        if(recibidos < 0){
            perror("Error al leer los micro segundos");
            exit(1);
        }
        printf("recibidos %d Bytes\n", recibidos);

        pid= fork();
        if(pid < 0){
            perror("Error al engendrar el hijo");
            exit(1);
        }
        //CODIGO DEL HIJO
        else if(pid == 0){
            int i=0;
            int maximo;
            fd_set fds, fds_copia;

            FD_ZERO(&fds);
            FD_SET(sd_cliente, &fds);
            maximo= sd_cliente;



            while(1){

                inicio.tv_sec  = s;
                printf("Segundos: %d \n", s);
                printf("Microsec: %d\n", htonl(ms));
                inicio.tv_usec = ntohl(ms);

                copia(&fds, &fds_copia, maximo+1);
                timeout= select(maximo+1, &fds_copia, NULL, NULL, &inicio);
                if(timeout <0){
                    perror("Error en el select");
                    exit(1);
                }
                
              

                if(FD_ISSET(sd_cliente, &fds_copia)){
                    recibidos= read(sd_cliente, &s, sizeof(s));
                    if(recibidos == 0){ //Se ha cerrado la conexion al recibir 0 por el socket
                        close(sd_cliente);
                        printf("Programa finalizado correctamente!\n");
                        exit(0);
                    }

                    if(recibidos < 0){
                        perror("Error al recibir los sengudos");
                        exit(1);
                    }


                    recibidos= read(sd_cliente, &ms, sizeof(ms));
                    if(recibidos < 0){
                        perror("Error al leer los micro segundos");
                        exit(1);
                    }
                }


                if(timeout == 0){
                    sprintf(buffer, "Evento %d", i);
                    enviados= write(sd_cliente, buffer, strlen(buffer));
                    if(enviados < 0){
                        perror("Error en el envio de datos");
                        exit(1);
                    }
                    printf("Enviados %d Bytes\n", enviados);
                    i++;
                }

                
            }



        }
        //COGIDO DEL PADRE
        else{
            wait(0);
    
        }


    }//fin del while

    return 0;
}



//Implementacion de las funicones
void copia(fd_set* fds, fd_set* fds_copia, int max_mas_uno){
    FD_ZERO(fds_copia);
    for(int i=0; i< max_mas_uno; i++){
        if(FD_ISSET(i, fds)){
            FD_SET(i, fds_copia);
        }
    }
}