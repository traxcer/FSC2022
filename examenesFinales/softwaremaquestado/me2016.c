#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#define LISTO 0
#define RETRASO_TX 1
#define ESPERA_CTS 2
#define ESPERA_RX_FINISH 3
#define RETRASO_RTS 4
#define ESPERA_OFF 5


#define MANDA_MENSAJE 0
#define EX_TEMP 1
#define CTS 2
#define TX_FIN 5
#define CTS_OFF 6

int estado, evento;
int p[2];

void m1()
{
    u_int8_t e = EX_TEMP;
    write(p[1], &e, 1);
    signal(SIGALRM, m1);
}
void m2()
{
    u_int8_t e = MANDA_MENSAJE;
    write(p[1], &e, 1);
    signal(SIGINT, m2);
}
void m3()
{
    u_int8_t e = CTS;
    write(p[1], &e, 1);
    signal(SIGTERM, m3);
}
void m4()
{
    u_int8_t e = TX_FIN;
    write(p[1], &e, 1);
    signal(SIGUSR1, m4);
}
void m5()
{
    u_int8_t e = CTS_OFF;
    write(p[1], &e, 1);
    signal(SIGUSR2, m5);
}

int espera_evento()
{
    u_int8_t b;
    read(p[0], &b, 1);

    printf("Estado que le llega a la pipe %d", b);
    return b;
}

int main()
{
    pid_t pid = getpid();
    printf("pid= %d\n", pid);
    int estado = LISTO;
    int fin = 0;

    struct timeval t, t_zero;

    t_zero.tv_sec = 0;
    t_zero.tv_usec = 0;

    struct itimerval inter;

    inter.it_interval = t_zero;
    inter.it_value = t_zero;

    signal(SIGALRM, m1);
    signal(SIGINT, m2);
    signal(SIGTERM, m3);
    signal(SIGUSR1, m4);
    signal(SIGUSR2, m5);

    pipe(p);
    while (fin == 0)
    {
        evento = espera_evento();
        switch (estado)
        {
        case LISTO:
            switch (evento)
            {
            case MANDA_MENSAJE:
                // activamos un temporizador
                printf("Estoy en list y activo el temporizador\n");
                t.tv_sec = 1;
                t.tv_usec = 500000;
                inter.it_value = t;
                setitimer(ITIMER_REAL, &inter, NULL);
                estado = RETRASO_TX;
                break;

            default:
                printf("evento no esperado %d\n", evento);
                break;
            }

            break;
        case RETRASO_TX:
            switch (evento)
            {
            case EX_TEMP:
                t.tv_sec = 3;
                t.tv_usec = 300000;
                inter.it_value = t;
                printf("RTS activa\n");
                setitimer(ITIMER_REAL, &inter, NULL);
                estado = ESPERA_CTS;
                break;

            default:
                printf("evento no esperado %d\n", evento);
                break;
            }

            break;

        case ESPERA_CTS:
            switch (evento)
            {
            case CTS:
                printf("estoy en cts\n");
                t.tv_sec = 0;
                t.tv_usec = 0;
                inter.it_value = t;
                setitimer(ITIMER_REAL, &inter, NULL);
                printf("CTS activa");
                estado = ESPERA_RX_FINISH;
                break;
            case EX_TEMP:

                printf("ERROR CTS no activa\n ");

                fin = 1;
                break;
            default:
                printf("evento no esperado %d\n", evento);
                break;
            }
            break;
        case ESPERA_RX_FINISH:
            switch (evento)
            {
            case TX_FIN:
                estado = RETRASO_RTS;
                t.tv_sec = 1;
                t.tv_usec = 500000;
                inter.it_value = t;
                setitimer(ITIMER_REAL, &inter, NULL);
                break;

            default:
                printf("evento no esperado %d\n", evento);
                break;
            }
            break;
        case RETRASO_RTS:
            switch (evento)
            {
            case EX_TEMP:
                printf("RTS desactivada\n");
                estado = ESPERA_OFF;
                t.tv_sec = 4;
                t.tv_usec = 200000;
                inter.it_value = t;
                setitimer(ITIMER_REAL, &inter, NULL);
                break;
            default:
                printf("evento no esperado %d\n", evento);
                break;
            }
            break;
        case ESPERA_OFF:
            switch (evento)
            {
            case CTS_OFF:
                t.tv_sec = 0;
                t.tv_usec = 0;
                inter.it_value = t;
                setitimer(ITIMER_REAL, &inter, NULL);
                printf("CTS desactivada\n");
                break;

            case EX_TEMP:

                printf("ERROR CTS no desactivada\n");

                fin = 1;
                break;
            default:
                printf("evento no esperado %d\n", evento);
                break;
            }
            break;
        default:
            printf("estado no esperado %d\n", estado);
            break;
        }
    }
    close(p[0]);
    close(p[1]);
    return 0;
}
