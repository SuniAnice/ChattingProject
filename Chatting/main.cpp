

#include <iostream>
#include <unordered_map>
#include "protocol.h"


int g_Sockets = 0;


using namespace std;


int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		cout << "WSAStartup" << endl;
		return -1;
	}

	SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
	SOCKADDR_IN serverAddress;
	memset(&serverAddress, 0, sizeof(SOCKADDR_IN));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);
	serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
	listen(listenSocket, SOMAXCONN);

	u_long flagOn = 1;
	ioctlsocket(listenSocket, FIONBIO, &flagOn);

	FD_SET readSet, writeSet;
	SOCKADDR_IN clientAddress;
	vector<SocketInfo*> userSockets;
	int retVal;

	while (true) {
		FD_ZERO(&readSet);
		FD_ZERO(&writeSet);
		FD_SET(listenSocket, &readSet);

		for (auto &socket : userSockets)
		{
			if (socket->recvBytes > socket->sendBytes)
				FD_SET(socket->socket, &writeSet);
			else
				FD_SET(socket->socket, &readSet);
		}

		if (select(0, &readSet, &writeSet, NULL, NULL) == SOCKET_ERROR)
		{
			cout << "select" << endl;
			return -1;
		}

		// accept 처리
		if (FD_ISSET(listenSocket, &readSet))
		{
			int addressSize = sizeof(clientAddress);
			SOCKET clientSocket = WSAAccept(listenSocket, reinterpret_cast<sockaddr*>(&clientAddress),
				&addressSize, NULL, NULL);
			if (clientSocket == INVALID_SOCKET)
			{
				cout << "accept error" << endl;
			}
			else
			{
				// 컨테이너에 유저 소켓 등록
				char buf[32];
				cout << "클라이언트 접속 : " << inet_ntop(AF_INET, &clientAddress.sin_addr, buf, sizeof(buf)) << endl;
				SocketInfo* info = new SocketInfo(clientSocket);
				userSockets.push_back(info);
			}
		}

		// 데이터 통신
		for (auto iter = userSockets.begin(); iter != userSockets.end(); )
		{
			auto sock = (*iter);

			// recv
			if (FD_ISSET(sock->socket, &readSet))
			{
				retVal = recv(sock->socket, sock->buffer, BUFFER_SIZE, 0);
				if (retVal == SOCKET_ERROR)
				{
					// 컨테이너에서 유저 소켓 삭제
					cout << "recv error" << endl;
					delete (*iter);
					iter = userSockets.erase(iter);
					continue;
				}
				else if (retVal == 0)
				{
					// 컨테이너에서 유저 소켓 삭제
					delete (*iter);
					iter = userSockets.erase(iter);
					continue;
				}
				sock->recvBytes = retVal;
				sock->buffer[retVal] = '\0';
				
			}

			// send
			if (FD_ISSET(sock->socket, &writeSet))
			{
				retVal = send(sock->socket, sock->buffer + sock->sendBytes,
					sock->recvBytes - sock->sendBytes, 0);
				if (retVal == SOCKET_ERROR)
				{
					// 컨테이너에서 유저 소켓 삭제
					cout << "recv error" << endl;
					delete (*iter);
					iter = userSockets.erase(iter);
					continue;
				}
				sock->sendBytes += retVal;
				if (sock->recvBytes == sock->sendBytes)
				{
					sock->recvBytes = sock->sendBytes = 0;
				}
			}
			iter++;
		}

	}
	closesocket(listenSocket);
	WSACleanup();
}
