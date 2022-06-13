
//Defino ESTADOS
#define ESTADO_OFF 0
#define ESTADO_ON 1
//Defino EVENTOS


//Funciones de la Asignatura readn y writen
//===============================================
ssize_t readn(int fd, void* datos,size_t n){
    ssize_t leidos=0;
    size_t intento=n;
    size_t totalLeidos=0
    char * p=(char*) datos;

    do{
        errno=0;
        leidos=read(fd, p+totalLeidos,intento);
        if (leidos>0){
            totalLeidos+= leidos;
            intento -=leidos;
        }
        
    }  while(((leido>0)&&(totalLeidos<n))||errno ==EINTR);

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
    size_t totalEscritos=0
    char *p=(char*) datos;

    do{
        errno=0;
        
        escritos=write(fd, p+totalLeidos,intento);
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


