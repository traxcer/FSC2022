/* Mostrar el tamaño de una estructura
*/
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char *argv[])
{
    struct mis_datos_s {
        char dat_char[32];
        short int dat_sint[20];
        int dat_int[10];
        long dat_long;
        double dat_double[10];
    };

    struct mis_datos_s primer_dato = {};
    struct mis_datos_s varios_datos[10] = {};

    printf ("<mis_datos_s> tiene un tamaño de %zd bytes\n", sizeof (struct mis_datos_s));
    printf ("<primer_dato> tiene un tamaño de %zd bytes\n", sizeof (primer_dato));
    printf ("<varios_datos[0]> tiene un tamaño de %zd bytes\n", sizeof (varios_datos[0]));
    printf ("<varios_datos> tiene un tamaño de %zd bytes\n", sizeof (varios_datos));

    printf ("Numero de elementos en <varios_datos>: %zd\n",
            sizeof (varios_datos) / sizeof (varios_datos[0]));

    exit (EXIT_SUCCESS);
}