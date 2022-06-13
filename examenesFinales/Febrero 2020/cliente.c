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
#define BLOQUE 25


int main(int argc, char* argv[]){

    //argv[1] --> ip
    //argv[2] --> puerto
    //argv[3] --> desplazamiento

    int sd, fd, r, leidos, enviados;
    struct sockaddr_in dirServer;
    char buffer[T];
    int ok= 1;
    uint16_t cabeceraBigEndian;
    int desplazamiento= atoi(argv[3]);


    if( argc < 4){
        perror("Numero de argumentos insuficiente");
        exit(1);
    }


    uint32_t dir= inet_addr( argv[1]);
    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family = AF_INET;
        dirServer.sin_port   = htons( atoi(argv[2]) ); 
        memcpy(&dirServer.sin_addr, &dir, 4);


    
    while(ok){
        printf("Introduzca nombre del archivo o fin:\n");
        leidos= read(0, buffer, T);
        buffer[leidos-1]= '\0';

        if((strcmp("fin", buffer)) ){
            sd= socket(PF_INET, SOCK_STREAM, 0);
            if(sd < 0){
                perror("Error al crear el socket");
                exit(1);
            }
            
            fd= open(buffer, O_RDONLY);
            if(fd < 0){
                perror("Error al abrir el archivo");
                close(sd);
                exit(1);
            }

            r= connect(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
            if(r < 0){
                perror("Error al conectar con el server");
                exit(1);
            }
            else
                printf("Conexion establecida correctamente con el server!\n");

            
            leidos= read(fd, buffer, BLOQUE); //Lectura por adelantado
            while(leidos > 0){
                cabeceraBigEndian= htons(leidos);
                enviados= write(sd, &cabeceraBigEndian, sizeof(cabeceraBigEndian));
                if(enviados < 0){
                    perror("Error al mandar la cabecera al server");
                    exit(1);
                }

                //desplazamiento
                for(int j=0; j< leidos; j++){
                    buffer[j]= buffer[j] + desplazamiento;
                }

                enviados= write(sd, buffer, leidos);
                if(enviados < 0){
                    perror("Error al mandar un bloque al server");
                    exit(1);
                }

                leidos= read(fd, buffer, BLOQUE);
            }
            if(leidos == 0){
                cabeceraBigEndian= htons(leidos);
                enviados= write(sd, &cabeceraBigEndian, sizeof(cabeceraBigEndian));
                if(enviados < 0){
                    perror("Error al mandad cabecera de fin");
                    exit(1);
                }

                if((close(fd)) < 0){
                    perror("Error al cerrar el socket");
                    exit(1);
                }

                if((close(sd)) < 0){
                    perror("Error al cerrar el socket");
                    exit(1);
                }
            }
            
        }
        else{
            ok=0;
            printf("Programa finalizado con exito\n");
        }
    }//whil ok

    return 0;
}