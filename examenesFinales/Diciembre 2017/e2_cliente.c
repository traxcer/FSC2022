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
ssize_t readn(int fd, void * dato, size_t n){

 size_t TotalLeido = 0;
 size_t Intento = n;
 char * p = (char *) dato;
 ssize_t leidos;
 do{
    errno = 0;
    leidos = read(fd, p+TotalLeido, Intento);
    if(leidos){
        TotalLeido += leidos;
        Intento -= leidos;
 }
35
 }while (((leidos > 0) && (TotalLeido < n))||(errno == EINTR));
36
37
 if(leidos < 0)
38
 {
39
 return leidos;
40
 }else{
41
 return TotalLeido;
42
 }
43
44
 }
45
46
 ssize_t writen(int fd, void * dato, size_t n)
47
 {
48
 size_t TotalLeido = 0;
49
 size_t Intento = n;
50
 char * p = (char *) dato;
51
 ssize_t leidos;
52
53
 do{
54
 errno = 0;
55
 leidos = write(fd, p+TotalLeido, Intento);
56
 if(leidos)
57
 {
58
 TotalLeido += leidos;
59
 Intento -= leidos;
60
 }
}while (((leidos > 0) && (TotalLeido < n))||(errno == EINTR));
62
63
 if(leidos < 0)
64
 {
65
 return leidos;
66
 }else{
67
 return TotalLeido;
68
 }
69
70 }
71
72 int main(int argc, char * argv[])
73 {
74
 if(argc < 2)
75
 {
76
 perror("Usa: %s <IP>");
77
 exit(1);
78
 }
79
80
 struct sockaddr_in direccion_servidor;
81
 uint32_t ip_servidor;
82
 int sd, leidos, escritos, fd_origen, fd_destino, leidos_fich;
83
 uint16_t opcion, opcionBe;
84
 char origen[T], destino[T], b[T], cod[T];
85
 memset(&direccion_servidor, 0, sizeof(direccion_servidor));
86
 direccion_servidor.sin_family = AF_INET;
87
 direccion_servidor.sin_port = htons(PUERTO);
88
89
 if((ip_servidor = inet_addr(argv[1])<0))
90
 {
91
 printf("Error al convertir la ip");
92
 exit(1);
93
 }
94
95
 memcpy(&direccion_servidor.sin_addr, &ip_servidor, sizeof(ip_servidor));
96
97
 sd = socket(PF_INET, SOCK_STREAM, 0);
98
 if(sd < 0)
99
 {
100
 perror("error al crear el socket");
101
 exit(1);
102
 }
103
104
 if(connect(sd, (struct sockaddr *)&direccion_servidor,
sizeof(direccion_servidor))<0)
105
 {
106
 perror("error en connect");
107
 exit(1);
108
 }
109
110
 printf("Selecciona el modo en que se ejecuta el programa (0 -> encriptado, 1->
desencriptado)\n");
111
 readn(0, &opcion, sizeof(opcion));
112
 printf("Escriba el nombre del fichero de origen:\n");
113
 leidos = read(0, origen, T);
114
 origen[leidos - 1] = '\0';
115
 printf("Escriba el nombre del fichero destino:\n");
116
 leidos = read(0, destino, T);
117
 destino[leidos-1] = '\0';
118
printf("Opcion: %d\n", opcion);
//enviamos la opcion:
opcionBe = htons(opcion-2608);
printf("OpcionBe: %d\n", opcionBe);
escritos = writen(sd, &opcionBe, sizeof(opcionBe));
if(escritos != sizeof(opcionBe))
{
perror("error al envia al servidor la opcion.");
close(sd);
exit(1);
}
//Abrimos los ficheros
fd_origen = open(origen, O_RDONLY);
if(fd_origen < 0)
{
perror("error al abri fd_origen");
close(sd);
exit(1);
}
fd_destino = open(destino, O_WRONLY);
if(fd_destino < 0)
{
perror("error al abrir fd_destino");
close(fd_origen);
close(sd);
exit(1);
}
do
{
//Leemos del fichero de origen
leidos_fich = readn(fd_origen, b, 32);
if(leidos_fich < 0)
{
perror("error al leer ficher origen");
close(fd_origen);
close(fd_destino);
close(sd);
exit(1);
}
//Enviamos al servidor
escritos = writen(sd, b, 32);
if(escritos < 0)
{
perror("error al enviar al servidor");
close(fd_origen);
close(fd_destino);
close(sd);
exit(1);
}
//Recibimos texto codificado
leidos = readn(sd, cod, 32);
if(leidos < 0)
{
perror("error al recibir el texto codificado");
close(fd_origen);
close(fd_destino);
close(sd);
exit(1);
}
//Escribidos en el fichero destino
escritos = writen(fd_destino, cod, 32);
if(escritos < 0)
{
perror("error al escribir en el fichero destino");
close(fd_origen);
close(fd_destino);
close(sd);
exit(1);
}
} while (leidos_fich > 0);
printf("El contenido de origen es: %s\n", b);
printf("El contenido de destino es: %s\n", cod);
if(close(fd_origen)<0)
{
perror("error al cerrar fd_origen");
close(fd_destino);
close(sd);
close(1);
}
if(close(fd_destino)<0)
{
perror("error al cerrar fd_destino");
close(sd);
exit(1);
}
if(close(sd)<0)
{
perror("error al cerar sd");
exit(1);
}
}