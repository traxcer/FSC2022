//Ejercicio para introducir datos en ficheros y luego recuperarlos
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

struct Estructura{
	int a;
	float b;
	char c;
};

int main(int argc, char *argv[]){
	//0.- Procesar argumentos
	if (argc < 2) {
		printf("Uso %s <fichero>\n", argv[0]);
		return -1;
	}

	//1. Declara un descriptor de fichero denominado fd
	int fd;

	//2. Crea y abre el fichero utilizando creat()
	if ((fd = creat(argv[1], 0666)) < 0) {
		perror("ha fallado la creacion del fichero\n");
		return 1;
	}

	//3. Escribe el valor de la variable entera x en el fichero
	int x = 7;
	if(write(fd,&x,sizeof(int))!= sizeof(int)){
		perror("error en write del valor de x");
		return 1;
	}	
		
	//4. Escribe el valor de un array de enteros en el fichero
	int array_enteros[4] = {0x00,0x01,0x02,0x03};
	if(write(fd,array_enteros,sizeof(int)*4)!= (sizeof(int)*4)){
		perror("error en write del array");
		return 1;
	}
	
	//5. Escribe el valor de una estructura en el fichero
	struct Estructura est;
	est.a = 1; est.b = 2.0; est.c = '3';
	if(write(fd,&est,sizeof(struct Estructura))!= sizeof(struct Estructura)){
		perror("error en write de la estructura");
		return 1;
	}
	
	//6. Cierra el fichero con close() y comprueba su valor de retorno
	if (close(fd) == -1){
		perror("error cerrando fichero");
		return 1;
	}
	
	
	return 0;
}