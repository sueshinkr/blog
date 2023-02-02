// peek_send.cpp

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

void	error_handling(char *message);

int		main(int argc, char *argv[])
{
	int					sock;
	struct sockaddr_in	recv_adr;

	if(argc != 3)
	{
		cout << "Usage : " << argv[0] << " <IP> <port>\n";
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recv_adr, 0, sizeof(recv_adr));
	recv_adr.sin_family = AF_INET;
	recv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	recv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
		error_handling("connect() error!");
	
	write(sock, "123", strlen("123"));
	close(sock);
	return 0;
}

void	error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

