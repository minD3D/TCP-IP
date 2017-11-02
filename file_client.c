/*
7주차 실습
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sd;
    FILE *fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_addr;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
    }
    
    fp= fopen("receive.dat", "wb");
    
	//소켓생성하여 소켓에 할당된 숫자를 sock에 저장
	sd = socket(PF_INET, SOCK_STREAM, 0);
    
    //소켓을 이용해 연결할 상대의 주소를 초기화
	memset(&serv_addr, 0, sizeof(serv_addr)); //구조체 비우기
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//여기에 상대의 아이피를 넣어준다 상대방의 주소를 콘솔로 입력받음
	serv_addr.sin_port = htons(atoi(argv[2]));//호스트 주소를 네트워크 주소로 변환

	//연결 요청
	//sock의 숫자에 대해 위의 초기화한 주소를 연결
  
	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	
	while((read_cnt=read(sd, buf, BUF_SIZE ))!=0)
		fwrite((void*)buf, 1, read_cnt, fp);
	
	puts("Received file data");
	write(sd, "Thank you", 10);
	fclose(fp);
	close(sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
