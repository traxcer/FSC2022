#include "unp.h"

int main(int argc, char* argv[]){
    int sockfd, n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;

    if (argc!=2)
        printf("usar:%s <IPaddress>",argv[0]);
        exit 0;
    
    if ( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("Socket");
        exit -1;
        }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(13); /*day time server port 13*/
    
    if(inet_pton(AF_INET, argv[1],&servaddr.sin_addr<= 0)){
        printf("inet_pton error for %s", argv[1]);
        exit -1;
        }
    
    if(connect(sockfd,(SA *)&servaddr, sizeof(servaddr)) < 0){
        perror("Connect");
        exit -1;
        }
    
    while ( (n=read(sockfd, recvline, MAXLINE)>0)){
        recvline[n] = 0;
        if (fputs(recvline, stdout)==EOF){
            perror("fputs error");
            exit -1;
        }   
    if (n<0)
        perror("read");
    }
    return 0;
}