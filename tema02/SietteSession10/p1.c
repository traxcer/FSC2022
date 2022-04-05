/*
Implemente un programa C con tres procesos que ejecutan concurrentemente: 
un padre y dos hijos de forma indefinida. 
El proceso hijo1, cada 2 segundos, hace dos cosas: 
envía la señal SIGUSR1 al proceso padre y muestra la cadena Soy el 
proceso hijo1 con pid = 7763: papá, despierta a mi hermano. 

El proceso padre atiende SIGUSR1 enviando esta señal al hijo2 e imprimiendo 
por pantalla: Soy el proceso padre con pid = 7760: ya voy!. 

El proceso hijo2, al recibir la señal, muestra la cadena: 
Soy el proceso hijo2 con pid = 7764: ya estoy despierto. 
Los pid mostrados en el ejemplo son arbitrarios, y se deben obtener usando 
la función getpid().

Nota 1: por razones "ajenas a mi voluntad", las pruebas no funcionan cuando 
se usa printf(). Para que se pasen correctamente, hay que usar la función 
write() para escribir por pantalla. Así, el equivalente de

printf("Soy proceso hijo1 con PID=%d: papá, despierta a mi hermano\n",getpid());
sería:

#define T 256
char msg[T];
sprintf(msg,"Soy proceso hijo1 con PID=%d: papá, despierta a mi hermano\n",getpid());
write(1,msg,strlen(msg));

Nota 2: El uso de la función kill(pid_t pid, int signo) saca un warning, 
independientemente de las librerías incluidas. 
Hay dos formas de solucionarlo: compilar sin -std=c99, que es lo que se hace 
en Siette, o bien incluir el siguiente #define _POSIX_SOURCE al principio del 
código.
*/

