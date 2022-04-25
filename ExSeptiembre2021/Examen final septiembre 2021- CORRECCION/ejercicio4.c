#define _POSIX_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <signal.h>

#define PUERTO 5050

#define MAX_CLIENTES 3

#define GIRO 50
#define MOVIMIENTO 70
#define CONTENEDOR 1024

//variables blobales

int contador= 0;

//manejadores de señal
void m1(int signo){
    wait(0);
    signal(SIGCHLD, m1);
}

void m2(int signo){
    contador ++;
    signal(SIGUSR1, m2);
}


int main(int argc, char* argv[]){

    int sd, sd_cliente;
    int recibidos, r;
    struct sockaddr_in dirCliente;
    struct sockaddr_in dirServer;
    socklen_t lonDirCliente=  sizeof(dirCliente);
    int tipo_de_mensaje;
    int long_de_mensaje;
    uint16_t tipo, longitud;


     sd = socket(PF_INET, SOCK_STREAM ,0);
     if(sd < 0){
        perror("Error en el socket");
        exit(1);
    }

    memset(&dirServer, 0, sizeof(dirServer));
    dirServer.sin_family       = AF_INET;
    dirServer.sin_port         = htons(PUERTO);
    dirServer.sin_addr.s_addr  = INADDR_ANY;

    r = bind(sd, (struct sockaddr *) &dirServer, sizeof(dirServer));
    if(r < 0){
        perror("Error en en el bind");
        exit(1);
    }

    r = listen(sd, MAX_CLIENTES);
    if(r < 0){
        perror("Error en en el listen");
        exit(1);
    }

    int pid;

    while(1){

        printf("Esperando coexion entrante...\n");
        sd_cliente = accept(sd, (struct sockaddr *) &dirCliente, &lonDirCliente);

        pid= fork();
        if(pid < 0){
            perror("Error al engendrar el hijo");
            exit(1);
        }

        //CODIGO DEL HIJO
        else if(pid == 0){

            int16_t giro_bigEndian;
            int16_t x_bigEndian, y_bigEndian;
            int x, y, giro;  
   
            recibidos= read(sd_cliente, &tipo, sizeof(tipo));
            if(recibidos < 0){
                perror("Error al recibir el tipo de mensaje");
                close(sd_cliente);
                exit(1);
            }
            tipo_de_mensaje= ntohs(tipo);

            recibidos= read(sd_cliente, &longitud, sizeof(longitud));
            if(recibidos < 0){
                perror("Error al recibir la longitud");
                close(sd_cliente);
                exit(1);
            }
            long_de_mensaje= ntohs(longitud);



            if(tipo_de_mensaje == GIRO){
                recibidos= read(sd_cliente, &giro_bigEndian, long_de_mensaje);
                if(recibidos < 0){
                    perror("Error al enviar los datos de giro");
                    exit(1);
                }
                giro= ntohs(giro_bigEndian);
                printf("Los datos de giro son: %d\n", giro);

                if((close(sd_cliente)) < 0){
                perror("Error al cerrar el socket del cliente en el hijo");
                exit(0);
            }   }


                      

            else if(tipo_de_mensaje == MOVIMIENTO){
                recibidos= read(sd_cliente, &x_bigEndian, sizeof(x_bigEndian));
                if(recibidos < 0){
                    perror("Error al enviar los datos de la coordenada x");
                    exit(1);
                }
                x= ntohs(x_bigEndian);

                recibidos= read(sd_cliente, &y_bigEndian, sizeof(y_bigEndian));
                if(recibidos < 0){
                    perror("Error al enviar los datos de la coordenada y");
                    exit(1);
                }
                y= ntohs(y_bigEndian);

                printf("Coordenada x es: %d\n", x);
                printf("Coordenada y es: %d\n", y);

                if((close(sd_cliente)) < 0){
                perror("Error al cerrar el socket del cliente en el hijo");
                exit(0);
                }
            }

            else if(tipo_de_mensaje == CONTENEDOR){
                recibidos= read(sd_cliente, &x_bigEndian, sizeof(x_bigEndian));
                if(recibidos < 0){
                    perror("Error al enviar los datos de la coordenada x");
                    exit(1);
                }
                x= ntohs(x_bigEndian);

                recibidos= read(sd_cliente, &y_bigEndian, sizeof(y_bigEndian));
                if(recibidos < 0){
                    perror("Error al enviar los datos de la coordenada y");
                    exit(1);
                }
                y= ntohs(y_bigEndian);

                recibidos= read(sd_cliente, &giro_bigEndian, sizeof(giro_bigEndian));
                if(recibidos < 0){
                    perror("Error al enviar los datos de giro");
                    exit(1);
                }
                giro= ntohs(giro_bigEndian);
                
                printf("Coordenada x es: %d\n", x);
                printf("Coordenada y es: %d\n", y);
                printf("Los datos de giro son: %d\n", giro);

                if((close(sd_cliente)) < 0){
                perror("Error al cerrar el socket del cliente en el hijo");
                exit(0);
                }

            }
            else{
                int pid_padre= getppid();
                kill(pid_padre, SIGUSR1);
            }

        }
        
        //CODIGO DEL PADRE
        else{

            if((close(sd_cliente)) < 0){
                perror("Error al cerrar el socket del cliente en el padre");
                exit(1);
            }

        }

    }//fin del while(1)

  
    return 0;
}

