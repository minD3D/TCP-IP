#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
/*
7주차 실습
*/
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sd, clnt_sd;
    FILE * fp;
	char message[BUF_SIZE];
	int read_cnt;
	
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	//서버 주소값 초기화
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	//초기화한 주소값을 bind함수를 통해 할당
    bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
    listen(serv_adr,5);
    
    clnt_adr_sz=sizeof(clnt_adr);
    clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    
    while(1){
        read_cnt=fread((void*)buf, 11,BUF_SIZE,fp);
        if(read_cnt<BUF_SIZE){
            write(clnt_sd,buf,read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
    }
    
    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd,buf, BUF_SIZE);
    printf("message from client : %s \n", buf);

    fclose(fp);
    close(clnt_sd) close(serv_sd);
    return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
