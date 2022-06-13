#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define T 100

// ESTADOS

#define INICIO 0
#define FICH1 1
#define FICH2 2
// EL ESTADO DE FIN NO HACE FALTA

//EVENTOS
#define SIG1 1 // SIGUSR1
#define SIG2 2 //SIGUSR2
#define SIG3 3 //SINGINT

int senal;

// tenemos tres señales

void espera_hijos()
{
    wait(0);
    signal(SIGCHLD, espera_hijos);
}

void m1()
{
    senal = SIG1;
}

void m2()
{
    senal = SIG2;
    if (signal(SIGUSR2, m2) == SIG_ERR)
    {
        perror(" manejador m2");
        exit(1);
    }
}

void m3()
{
    senal = SIG3;
    if (signal(SIGINT, m3) == SIG_ERR)
    {
        perror(" manejador m3");
        exit(1);
    }
}
int p[2];

/*void espera_evento2(int p_escritura)
{
   
  
}*/
// el espera evento
// lo que voy a hacer es que cuando recibe una señalcambia el evento
//cuando cambie el evento lo que hago es que envio un caracter differente e
// enn el caso que sea
int espera_evento1(int p_lectura)
{
    //el hijo lee de la pipe
    char c[1];
    if (read(p_lectura, c, 1) < 0)
    {
        perror("leer caracter");
        exit(1);
    }
    return atoi(c);
    
}

int main()
{
    pid_t pid = getpid();
    printf("PID = %d\n", pid);
    // señales
    if (signal(SIGUSR1, m1) == SIG_ERR)
    {
        perror(" manejador m1");
        exit(1);
    }

    if (signal(SIGUSR2, m2) == SIG_ERR)
    {
        perror(" manejador m2");
    }

    if (signal(SIGINT, m3) == SIG_ERR)
    {
        perror(" manejador m3");
        exit(1);
    }
    if (signal(SIGCHLD, espera_hijos) < 0)
    {
        perror("esperar al hijo ");
        exit(1);
    }

    int estado = INICIO;
    if (pipe(p) < 0)
    {
        perror("pipe");
        exit(1);
    }
    int evento;
    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        int fin = 0;
        if (close(p[1]) < 0)
        {
            perror("cerra modo escritura del hijo ");
        }
        // el hijo lee de la pipe

        while (fin == 0)
        {
            
            printf("Estamos en el estado %d\n", estado);
            switch (estado)
            {

            case INICIO:
                switch (evento)
                {
                case SIG1:
                    printf("entramos al evento %d\n", evento);
                    estado = FICH1;
                    printf("Transitando al estado %d\n", estado);
                    break;

                default:
                    printf("el evento esperado no corresponde %d\n", evento);
                    break;
                }
                break;
            case FICH1:
                switch (evento)
                {
                case SIG2:
                    printf("entramos al evento %d\n", evento);
                    estado = FICH2;
                    printf("Transitando al estado %d\n", estado);
                    break;

                default:
                    printf("el evento esperado no corresponde %d\n", evento);
                    break;
                }
                break;
            case FICH2:
                switch (evento)
                {
                case SIG3:
                    printf("entramos al evento %d\n", evento);
                    fin = 1;
                    printf("TERMINANDO....\n");
                    break;

                default:
                    printf("el evento esperado no corresponde %d\n", evento);
                    break;
                }
                break;

            default:
                printf("El estado nno coincide con ninnguno de los esperados %d\n", estado);
                break;
            } // ESTADO INNICIO
        }     //WHILE FINN
        exit(0);
    }
    else
    {
        //padre excribe por la pipe
        close(p[0]);
        int cont = 0;
        pause();
        espera_evento1(evento);
        switch (evento)
        {
        case SIG1:
        
            
                if (write(p[1], "1", 1) < 0)
                {
                    perror("escribir unno");
                    exit(1);
                }
               
            

            break;
        case SIG2:
            if (write(p[1], "2", 1) < 0)
            {
                perror("escribir dos");
                exit(1);
            }
            break;
        case SIG3:
            if (write(p[1], "3", 1) < 0)
            {
                perror("escribir tres");
                exit(1);
            }
            break;
        default:
            printf("evento no esperado\n");
            break;
        }
    }

    return 0;
}