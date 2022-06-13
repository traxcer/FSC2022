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
#define MAX_CONEXIONES 10
#define T 512


void m1(int signo){
    wait(0);
    signal(SIGCLD, m1);
}




int main( int argc, char* argv[]){  

    if(argc < 7){
        perror("NUMERO DE ARGUMENTOS INSUFICIENTE");
        exit(1);
    }

    if( (signal(SIGCLD, m1)) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(1);
    }

    //Variables del programa
    int sd, sd_cliente;
    int recibidos, enviados, r;
    struct sockaddr_in dirServer;
    struct sockaddr_in dirCliente;
    socklen_t lon_dirCliente = sizeof(dirCliente);
    int cabecera;
    uint16_t cabeceraBigEndian;
    char buffer[T];
    char nombre[124];
    pid_t pid;

    printf("argumento 1: %s \n", argv[1]);
    printf("argumento 2: %s \n", argv[2]);
    printf("argumento 3: %s \n", argv[3]);
    printf("argumento 4: %s \n", argv[4]);
    printf("argumento 5: %s \n", argv[5]);
    printf("argumento 6: %s \n\n", argv[6]);
    


    char* nombre1 = argv[1];
    char* ip1     = argv[2];

    char* nombre2 = argv[3];
    char* ip2     = argv[4];

    char* nombre3 = argv[5];
    char* ip3     = argv[6];

    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error al iniciar el socket");
        exit(1);
    }

    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family      = AF_INET;
        dirServer.sin_addr.s_addr = INADDR_ANY;
        dirServer.sin_port        = htons(PUERTO);

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
        sd_cliente= accept(sd, (struct sockaddr*) &dirCliente, &lon_dirCliente);
        if(sd_cliente < 0){
            perror("Error en el accept");
            exit(1);
        }

        printf("Conexion aceptada con exito\n");

        pid= fork();
        if(pid < 0){
            perror("error al engendrar el hijo");
            exit(1);
        }

        //CODIGO DEL HIJO
        else if(pid == 0){

            while(1){
                recibidos= read(sd_cliente, &cabeceraBigEndian, sizeof(cabeceraBigEndian));
                if(recibidos < 0){
                    perror("Error al recibir la cabecera");
                    exit(1);
                }
                else if( recibidos == 0){
                    printf("El cliente se ha desconectado...\n");
                    if ( (close(sd_cliente)) < 0){
                        perror("Error al cerrar el sd_cliente");
                        exit(1);
                    }
                    printf("Cierre completado!\n");
                    break;
                }

                cabecera= ntohs(cabeceraBigEndian);

                recibidos= read(sd_cliente, buffer, cabecera) ;
                if(recibidos != cabecera){
                    perror("Error al recibir la direccion");
                    exit(1);
                }

                buffer[cabecera] = '\0';
                printf("IP recibida: %s \n", buffer);

                if( !(strcmp(ip1, buffer)) )
                    strcpy(nombre, nombre1);

                else if( !(strcmp(ip2, buffer)) )
                    strcpy(nombre, nombre2 );

                else if( !(strcmp(ip3, buffer)) )
                    strcpy(nombre, nombre3);

                else
                    strcpy(nombre, "0.0.0.0");
                
                cabecera= strlen(nombre);
                cabeceraBigEndian= htons(cabecera);

                enviados= write(sd_cliente, &cabeceraBigEndian, sizeof(cabeceraBigEndian));
                if(enviados < 0){
                    perror("Error al enviar la cabecera al cliente");
                    exit(1);
                }

                enviados= write(sd_cliente, nombre, strlen(nombre));
                if(enviados != (strlen(nombre)) ){
                    perror("Error al enviar el nombre de la ip recibida");
                    exit(1);
                }
                printf("Enviado: %s\n", nombre);

            }//fin while con cliente.

        }
        //CODIGO DEL PADRE
        else{
            r= close(sd_cliente);
            if(r < 0){
                perror("Error al cerrar el sd con el cliente en el padre");
                exit(1);
            }
        }
  

    }//win while del server
    
    return 0;
}
