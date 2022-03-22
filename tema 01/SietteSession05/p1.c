/* Escriba un programa en C que, implemente la función

   int checkMail(char * email, char * user, char * host); 

que devuelve
0: en caso de que email sea una dirección de correo electrónico bien formada. 
En nuestro caso, una dirección de correo electrónico está bien formada si 
tiene formato [user]@[host], donde user y host son dos cadenas de texto de 
longitud mayor que cero.
-1: cuando en email no aparece el carácter @.
1: cuando en email las cadenas user o host están vacías. Por ejemplo: @lcc.uma.es o flv@.


Implemente también el programa principal main() de forma que la dirección de correo 
electrónico se reciba como argumento de la función.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkMail(char * email, char * user, char * host); 

int main(int argc, char * argv[])
{
    if (argc<2){
        printf("Debe entrar: %s email@host.pp\n",argv[0]);
        return -1;
    }
    printf("Se han pasado %d parametros y el primer parametro es %s\n",argc,argv[1]);
    char * email = (char*)malloc(50*sizeof(char));
    strcpy(email,argv[1]);
    char * usuario = (char*)malloc(50*sizeof(char));
    char * hostname = (char*)malloc(50*sizeof(char));

    printf ("Estado del correo: %d\n",checkMail(argv[1],usuario,hostname));
    printf("Usuario: %s Hostname: %s\n ", usuario, hostname);
}

int checkMail(char * email, char * user, char * host){
    char * control; //puntero con el control de @
    control=strchr(email,'@');

    if (control == NULL ) return -1; //no existe @
    if (control == email) return 1; //es la primera pos o sea ni hay nombre
    if (control == email + strlen(email)-1) return 1;//es la ultima
    memcpy(user, email,control-email);
    memcpy(host, control+1,strlen(email));
    return 0;
}
