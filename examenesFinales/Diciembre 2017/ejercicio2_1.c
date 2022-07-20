#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PUERTO 4950
#define T 200

//Funciones de la Asignatura readn y writen
//===============================================
ssize_t readn(int fd, void* datos,size_t n){
    ssize_t leidos=0;
    size_t intento=n;
    size_t totalLeidos=0;
    char * p=(char*) datos;
    do{
        errno=0;
        leidos=read(fd, p+totalLeidos,intento);
        if (leidos>0){
            totalLeidos+= leidos;
            intento -=leidos;
        }
        
    }  while(((leidos>0)&&(totalLeidos<n))||errno ==EINTR);
    if (leidos<0){
        return leidos;
    }else {
        return totalLeidos;
    }
}
//===============================================
ssize_t writen(int fd, void* datos,size_t n){
    ssize_t escritos=0;
    size_t intento=n;
    size_t totalEscritos=0;
    char * p=(char*) datos;
    do{
        errno=0;
        escritos=write(fd,p+totalEscritos,intento);
        if (escritos>0){
            totalEscritos+= escritos;
            intento -=escritos;
        }
    }  while(((escritos>0)&&(totalEscritos<n))||errno ==EINTR);
    if (escritos<0){
        return escritos;
    }else {
        return totalEscritos;
    }
}
//===============================================

/*================
 PROGRAMA PRINCIPAL
==================*/
int main (int argc, char* argv[]){
    if (argc<2){
        printf("Usar: %s <IP>\n",argv[0]);
        exit (1);
    }
    int sockfd;
    struct sockaddr_in server_addr; 
    //struct sockaddr_in client_addr;
    //socklen_t sin_size = sizeof(client_addr);
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
            perror("Error al crear el socket");
            exit(1);
    }
    memset((char *)&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PUERTO);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    int leidos, escritos,fd_origen, fd_destino,leidos_fich;
    uint16_t opcion,opcionBe;
    char origen[T], destino[T],b[T], cod[T];
    
    if (connect(sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr))<0){
        perror("error en connect");
        exit(1);
    }
    printf("Selecciona el modo en que ejecuta el programa (0-> encriptado, 1->desencriptado\n");
    readn(0,&opcion, sizeof(opcion));
    printf("Escriba el nombre del fichero de origen:\n");
    leidos=read(0,origen,T);
    origen[leidos-1]='\0';
    printf("Escriba el nombre del fichero destino:\n");
    leidos=read(0,destino,T);
    destino[leidos-1]='\0';
    printf("Opcion: %d\n", opcion);
    //enviamos la opcion:
    opcionBe = htons(opcion-2608);
    printf("OpcionBe: %d\n", opcionBe);
    escritos = writen(sockfd, &opcionBe, sizeof(opcionBe));
    if(escritos != sizeof(opcionBe)){
        perror("error al enviar al servidor la opcion.");
        close(sockfd);
        exit(1);
    }
    //Abrimos los ficheros
    fd_origen = open(origen, O_RDONLY);
    if(fd_origen < 0){
        perror("error al abrir fd_origen");
        close(sockfd);
        exit(1);
    }
    fd_destino = open(destino, O_WRONLY);
    if(fd_destino < 0){
        perror("error al abrir fd_destino");
        close(fd_origen);
        close(sockfd);
        exit(1);
    }
    do{
        //Leemos del fichero de origen
        leidos_fich = readn(fd_origen, b, 32);
        if(leidos_fich < 0){
            perror("error al leer ficher origen");
            close(fd_origen);
            close(fd_destino);
            close(sockfd);
            exit(1);
        }
        //Enviamos al servidor
        escritos = writen(sockfd, b, 32);
        if(escritos < 0){
            perror("error al enviar al servidor");
            close(fd_origen);
            close(fd_destino);
            close(sockfd);
            exit(1);
        }
        //Recibimos texto codificado
        leidos = readn(sockfd, cod, 32);
        if(leidos < 0){
            perror("error al recibir el texto codificado");
            close(fd_origen);
            close(fd_destino);
            close(sockfd);
            exit(1);
        }
        //Escribidos en el fichero destino
        escritos = writen(fd_destino, cod, 32);
        if(escritos < 0){
            perror("error al escribir en el fichero destino");
            close(fd_origen);
            close(fd_destino);
            close(sockfd);
            exit(1);
        }
    } while (leidos_fich > 0);
    printf("El contenido de origen es: %s\n", b);
    printf("El contenido de destino es: %s\n", cod);
    if(close(fd_origen)<0){
        perror("error al cerrar fd_origen");
        close(fd_destino);
        close(sockfd);
        close(1);
    }
    if(close(fd_destino)<0){
        perror("error al cerrar fd_destino");
        close(sockfd);
        exit(1);
    }
    if(close(sockfd)<0){
        perror("error al cerar sd");
        exit(1);
    }

} //main