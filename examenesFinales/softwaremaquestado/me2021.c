#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define IDLE 0
#define EJECUTANDO 1
#define ESPERANDD 2

#define COMANDO 0
#define CONT 1
#define STOP 2
#define FIN 3

#define T 100
int evento_recibido;

int espera_evento(char *mensaje,int fd){

   
    int leidos;
     
    leidos=read(fd,mensaje,T);
    printf("leidos=%d\n",leidos);
    if(leidos<0){
        perror("read fifo");
        close(fd);
        exit(1);
    }
    mensaje[leidos-1]='\0';
    

    if((mensaje[0]=='/')&&(mensaje[1]=='c')&&(mensaje[2]=='m')&&(mensaje[3]=='d')){
        return COMANDO;
    }else if(strcmp(mensaje,"/stop")==0){
        return STOP;
    }else if(strcmp(mensaje,"/cont")==0){
        return CONT;
    }else if(strcmp(mensaje,"/fin")==0){
        return FIN;
    }
   
}
/*void m1(int signo){
    evento_recibido=STOP;
    if(signal(SIGSTOP,m1)==SIG_ERR){
        perror("señal m1");
        exit(1);
    }
}*/

void m2(int signo){
    evento_recibido=CONT;
    if(signal(SIGCONT,m2)==SIG_ERR){
        perror("señal m2");
        exit(1);
    }
}
/*void m3(int signo){
    evento_recibido=FIN;
    if(signal(SIGCONT,m3)==SIG_ERR){
        perror("señal m3");
        exit(1);
    }
}*/
char *strEvento(int evento){
    switch(evento){
        case COMANDO:
            return "COMANDO";
        case STOP:
            return"STOP";
        case CONT:
            return "CONT";
        case FIN:
            return "FIN";
        default:
        printf("estado nno valido\n");
        
    }
}
char *strEstado(int estado){
    switch(estado){
        case IDLE:
            return "IDLE";
        case EJECUTANDO:
            return"EJECUTANDO";
        case ESPERANDD:
            return "ESPERANDO";     
        default:
        printf("estado nno valido\n");
    }
}
void espera_hijo (int s){
    wait(0);
    signal(SIGCHLD,espera_hijo);
}

int main(){
    int estado=IDLE;
    int evento;
    int fin=0;
    char mensaje[T]; 
    int retorno;
    //inicializacion de las señales
    /*if(signal(SIGSTOP,m1)==SIG_ERR){
        perror("sigstop m1");
        exit(1);
    }*/
    int fd;
    if((fd=open("fifo_me",O_RDONLY))<0){
        perror("abrir fifo");
        exit(1);
    }
    if(signal(SIGCONT,m2)==SIG_ERR){
        perror("sigcont m2");
        exit(1);
    }
    if(signal(SIGCHLD,espera_hijo)==SIG_ERR){
        perror("sigchld");
        exit(1);
    }
   /* if(signal(SIGKILL,m3)==SIG_ERR){
        perror("sigkill m3");
        exit(1);
    }*/
    pid_t pid;
    while(fin==0){
        printf("ESTADO (%d) Esperando a que la fifo inntroduzca algo...\n",estado);
        pid_t pid=getpid();
         
        evento=espera_evento(mensaje,fd);
        printf("mensaje= %s\n",mensaje);
      
        switch(estado){
            case IDLE:
                switch (evento)
                {
                case COMANDO:
                    //ahora leo de la fifo 
                    estado=EJECUTANDO;
                    printf("Transito %s\n",strEstado(estado));
                    
                    char *p =strstr(mensaje," ");
                    *p='\0';
                    p++;
                    
                    printf("comando = %s\n",p);
                    //he guardado en comando lo que hay que pasarle a sistem
                    if((pid=fork())<0){
                        perror("ffork");
                        exit(1);
                    }else if(pid==0){
                        // codigo hijo 
                         
                        int s=execl("/bin/bash","antoino","-c",p,NULL);
                        if(s<0){
                            perror("system");
                            exit(1);
                        }
                        
                         
                    }
                    //el apdress
                    
                    break;
                default:
                    printf("evento no esperado en IDLE %d\n",evento);
                    fin=1;
                    break;
                }
            break;
            case EJECUTANDO:
                switch (evento)
                {
                case STOP:
                    estado=ESPERANDD;
                    printf("Transito %s\n",strEstado(estado));
                    retorno =kill(pid,SIGSTOP);
                    if(retorno<0){
                        perror("sigstop");
                        exit(1);
                    }
                    wait(0);
                    break;
                
                default:
                    printf("evento no esperado en stop %d\n",evento);
                    fin=1;
                    
                }
                break;
            case ESPERANDD: 
                switch (evento)
                {
                case FIN:
                    estado=IDLE;
                    printf("Transito %s\n",strEstado(estado));
                    retorno =kill(pid,SIGKILL);
                    if(retorno<0){
                        perror("sigkill");
                        exit(1);
                    }
                    break;
                case CONT:
                    estado=EJECUTANDO;
                    printf("Transito %s\n",strEstado(estado));
                    retorno =kill(pid,SIGCONT);
                    if(retorno<0){
                        perror("sigcont");
                        exit(1);
                    }
                    break;
                default:
                    printf("evento no esperado en IDLE %d\n",evento);
                    fin=1;
                    
                }

            break;

            default:
            printf("estado no esperado %d\n",estado);
            fin=1;
                
        }
    }


    //tenemos que cerrar la fifo 
    
    return 0;
}