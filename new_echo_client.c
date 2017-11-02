/*
#3 example
ex2에서 패킷이 완전히 전달됐는지 확인하는 부분이 추가됨
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
	int str_len,recv_len,recv_cnt;
	struct sockaddr_in serv_addr;

	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr)); //����ü ����
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//���⿡ ����� �����Ǹ� �־��ش� ������ �ּҸ� �ַܼ� �Է¹���
	serv_addr.sin_port = htons(atoi(argv[2]));//ȣ��Ʈ �ּҸ� ��Ʈ��ũ �ּҷ�

											  //���� ��û
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	else
        puts("Connected..........");
        
 
	while (1)
	{
		//�Է� ��û �޼����� ������ ��Ʈ���� �Է��Ѵ�
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		//Q�� �Է��ߴٸ� ������.
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
        
        //잘 보내졌는지 확인하는 매커니즘
        //실습 코드시험 나올확률 있음!!!!!!!!!!!!!!!!!!!!!!!!!!
        str_len=write(sock, message, strlen(message));
        recv_len=0;
        while(recv_len<str_len){
            recv_cnt=read(sock,&message[recv_len],BUF_SIZE-1);
            if(recv_cnt==-1)
            error_handling("read() error!");
            recv_len+=recv_cnt;
        }

        message[recv_len] = 0;
		write(sock, message, strlen(message));//sock�� �޼����� �Է��Ѵ�
        
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