/*
중간 범위였지만
사용자의 입력과 소켓으로부터 입력으로 나누어
셀렉트 서버와 같이 구현하는 과제로 나올 예정


수정전\


기말 나올 수 있음
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

int main(int argc, char *argv[])
{
	int sock;
	//pid_t pid;
	fd_set temp;////////////////
	char buf[BUF_SIZE];
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);  
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	while(1){
		//넌블록 함수이므로 시간설정
		//카피 리드 등 fd 복사가 일어나야 한다

		select();

		//if문으로 어떤 파일에 변화가 있는지 확인
		if(FD_isset())//사용자로부터 입력 fd 0번의 변화
			write_routine();
		if(FDisset())//소켓에 fd변화가 있는지
			read_routine();
	}
	//pid=fork();
	//if(pid==0)
		//write_routine(sock, buf);
	//else 
		//read_routine(sock, buf);

	close(sock);
	return 0;
}

//내부적으로 변화가 필요
void read_routine(int sock, char *buf)
{
		int str_len=read(sock, buf, BUF_SIZE);
		if(str_len==0)
			return;

		buf[str_len]=0;
		printf("Message from server: %s", buf);
}


void write_routine(int sock, char *buf)
{
		fgets(buf, BUF_SIZE, stdin);
		if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{	
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf));	
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}