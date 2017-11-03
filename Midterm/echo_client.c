/*
12131482_김민지
컴퓨터 네트워크 실습 과제
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
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_addr;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	//소켓생성하여 소켓에 할당된 숫자를 sock에 저장
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");
	//소켓을 이용해 연결할 상대의 주소를 초기화
	memset(&serv_addr, 0, sizeof(serv_addr)); //구조체 비우기
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//여기에 상대의 아이피를 넣어준다 상대방의 주소를 콘솔로 입력받음
	serv_addr.sin_port = htons(atoi(argv[2]));//호스트 주소를 네트워크 주소로 변환

	//연결 요청
	//sock의 숫자에 대해 위의 초기화한 주소를 연결
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	else
		puts("Connected..........");


	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		
		//파일 디스크립터 소켓을 이용하여 메세지를 읽고 쓴다
		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE - 1);
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

