

#include "ChattingRoom.h"
#include "ChattingServer.h"
#include "LoginScene.h"
#include "Session.h"
#include <iostream>
#include <memory>


#pragma comment( lib, "Ws2_32.lib" )


using namespace std;


int main()
{
	WSADATA wsaData;
	if ( WSAStartup( MAKEWORD( 2, 0 ), &wsaData ) != 0 )
	{
		cout << "WSAStartup" << endl;
		return -1;
	}

	SOCKET listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, 0 );
	SOCKADDR_IN serverAddress;
	memset( &serverAddress, 0, sizeof( SOCKADDR_IN ) );
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( SERVER_PORT );
	serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	bind( listenSocket, reinterpret_cast< sockaddr* >( &serverAddress ), sizeof( serverAddress ) );
	listen( listenSocket, SOMAXCONN );

	u_long flagOn = 1;
	ioctlsocket( listenSocket, FIONBIO, &flagOn );

	FD_SET readSet, writeSet;
	SOCKADDR_IN clientAddress;
	int retVal;
	ChattingServer server;

	while ( true ) {
		FD_ZERO( &readSet );
		FD_ZERO( &writeSet );
		FD_SET( listenSocket, &readSet );

		for ( auto &m_socket : server.m_userSockets )
		{
			FD_SET( m_socket->GetSocket(), &readSet );
		}

		if ( select( 0, &readSet, &writeSet, NULL, NULL ) == SOCKET_ERROR )
		{
			cout << "select error" << endl;
		}

		// accept ó��
		if ( FD_ISSET( listenSocket, &readSet ) )
		{
			int addressSize = sizeof( clientAddress );
			SOCKET clientSocket = WSAAccept( listenSocket, reinterpret_cast< sockaddr* >( &clientAddress ), &addressSize, NULL, NULL );
			if ( clientSocket == INVALID_SOCKET )
			{
				cout << "accept error" << endl;
			}
			else
			{
				// �����̳ʿ� ���� ���� ���
				char buf[ 32 ];
				auto ip = inet_ntop( AF_INET, &clientAddress.sin_addr, buf, sizeof(buf) );
				cout << "Ŭ���̾�Ʈ ���� : " << ip << endl;
				Session* info = new Session( clientSocket, ip, server );
				info->SetScene( make_shared< LoginScene >( info ) );
				server.m_userSockets.push_back( info );
			}
		}

		// ������ ���
		for ( auto iter = server.m_userSockets.begin(); iter != server.m_userSockets.end(); )
		{
			auto sock = ( *iter );

			// recv
			if ( FD_ISSET( sock->GetSocket(), &readSet ) )
			{
				retVal = sock->Recv();
				if ( retVal == SOCKET_ERROR )
				{
					// �����̳ʿ��� ���� ���� ����
					cout << "recv error" << endl;
					cout << "Ŭ���̾�Ʈ �������� : " << sock->GetIp() << endl;
					server.m_userNames.erase( sock->GetName() );
					delete ( *iter );
					iter = server.m_userSockets.erase( iter );
					continue;
				}
				else if ( retVal == 0 )
				{
					// �����̳ʿ��� ���� ���� ����
					cout << "Ŭ���̾�Ʈ �������� : " << sock->GetIp() << endl;
					server.m_userNames.erase( sock->GetName() );
					delete ( *iter );
					iter = server.m_userSockets.erase( iter );
					continue;
				}	
				// ������� �Է��� ó���� �ʿ䰡 ���� ���
				if ( sock->IsProcessing() )
					FD_SET( sock->GetSocket(), &writeSet );
			}

			// send
			if ( FD_ISSET( sock->GetSocket(), &writeSet ) )
			{
				// ���� Scene�� �´� �Է� ó�� -> false�� ��쿡�� �÷��̾� ���� ����
				if ( !sock->GetCurrentScene()->ExecutionInput() )
				{
					// �����̳ʿ��� ���� ���� ����
					cout << "Ŭ���̾�Ʈ �������� : " << sock->GetIp() << endl;
					server.m_userNames.erase( sock->GetName() );
					delete ( *iter );
					iter = server.m_userSockets.erase( iter );
					continue;
				}
			}
			iter++;
		}

	}
	closesocket( listenSocket );
	WSACleanup();
}
