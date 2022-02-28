

#pragma once


#include <WS2tcpip.h>
#include <vector>


#define BUFFER_SIZE 1024
#define SERVER_PORT 4000


#pragma comment(lib, "Ws2_32.lib")


class SocketInfo
{
public:
	SOCKET socket;
	char buffer[BUFFER_SIZE + 1];
	int recvBytes;
	int sendBytes;

	SocketInfo() = delete;

	SocketInfo(SOCKET sock)
	{
		socket = sock;
		recvBytes = 0;
		sendBytes = 0;
	}

	
};