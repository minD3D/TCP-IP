/*
12131482_김민지 
컴퓨터 네트워크 실습 과제


쓰레드 관련
기말범위
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
  
	//뮤텍스 초기화
	pthread_mutex_init(&mutx, NULL);
	//소켓을 생성 
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	
	//서버 주소값 초기화	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	//초기화한 주소값을 bind함수를 통해 할당
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	
	//서버 소켓 수를 전달하여 연결대기
	//일반소켓을 서버소켓으로 만들어줌
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		//accept 함수를 통해 연결
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++]=clnt_sock; //클라이언트 소켓을 배열로 관리 
		//여러명의 사용자가 있기 때문

		pthread_mutex_unlock(&mutx);
	
		//클라이언트를 위한 쓰레드 각각 생성
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		
		//pthread datach는 블럭킹 되어있지 않아 연속적으로 호출가능 (조인은 블록킹 되어있음)
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}
	
void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];
	
	//하나 받으면 모든 클라이언트에 뿌려줌
	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		send_msg(msg, str_len);
	

	//뮤텍스 락으로 크리티컬섹션 보호
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)   // remove disconnected client
	//서버를위한 임의 주소로 메세지를 보내게 됨으로 한칸씩 왼쪽으로 옮긴다
	{
		if(clnt_sock==clnt_socks[i])
		{
			while(i <clnt_cnt-1)//기존코드에서 수정했음
			//FD의 0,1,2지정 되어있고 4번은 -----에사용됬으나
			//4번부터 채워넣어야 하므로 4번으로 한칸씩 옮겨주는 작업을 해주어야함
				clnt_socks[i]=clnt_socks[i+1];
				i++;
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}
void send_msg(char * msg, int len)   // send to all
{
	int i;
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)//모든 클라이언트에게 메세지를 보내주고 있음
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}
void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}