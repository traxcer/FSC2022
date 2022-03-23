/* Se quiere diseñar un programa que almacena información de forma estructurada en un fichero, 
para su posterior recuperanción. Para ello, se pide, dada la siguiente estructura

struct PDU {
   short size;
   char * payload;
   long crc;
};
Diseñar la siguiente función:

int guardar(char * filename, struct PDU * pdu);

que toma como argumento un nombre de fichero, y guarda en él el contenido del struct que se le 
pasa como argumento. 
El fichero, si no existe, se crea, y si existe, se añade por el final. 
La función devuelve -1 en caso de error, o 0 en cualquier otro caso.

Para probar su funcionamiento, será necesario diseñar un programa principal que tome como 
argumentos, por un lado, el nombre del fichero y, por otro, un mensaje de texto de longitud 
arbitraria. 
En dicho programa principal se deberá crear la estructura correspondiente para 
pasársela a la función guardar(). Los campos de la estructura son:
short size: la longitud del mensaje recibido como argumento del main.
char * payload: una cadena de caracteres con el mensaje recibido.
long crc: este campo simula un código corrector de error que se calcula sumando los valores 
en la tabla ASCII de los caracteres que componen la cadena. Es decir, para la 
cadena c = "abc", crc = c[0] + c[1] + c[2];
Prueba 1: Compilación
Prueba de funcionamiento

Partiendo de que "data.dat" no existe, una posible secuencia ejecución del sería:
           $ ./guardar data.dat kqm
           // data.dat debe incluir: 
           //      PDU 1: {size = 3, payload = kqm, crc = 329} 
           //      donde 329 es la suma de 'k' + 'q' + 'm'
          $ ./guardar data.dat f189g1aw
           // data.dat debe incluir: 
           //      PDU 1: {size = 3, payload = kqm, crc = 329} 
           //      PDU 2: {size = 8, payload = f189g1aw, crc = 632}
Nótese que el contenido de las PDUs están entre comentarios porque no es más que una representación 
gráfica para ejemplificar una solución, pero en realidad se ha debido generar un fichero binario.
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
int guardar(char * filename, struct PDU * pdu);

//==================================================
int main(int argc, char* argv[]){
   if (argc<2){
      printf("Debe usarse: %s payload\n",argv[0]);
      return 1;
   }
   struct PDU pdu;
   pdu.size = strlen(argv[1]);
   pdu.payload=(char *) malloc (pdu.size); //asigno memoria en el heap para el puntero del payload
   memcpy(pdu.payload,argv[1],pdu.size); //asigno la cadena que he pasado al payload
   pdu.crc=0;
   for (int i=0;i<pdu.size;i++){
      pdu.crc+=pdu.payload[i];
   }
   printf("PDU:\n\tpdu.size=%d\n\tpdu.payload=%s\n\tpdu.crc=%lu\n",pdu.size,pdu.payload,pdu.crc);
   guardar("pdu.txt",&pdu);
   printf("\nHemos terminado\n");
}
//===================================================
int guardar(char * filename, struct PDU * pdu){
int fd, escrito;
   //Abre el fichero para añadir, si no existe se crea
   fd=open(filename, O_WRONLY | O_APPEND | O_CREAT,0666);
   if (fd<0){
       perror("Error en open");
       exit(-1);
    }
   //Escribe la estructura
   // printf("PDU a Escribir:\n\tpdu.size=%d\n\tpdu.payload=%s\n\tpdu.crc=%lu\n",pdu->size,pdu->payload,pdu->crc);
   escrito=write(fd,&pdu->size,sizeof(short));
   if (escrito<0){
       perror("Error escribiendo pdu.size");
       exit(-1);
    }
   escrito=write(fd,pdu->payload,(pdu->size));
   if (escrito<0){
       perror("Error escribiendo pdu.payload");
       exit(-1);
    }
    escrito=write(fd,&pdu->crc,sizeof(long));
   if (escrito<0){
       perror("Error escribiendo pdu.crc");
       exit(-1);
    }
   
close(fd);
return 1;
}
