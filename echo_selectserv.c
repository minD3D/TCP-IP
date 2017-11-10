/*
fd_set[]의 번호

connected client: 4
connected client: 5
이렇게 뜨는데 3은 서버소켓이기 때문!
0 표준입력
1 표준출력
2 에러
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(char *buf);

int main(int argc, char *argv[]){

    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr,clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max,str_len,fd_num,i;
    char buf[BUF_SIZE];
    if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    fd_max=serv_sock;

    while(1){
        cpy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        // fd_max+1은 갯수를 넣어야하기 때문에 파일디스크립터 최대값 +1 이 총 갯수        
        if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1)//select는 변경된 값의 갯수를 리턴한다
            break;

        for(i=0; i<fd_max+1; i++){//모든 파일 디스크립터 안의 값이 1인지 확인한다.[0/0/0/0/1]로 셋되어 있을것.
            if(FD_ISSET(i,&cpy_reads)){
                if(i==serv_sock){//서버소켓 위치인 경우
                    adr_sz=sizeof(clnt_adr);
                    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_adr,&adr_sz);
                FD_SET(clnt_sock,&reads);
                if(fd_max<clnt_sock)
                    fd_max=clnt_sock;
                
                printf("connected client: %d \n", clnt_sock);
                }
                else{//클라이언트 소켓 파일디스크립터가 1인경우
                    str_len=read(i,buf,BUF_SIZE);
                    if(str_len==0){
                        FD_CLR(i,&reads);
                        close(1);
                        printf("closed client: %d \n",i);
                    }
                    else{
                        write(i,buf,str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
	return 0;
}


void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}