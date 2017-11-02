#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len;
    
    if(argc!=3){
        printf("ip num , port num",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_STREAM,0);//SOCKET(도메인 -> IPv4, 타입, 프로토콜)
    if(sock==-1)
        error_handling("socket error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;//IPV4어드레스 페밀리 의미
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);//빅에디안 변환된 32비트 정수(네트워크)
    serv_addr.sin_port=htons(atoi(argv[2]));
    
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect error");

        
    str_len=read(sock, message, sizeof(message)-1);
    if(str_len==-1)
        error_handling("read error");

    printf("mindimindi server:%s \n", message);
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs( message,stderr);
    fputc('\n', stderr);
    exit(1);
}