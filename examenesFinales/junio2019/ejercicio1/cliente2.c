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
#define N_INTENTOS 3

//Funciones auxiliares
ssize_t read_n(int fd, void* b, size_t n);
ssize_t write_n(int fd, void* b, size_t n);
int leerFichero(int sd, char* filename);


//manejadores de la señal
void m1(int signo){

    signal(SIGALRM, m1);
}



int main(int argc, char* argv[]){

    //argv[1] --> ip
    //argv[2] --> nombre fichero 1
    //argv[3] --> nomre fichero 2

    int sd, r;
    struct sockaddr_in dirServer;
    uint32_t ip_en_bigEndian;

    struct itimerval temporizador;
    struct timeval inicio, intervalo;
    inicio.tv_sec= 2;
    inicio.tv_usec= 0;
    intervalo.tv_sec= 0;
    intervalo.tv_usec= 0;

    temporizador.it_value= inicio;
    temporizador.it_interval= intervalo;

    if( (signal(SIGALRM, m1) ) == SIG_ERR){
        perror("Error en el manejador de la signal");
        exit(-1);
    }
    


    if(argc < 4){
        printf("Numero de argumentos insuficiene\n");
        exit(1);
    }


    sd= socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Error al  crear el socket");
        exit(-1);
    }

    ip_en_bigEndian= inet_addr(argv[1]);
    if(ip_en_bigEndian < 0){
        perror("error en la ip");
        close(sd);
        exit(1);
    }


    memset(&dirServer, 0, sizeof(dirServer));
        dirServer.sin_family  =   AF_INET;
        dirServer.sin_port    =     htons(PUERTO);
        memcpy(&dirServer.sin_addr, &ip_en_bigEndian, 4);
    

    r= connect(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));
    if(r < 0){
        printf("Error al conectar con el servidor\n");
    }

    int cont=0;
    while( (r < 0) && (cont < N_INTENTOS) ){
        printf("Reconectando con el servidor...\n");
        setitimer(ITIMER_REAL, &temporizador, NULL);
        pause();

        r= connect(sd, (struct sockaddr*) &dirServer, sizeof(dirServer));

        cont++;
    }

    if(r < 0){
        printf("Tiempo de conexion expritado. No se pudo establecer conexion\n");
        close(sd);
        return(1);
    }
    else
        printf("Conexion establecda con exito!\n");
   


    //recepcion del primer archivo
    r= leerFichero(sd, argv[2]);
    if(r< 0){
        perror("Error al leer el arvhido 1 del satelite");
        close(sd);
        exit(1);
    }


    //Recepcion del 2 archivo
    r= leerFichero(sd, argv[3]);
    if(r< 0){
        perror("Error al leer el arvhido 3 del satelite");
        close(sd);
        exit(1);
    }


    if( (close(sd)) < 0){
        perror("Error al cerrar le descriptor del socket");
        exit(1);
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

int leerFichero(int sd, char* filename){

    int ok= 0;
    int fail= -1;
    int fd, leidos, escritos, cabecera;
    uint16_t cabecera_en_bigEndian;
    char buffer[T];


    fd= open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if(fd < 0){
        printf("Error al crear el arvhivo\n");
        return fail;
    }

    do{
        
        //lee la cabecera
        leidos= read_n(sd, &cabecera_en_bigEndian, sizeof(cabecera_en_bigEndian));
        if(leidos !=  sizeof(cabecera_en_bigEndian)){
            printf("Error al mandar la cabecera\n");
            close(fd);
            return fail;
        }

        cabecera= ntohs(cabecera_en_bigEndian);

        if(cabecera > 0){
            //lee el fragmento de archivo
            leidos= read_n(sd, buffer, cabecera);
            if(leidos != cabecera){
                printf("Error al leer el bloque de datos\n");
                close(fd);
                return fail;
            }
            printf("Se han redibido %d Bytes\n", leidos);

            //Escribe en el fichero el bloque leido
            escritos= write_n(fd, buffer, leidos);
            if(escritos != leidos){
                printf("Error al escribir en el fichero\n");
                close(fd);
                return fail;
            }
        }

    
        if(cabecera == 0){
            printf("Archivo  recinido con exito!\n");

            if( (close (fd)) < 0){
                printf("Error al cerrar el descriptor del fichero\n");
                return fail;
            } 
            else
            return ok;
        }

    }
    while(cabecera != 0);

    return ok;

}