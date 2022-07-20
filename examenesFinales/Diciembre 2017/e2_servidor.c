 #include
 <sys/socket.h>
4
 #include
 <sys/types.h>
5
 #include
 <netinet/in.h>
6
 #include
 <arpa/inet.h>
7
 #include
 <unistd.h>
8
 #include
 <stdio.h>
9
 #include
 <time.h>
10
 #include
 <stdlib.h>
11
 #include
 <string.h>
12
 #include
 <errno.h>
13
 #include
 <signal.h>
14
 #include
 <sys/stat.h>
15
 #include
 <fcntl.h>
16
17
 #define PUERTO 4950
18
 #define T 200
19
20
 ssize_t writen(int fd, void * dato, size_t n)
21
 {
22
 size_t TotalLeido = 0;
23
 size_t Intento = n;
24
 char * p = (char *) dato;
25
 ssize_t leidos;
26
27
 do
28
 {
29
 errno = 0;
30
 leidos = write(fd, p+TotalLeido, Intento);
31
 if(leidos > 0)
32
 {
33
 TotalLeido += leidos;
34
 Intento -= leidos;
35
 }
36
 } while (((leidos > 0) && (TotalLeido < n))||(errno == EINTR));
37
38
 if(leidos < 0)
39
 {
40
 return leidos;
41
 }else{
42
 return TotalLeido;
43
 }
44
45
 }
46
47
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
74 int main(int argc, char * argv[])
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
90
 memset(&direccion_servidor, 0, sizeof(direccion_servidor));
91
 direccion_servidor.sin_family = AF_INET;
92
 direccion_servidor.sin_port = htons(PUERTO);
93
 direccion_servidor.sin_addr.s_addr = INADDR_ANY;
94
95
 sd = socket(PF_INET, SOCK_STREAM, 0);
96
 if(sd < 0)
97
 {
98
 perror("Error al crear el socket\n");
99
 exit(1);
100
 }
101
102
 if(bind(sd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor))
<0)
103
 {
104
 perror("Error en bind");
105
 exit(1);
106
 }
107
108
 if(listen(sd, 10) < 0)
109
 {
110
 perror("Error en listen");
111
 exit(1);
112
 }
113
114
 longitud_cliente = sizeof(direccion_cliente);
115
116
 while(1)
117
 {
118
 sd_data = accept(sd, (struct sockaddr *)&direccion_cliente,
&longitud_cliente);
if(sd_data < 0)
{
perror("error en accept");
exit(1);
}

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
if(b[i]>='A' && b[i]<= 'Z')
{
b[i] = b[i] - offset;
}
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
if(close(sd_data)<0)
{
perror("error al cerrar sd_Data");
close(sd);
exit(1);
}
}
if(close(sd)<0)
{
perror("error al cerrar sd");
exit(1);
}
}
