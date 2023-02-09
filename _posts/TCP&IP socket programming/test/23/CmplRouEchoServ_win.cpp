// CmplRouEchoServ_win.cpp

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUF_SIZE 1024
void CALLBACK	ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK	WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void			ErrorHandling(char *message);

typedef struct
{
	SOCEKT	hClntSock;
	char	buf[BUF_SIZE];
	WSABUF	wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

int main(int argc, char *argv[])
{
	WSADATA			wsaData;
	SOCKET			hLisnSock, hRecvSock;
	SOCKADDR_IN		lisnAdr, recvAdr;
	LPWSAOVERLAPPED	lpOvLp;
	DWORD			recvBytes;
	LPPER_IO_DATA	hbInfo;
	int				mode = 1, recvAdrSz, flagInfo = 0;

	if(argc != 2)
	{
		cout << "Usage : " << argv[0] << " <port>\n";
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ioctlsocket(hLisnSock, FIONBIO, &mode);

	memset(&lisnAdr, 0, sizeof(lisnAdr));
	lisnAdr.sin_family = AF_INET;
	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	lisnAdr.sin_port = htons(atoi(argv[1]));

	if (::bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (::listen(hLisnSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
	
	recvAdrSz = sizeof(recvAdr);
	while (1)
	{
		SleepEx(100, TRUE);	// for alertable wait state
		hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);
		if (hRecvSock == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			else
				ErrorHandling("accept() error");
		}
		cout << "Client connected....." << endl;

		lpOvLp = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
		memset(lpOvlp, 0, sizeof(WSAOVERLAPPED));

		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		hbInfo->hClntSock = (DWORD)hRecvSock;
		(hbINfo->wsaBuf).buf = hbInfo->buf;
		(hbInfo->wsaBuf).len = BUF_SIZE;

		lpOvLp->hEvent = (HANDLE)hbInfo;
		WSARecv(hRecvSock, &(hbInfo->wsaBuf), 1, &recvBytes, &flagInfo, lpOvLp, ReadCompRoutine);
	}
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	WSACleanup();
	return 0;
}

void CALLBACK	ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	LPPER_IO_DATA	hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	SOCEKT			hSock = hbInfo->hClntSock;
	LPWSABUF		bufInfo = &(hbInfo->wsaBuf);
	DWORD			sentBytes;

	if (szRecvBytes == 0)
	{
		closesocket(hSock);
		free(lpOverlapped->hEvent);
		free(lpOverlapped);
		cout << "Client disconnected.....\n";
	}
	else	// echo
	{
		bufInfo->len = szRecvBytes;
		WSASend(hSock, bufInfo, 1, &sentBytes, 0, lpOverlapped, WriteCompRoutine);
	}
}

void CALLBACK	WriteCompRoutine(DWORD dwError, DWORD szSendBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	LPPER_IO_DATA	hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	SOCEKT			hSock = hbInfo->hClntSock;
	LPWSABUF		bufInfo = &(hbInfo->wsaBuf);
	DWORD			recvBytes;
	int				flagInfo = 0;

	WSARecv(hSock, bufInfo, 1, &recvBytes, &flagInfo, lpOverlapped, ReadCompRoutine);
}

void	ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

