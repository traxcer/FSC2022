/*
Dado un fichero binario que contiene información de varias estructuras 
PDU como la siguiente:

struct PDU {
   short size;
   char * payload;
   long crc;
};
Se pide diseñar una función

void cargar(char * filename);

que toma como argumento un nombre de fichero y muestra por pantalla la información de 
todas las PDUs incluidas en el fichero. Así, para un fichero con 3 PDUS, la información 
se debe mostrar con el siguiente formato:

PDU 1: {size = 3,payload = 0e3,crc = 200}
PDU 2: {size = 2,payload = 22,crc = 100}
PDU 3: {size = 6,payload = l9r649,crc = 442}

Además, una vez cargado el payload, a función debe calcular también el código de error (crc) 
y comprobar que el valor leído del fichero, y el calculado, coinciden. Si no es así, se 
debería mostrar PDU corrupta . 
Por ejemplo, al cargar la PDU 2: {size = 10,payload = z66cwupd86,crc = 887} 
y el crc es distinto de 887, entonces debe salir por pantalla:
PDU 1: {size = 10,payload = j697qu29tb,crc = 823}
PDU 2 corrupta
PDU 3: {size = 12,payload = 515al84r6z6p,crc = 924}
Nota importante: la salida debe ser exactamente igual a la mostrada en el ejemplo.
Prueba 1: Compilación
Prueba de funcionamiento

Asumiendo que "data.dat" tiene PDUs para los siguientes tres mensajes:
         Mensaje 1: t
         Mensaje 2: 01bck0j7
         Mensaje 3: n2fv174xt25
la salida esperada sería:
           $ ./cargar data.dat
           PDU 1: {size = 1,payload = t,crc = 116}
           PDU 2: {size = 8,payload = 01bck0j7,crc = 610}
           PDU 3: {size = 11,payload = n2fv174xt25,crc = 875}
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct PDU {
   short size;
   char * payload;
   long crc;
};

//Prototipos de funciones
int cargar(char * filename);

//==================================================
int main(int argc, char* argv[]){
   if (argc<2){
      printf("Debe usarse: %s pdu.txt\n",argv[0]);
      return 1;
   }
   cargar(argv[1]);
}
//===================================================
int cargar(char * filename){
    struct PDU pdu;
    int fd,leo;
   //Abre el fichero para lectura
   fd=open(filename, O_RDONLY);
   if (fd<0){
       perror("Error OPEN");
       exit(-1);
    }
   //Lee la estructura
   int numpdu=1;
   long crcchk;
   while((leo=read(fd,&pdu.size,sizeof(short)))>0){
        printf("PDU %d",numpdu);
        pdu.payload=(char*)malloc(pdu.size);
        read(fd,pdu.payload,pdu.size);
        read(fd,&pdu.crc,sizeof(long));
        crcchk=0;
        for (int i=0;i<pdu.size;i++)
            crcchk+=pdu.payload[i];
        if (crcchk==pdu.crc) printf(": {size = %d,payload = %s,crc = %lu}\n",pdu.size,pdu.payload,pdu.crc);
        else printf("corrupta\n");
        numpdu++;
    }
    if (leo<0){
        perror("Error lecrura");
        close (fd);
        return -1;
    }
close(fd);
return 1;
}
