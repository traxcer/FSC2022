#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

# define T 255 

int main()
{
    char buffer[T];
    int leidos;
    int fd= open("texto.txt",O_RDONLY);
        if (fd < 0)
        {
            perror("open");
            exit(1);
        }
    leidos =read(fd,buffer,T-1);
    while (leidos>0)
        {
        buffer[leidos] = '\0';
        printf("%s",buffer);
        leidos =read(fd,buffer,T-1);
        }
    close(fd);
}