 #include <sys/types.h>
 #include <unistd.h>
 #include <stdio.h>
 #include <time.h>
 #include <stdlib.h>
 #include <string.h>
 #include <errno.h>
 #include <signal.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <sys/time.h>

//Defino ESTADOS
#define ESTADO_OFF 0
#define ESTADO_ON 1

//Defino EVENTOS
#define EVENTO_CONTROL 0
#define EVENTO_TIMEOUT 1
#define EVENTO_CONTADOR 0

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

int p[2]; //tuberia eventos

void ev_control (int s){ //encola evento en la pipe
    int evento= EVENTO_CONTROL;
    if(writen(p[1],&evento,sizeof(int))!=sizeof(int))
        {
            perror("ev_control: writen");
            exit (-1);
        }
    signal(SIGINT, ev_control);
}

void ev_timeout (int s){
    int evento= EVENTO_TIMEOUT;
    if(writen(p[1],&evento,sizeof(int))!=sizeof(int))
        {
            perror("ev_timeout: writen");
            exit (-1);
        }
    signal(SIGALRM, ev_timeout);
}

int ev_espera(){
    int evento;
    if(readn(p[0],&evento,sizeof(int))!=sizeof(int))
        {
            perror("ev_espera: readn");
            exit (-1);
        }
    return evento;
}




/*================
 PROGRAMA PRINCIPAL
==================*/
int main(){

    int contador=1;
    int estado = ESTADO_OFF; //Estado inicial
    int evento;

    // FIFO, Apertura <-
    int fd_fifo= open("fifo_ej1", O_WRONLY);
    if(fd_fifo<0){
        perror("Error apertura fifo");
        exit(1);
    }
    
    //Estructuras temporizador
    struct timeval inicio, intervalo, cero;
        inicio.tv_sec = 1;
        inicio.tv_usec = 0;
        intervalo.tv_sec = 1;
        intervalo.tv_usec = 0;
        cero.tv_sec = 0;
        cero.tv_usec = 0;
    struct itimerval temporizador, it_cero;
        it_cero.it_value = cero;
        it_cero.it_interval = cero;
        temporizador.it_value = inicio;
        temporizador.it_interval = intervalo;

    //Armo la señal para el evento Crtl+C
    if(signal(SIGINT,ev_control)== SIG_ERR){
        perror("ev_control");
        exit(1);
    }
    //Armo la señal para el evento Temporizador
    if(signal(SIGALRM,ev_timeout)== SIG_ERR){
        perror("ev_timeout");
        exit(1);
    }
    //Creo la tuberia
    if(pipe(p)<0){
        perror("Error creando pipe");
        exit(1);
    }

 //Bucle principal
while(1){
    evento = ev_espera();
    switch(estado){
        case ESTADO_OFF:
            switch(evento){
                case EVENTO_CONTROL:
                    estado = ESTADO_ON;
                    printf("ESTADO: Transitado de OFF a ON\n");
                    setitimer(ITIMER_REAL, &temporizador, NULL);
                    contador = 1;
                break;
            }
            break;
        case ESTADO_ON:
            switch(evento){
                case EVENTO_TIMEOUT:
                    signal(SIGINT, SIG_IGN);
                    writen(fd_fifo,"Watchdog!\n",10);
                    if(contador == 3){
                        estado=ESTADO_OFF;
                        printf("ESTADO: Transitado de ON a OFF\n");
                        setitimer(ITIMER_REAL, &it_cero, NULL);
                        signal(SIGINT,ev_control);
                    }
                    contador ++;
                    break;
            break;
            }
        } // estado
    } //while

} //main       

