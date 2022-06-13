#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>

#define IDLE 0
#define CIPH1 1
#define CIPH2 2
#define FIN 3

#define DATOS 0
#define CIERRE 1
#define STERM 2
#define SR1 3
#define SINT 4

#define T 100

int p[2];

ssize_t read_n(int sd, void *b,size_t n){
    ssize_t a_leer=n;
    ssize_t leidos;
    ssize_t total_leidos=0;

    do{
        errno=0; 
        leidos= write(sd,b+total_leidos,a_leer);
        if(leidos>=0){
            total_leidos+=leidos;
            a_leer-=leidos;
        }

    }while(((leidos>0)&&(total_leidos<n))||(errno==EINTR));

    if(total_leidos>0){
        return total_leidos;
    }else{
        return leidos;
    }

}

ssize_t write_n(int sd, void *b,size_t n){
    ssize_t a_escribir=n;
    ssize_t escritos;
    ssize_t total_escritos=0;

    do{
        errno=0; 
        escritos= write(sd,b+total_escritos,a_escribir);
        if(escritos>=0){
            total_escritos+=escritos;
            a_escribir-=escritos;
        }

    }while(((escritos>0)&&(total_escritos<n))||(errno==EINTR));

    if(total_escritos>0){
        return total_escritos;
    }else{
        return escritos;
    }

}
void m2(int signo){
    write(1,"LLEGA SIGTERM\n",14);
    int even=STERM;
    if(write_n(p[1],&even,sizeof(int))!=sizeof(int)){
        perror("enviar STERM manejador");
        exit(1);
    }
    signal(SIGUSR1,m2);
}

void m1(int signo){
    write(1,"LLEGA SIGINT\n",14);
    int even=SINT;
    int w=write_n(p[1],&even,sizeof(int));
    printf("w=%d\n",w);
    if(w!=sizeof(int)){
        perror("enviar SINT manejador");
        exit(1);
    }
    signal(SIGUSR1,m1);
}
void m3(int signo){
    write(1,"LLEGA SIGUSR1\n",14);
    int even=SR1;
    if(write_n(p[1],&even,sizeof(int))!=sizeof(int)){
        perror("enviar sgr1 manejador");
        exit(1);
    }
    signal(SIGUSR1,m3);
}

char *strEvento(int evento){
    switch(evento){
        case DATOS:
            return "DATOS";            
        case STERM:
            return "SIGTERM";
        case CIERRE:
            return "CIERRE";
        case SR1:
            return "SIGUSR1";
        case SINT: 
            return "SIGINT";    
    }
}

char *strEstado(int estado){
    switch(estado){
        case IDLE:
            return "IDLE";            
        case FIN:
            return "FIN";
        case CIPH1:
            return "CIPH1";
        case CIPH2:
            return "CIPH2";
           
    }
}



int maxi(int anterior,int posterior){
    if(anterior>posterior){
        return anterior;
    }else{
        return posterior;
    }
}

void Copia(fd_set *destino,fd_set *origen,int mas_uno){
    FD_ZERO(destino);
    for(int i=0;i<mas_uno;i++){
        if(FD_ISSET(i,origen)){
            FD_SET(i,destino);
        }
    }
}
int fi;
int espera_evento(){
    // se implementa mediante un select el cual 
    //esta esperando tanto a la fifo como a la pipe

    
    if((fi=open("fifo_fsc",O_RDONLY))<0){
        perror("open fifo");
    }
    fd_set cd,cd_n;
    FD_ZERO(&cd);
    FD_SET(fi,&cd);
    FD_SET(p[0],&cd);
    int maximo=maxi(p[0],fi);
    while(1){
        Copia(&cd_n,&cd,maximo+1);
        int sel;
        if((sel=select(maximo+1,&cd_n,NULL,NULL,NULL))<0){
            if(errno==EINTR){
                errno=0;
                continue;
            }else{
                perror("select");
                close(fi);
                
                exit(1);
            }
        }
        if(FD_ISSET(fi,&cd_n)){
            return DATOS;
        } 
         if(FD_ISSET(p[0],&cd_n)){
            int evento_recibido;
            int l=read_n(p[0],&evento_recibido,sizeof(int));
            printf("leidos de la pipe : %d\n",l);
            if(l!=sizeof(int)){
                perror("lee de pipe");
                exit(1);
            }
            return evento_recibido;
        }

               
    }
}





int main(){

    if(signal(SIGINT,m1)==SIG_ERR){
        perror("siginnt");
        exit(1);
    }
    if(signal(SIGUSR1,m3)==SIG_ERR){
        perror("sigusr1");
        exit(1);
    }
    if(signal(SIGTERM,m2)==SIG_ERR){
        perror("siGterm");
        exit(1);
    }

    if(pipe(p)<0){
        perror("pipe");
        exit(1);
    }

    int estado=IDLE;
    int evento;
    int fin=0;
    int leidos;
    char dat[T];
    while(fin==0){
        printf("Estado = %s. Esperando evento\n",strEstado(estado));
        evento=espera_evento();
        printf("Evento= %s\n",strEvento(evento));
        pid_t pid=getpid();
        printf("pid= %d\n",pid);
        switch (estado)
        {
        case IDLE:
            switch (evento)
            {
            case DATOS:
                // SE LE DE LA FIFO Y SE IMPRIME POR PANTALLA 
                
                if((leidos=read(fi,dat,T))<0){
                    perror("leer de fifo");
                    exit(1);
                }
                if(leidos==0){
                    //cierre del canal 
                    estado=FIN;
                    break;
                }
                dat[leidos]='\0';
                if(write_n(1,dat,leidos-1)!=(leidos-1)){
                    perror("escribir lo datos");
                    exit(1);
                }
                break;
            case SINT:
                break;
            case SR1:
                evento=CIPH1;
                printf("Evento= %s\n",strEvento(evento));
                break;
            case STERM:
                estado=FIN;
                printf("Evento= %s\n",strEvento(evento));
                break;
            default:
                printf("evento no esperado\n");
                break;
            }
            break;//IDLE
        case FIN:
            printf("FIN DE LA MAQUINA\n");
            fin=1;
            break;//FIN
        case CIPH1: 
            switch (evento)
            {
            case STERM:
                estado=FIN;
                printf("Evento= %s\n",strEvento(evento));
                break;
            case SR1:
                estado=CIPH2;
                printf("Evento= %s\n",strEvento(evento));
                break;
            case DATOS:
                 if((leidos=read(fi,dat,T))<0){
                    perror("leer de fifo");
                    exit(1);
                }
                if(leidos==0){
                    //cierre del canal 
                    estado=FIN;
                    break;
                }
                dat[leidos]='\0';
                for(int i=0;i<leidos;i++){
                    dat[i]+=1;
                }
                if(write_n(1,dat,leidos-1)!=(leidos-1)){
                    perror("escribir lo datos");
                    exit(1);
                }
                break;
            case SINT:
                estado=IDLE;
                printf("Evento= %s\n",strEvento(evento));
                break;

            default:
                break;
            }

            break;//CIPH1
        case CIPH2:
            break;
        
        default:
            break;
        }// switch estado
    }//while fin

    return 0;
}

