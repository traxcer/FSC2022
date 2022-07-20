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

ssize_t writen(int fd, void * dato, size_t n){
    size_t TotalEscrito = 0;
    size_t Intento = n;
    char * p = (char *) dato;
    ssize_t escritos;
    do{
        errno = 0;
        escritos = write(fd, p+TotalEscrito, Intento);
        if(escritos > 0){
            TotalEscrito += escritos;
            Intento -= escritos;
       }
    } while (((escritos > 0) && (TotalEscrito < n))||(errno == EINTR));
    if(escritos < 0){
        return escritos;
    }else{
        return TotalEscritos;
    }
 }

 ssize_t readn(int fd, void * dato, size_t n)
48
 {
49
 size_t TotalLeido = 0;
50
 size_t Intento = n;
51
 char * p = (char *) dato;
52
 ssize_t leidos;
53
54
 do
55
 {
56
 errno = 0;
57
 leidos = read(fd, p+TotalLeido, Intento);
58
 if(leidos > 0)
59
 {
60
 TotalLeido += leidos;
Intento -= leidos;
62
 }
63
 } while (((leidos > 0) && (TotalLeido < n))||(errno == EINTR));
64
65
 if(leidos < 0)
66
 {
67
 return leidos;
68
 }else{
69
 return TotalLeido;
70
 }
71
72 }
73
74 int
 main(int argc, char * argv[])
75 {
76
 if(argc < 2)
77
 {
78
 printf("Usa: %s <offset>", argv[0]);
79
 exit(1);
80
 }
81
82
 struct sockaddr_in direccion_servidor;
83
 struct sockaddr_in direccion_cliente;
84
 int sd, sd_data, leidos, escritos;
85
 uint16_t opcionBe, opcion;
86
 socklen_t longitud_cliente;
87
 char b[T];
88
 int offset = atoi(argv[1]);
89
 pid_t pid;
90
91
 memset(&direccion_servidor, 0, sizeof(direccion_servidor));
92
 direccion_servidor.sin_family = AF_INET;
93
 direccion_servidor.sin_port = htons(PUERTO);
94
 direccion_servidor.sin_addr.s_addr = INADDR_ANY;
95
96
 sd = socket(PF_INET, SOCK_STREAM, 0);
97
 if(sd < 0)
98
 {
99
 perror("Error al crear el socket\n");
100
 exit(1);
101
 }
102
103
 if(bind(sd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor))
<0)
104
 {
105
 perror("Error en bind");
106
 exit(1);
107
 }
108
109
 if(listen(sd, 10) < 0)
110
 {
111
 perror("Error en listen");
112
 exit(1);
113
 }
114
115
 longitud_cliente = sizeof(direccion_cliente);
116
117
 while(1)
118
 {
sd_data = accept(sd, (struct sockaddr *)&direccion_cliente,
&longitud_cliente);
if(sd_data < 0)
{
perror("error en accept");
exit(1);
}
pid = fork();
if(pid{
== 0)
//Recibimos del cliente la opcion:
leidos = readn(sd_data, &opcionBe, sizeof(opcionBe));
if(leidos != sizeof(opcionBe))
{
perror("error al leer opcion");
exit(1);
}
//la reconvertimos a formato maquina
opcion = ntohs(opcionBe);
printf("Recibimos la opcion: %d\n", opcion);
switch(opcion)
{
case 0:
do
{
leidos = readn(sd_data, b, 32);
if(leidos < 0)
{
perror("error al recibir cadena");
close(sd_data);
close(sd);
exit(1);
}
for(int i=0; i<32; i++)
{
if(b[i]>='a' && b[i]<= 'z')
{
b[i] = b[i] + offset;
}
if(b[i]>='A' && b[i]<= 'Z')
{
b[i] = b[i] + offset;
}
}
escritos = writen(sd_data, b, 32);
if(escritos < 0)
{
perror("error al enviar cadena codificada");
close(sd_data);
close(sd);
exit(1);
}
} while (leidos > 0);
break;
case 1:
do
{
leidos = readn(sd_data, b, 32);
if(leidos < 0)
{
perror("error al recibir cadena");
close(sd_data);
close(sd);
exit(1);
}
for(int i=0; i<32; i++)
{
if(b[i]>='a' && b[i]<= 'z')
{
b[i] = b[i] - offset;
}
}
if(b[i]>='A' && b[i]<= 'Z')
{
b[i] = b[i] - offset;
}
escritos = writen(sd_data, b, 32);
if(escritos <0)
{
perror("error al enviar cadena codificada");
close(sd_data);
close(sd);
exit(1);
}
} while (leidos > 0);
break;
}
}else{
if(close(sd_data)<0)
{
perror("error al cerrar sd_Data");
close(sd);
exit(1);
}
}
}
if(close(sd)<0)
{
perror("error al cerrar sd");
exit(1);
}
}
