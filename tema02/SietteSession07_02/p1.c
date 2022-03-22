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
arbitraria. En dicho programa principal se deberá crear la estructura correspondiente para 
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

struct PDU {
   short size;
   char * payload;
   long crc;
}pdu;

//Prototipos de funciones
int guardar(char * filename, struct PDU * pdu);

//==================================================
int main(){
   pdu.size = 10;
   pdu.payload="payload";
   pdu.crc=pdu.payload[0] + pdu.payload[1] + pdu.payload[2];
   guardar("pdu.dat",&pdu);
   printf("\nHemos temrinado");
}
//===================================================
int guardar(char * filename, struct PDU * pdu){
int fd;
ssize_t escrito;
    if ((fd=open(filename,O_CREAT || O_WRONLY || O_APPEND,644)==-1)){ //Abrimos el fichero para añadir, si no existe se crea
       perror("open");
       exit(-1);
    }
   if((escrito=write(fd,pdu, sizeof(pdu)))<0){
      perror("write");
      exit(-1);
   }
   return 1;
}
