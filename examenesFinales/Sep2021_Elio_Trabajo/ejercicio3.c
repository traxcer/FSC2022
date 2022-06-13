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

#define GIRO 50
#define MOVIMIENTO 70
#define CONTENEDOR 1024


int main(int argc, char* argv[]){

    int sd, r;
    struct sockaddr_in dirServer;
    uint16_t tipo, longitud;
    int tipo_de_mensaje;
    int enviados;

    int giro;  
    int x;         
    int y;  

    if(argc == 2){
        giro = atoi(argv[1]);
        printf("El valor del giro es de: %d\n", giro);
        tipo_de_mensaje= GIRO;
    }

    if(argc == 3){
        x = atoi(argv[1]);
        y = atoi(argv[2]);
        tipo_de_mensaje = MOVIMIENTO;
    }

    if(argc == 4){
        x =    atoi(argv[1]);
        y =    atoi(argv[2]);
        giro=  atoi(argv[3]);
        tipo_de_mensaje= CONTENEDOR;
    }


    sd = socket(PF_INET, SOCK_STREAM ,0);
    if(sd < 0){
        perror("Error en el socket");
        exit(1);
    }

    uint32_t dir = inet_addr("127.0.0.1");
    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family   = AF_INET;
        dirServer.sin_port     = htons(PUERTO);
        memcpy(&dirServer.sin_addr, &dir, 4);


    r= connect( sd, (struct sockaddr*) &dirServer, sizeof(dirServer)); 
    if(r< 0){
        perror("Error en el connect");
        exit(1);
    } 
    else
        printf("Conexion establecida con exito!\n");




    if(tipo_de_mensaje == GIRO){
        tipo = htons(GIRO);
        int16_t giro_bigEndian= htons(giro);
        longitud= htons(sizeof(giro_bigEndian));

        enviados= write(sd, &tipo, sizeof(tipo));
        if(enviados < 0){
            perror("Error al enviar el tipo");
            exit(1);
        }
        enviados= write(sd, &longitud, sizeof(longitud));
        if(enviados < 0){
            perror("Error al enviar la longitud");
            exit(1);
        }
        enviados= write(sd, &giro_bigEndian, sizeof(giro_bigEndian));
        if(enviados < 0){
            perror("Error al enviar los datos de giro");
            exit(1);
        }          
    }

    if(tipo_de_mensaje == MOVIMIENTO){
        tipo = htons(MOVIMIENTO);       
        int16_t x_bigEndian= htons(x);
        int16_t y_bigEndian= htons(y);
        

        longitud= (htons(sizeof(y_bigEndian))) * 2 ;

        enviados= write(sd, &tipo, sizeof(tipo));
        if(enviados < 0){
            perror("Error al enviar el tipo");
            exit(1);
        }
        enviados= write(sd, &longitud, sizeof(longitud));
        if(enviados < 0){
            perror("Error al enviar la longitud");
            exit(1);
        }
        enviados= write(sd, &x_bigEndian, sizeof(x_bigEndian));
        if(enviados < 0){
            perror("Error al enviar los datos de x");
            exit(1);
        }
        enviados= write(sd, &y_bigEndian, sizeof(y_bigEndian));
        if(enviados < 0){
            perror("Error al enviar los datos de y");
            exit(1);
        }        
    }


    if(tipo_de_mensaje == CONTENEDOR){
        tipo = htons(CONTENEDOR);
        int16_t giro_bigEndian= htons(giro);
        int16_t x_bigEndian= htons(x);
        int16_t y_bigEndian= htons(y);

        longitud= htons(2);

        enviados= write(sd, &tipo, sizeof(tipo));
        if(enviados < 0){
            perror("Error al enviar el tipo");
            exit(1);
        }
        enviados= write(sd, &longitud, sizeof(longitud));
        if(enviados < 0){
            perror("Error al enviar la longitud");
            exit(1);
        }
     
        enviados= write(sd, &x_bigEndian, sizeof(x_bigEndian));
        if(enviados < 0){
            perror("Error al enviar los datos de x");
            exit(1);
        }
        enviados= write(sd, &y_bigEndian, sizeof(y_bigEndian));
        if(enviados < 0){
            perror("Error al enviar los datos de y");
            exit(1);
        }   

        enviados= write(sd, &giro_bigEndian, sizeof(giro_bigEndian));
        if(enviados < 0){
            perror("Error al enviar los datos de giro");
            exit(1);
        }     
    }


    if(close(sd) < 0){
        perror("Error al cerrar el socket descriptor");
        exit(1);
    }

    return 0;
}