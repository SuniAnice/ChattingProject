

#include "ChattingRoom.h"
#include "ChattingServer.h"
#include "LoginScene.h"
#include "Session.h"
#include "StringTable.h"
#include <iostream>
#include <memory>


#pragma comment( lib, "Ws2_32.lib" )


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	if ( WSAStartup( MAKEWORD( 2, 0 ), &wsaData ) != 0 )
	{
		std::cout << str::errormsg::WSASTARTUP << std::endl;
		return -1;
	}

	SOCKET listenSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, 0 );
	SOCKADDR_IN serverAddress;
	memset( &serverAddress, 0, sizeof( SOCKADDR_IN ) );
	serverAddress.sin_family = AF_INET;
	if ( argc == 1 )
		serverAddress.sin_port = htons( SERVER_PORT );
	else
		serverAddress.sin_port = htons( atoi( argv[ 1 ] ) );

	serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	bind( listenSocket, reinterpret_cast< sockaddr* >( &serverAddress ), sizeof( serverAddress ) );
	std::cout << "서버 포트 : "<< htons( serverAddress.sin_port ) << std::endl;
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
			std::cout << str::errormsg::SELECT << std::endl;
		}

		// accept 처리
		if ( FD_ISSET( listenSocket, &readSet ) )
		{
			int addressSize = sizeof( clientAddress );
			SOCKET clientSocket = WSAAccept( listenSocket, reinterpret_cast< sockaddr* >( &clientAddress ), &addressSize, NULL, NULL );
			if ( clientSocket == INVALID_SOCKET )
			{
				std::cout << str::errormsg::ACCEPT << std::endl;
			}
			else
			{
				// 컨테이너에 유저 소켓 등록
				char buf[ 32 ];
				auto ip = inet_ntop( AF_INET, &clientAddress.sin_addr, buf, sizeof(buf) );
				std::cout << str::msg::CLIENT_LOGON << ip << ":" << htons( clientAddress.sin_port ) << std::endl;
				Session* info = new Session( clientSocket, ip, htons( clientAddress.sin_port ), server );
				info->ChangeToLoginScene();
				server.m_userSockets.push_back( info );
			}
		}

		// 데이터 통신
		for ( auto iter = server.m_userSockets.begin(); iter != server.m_userSockets.end(); )
		{
			auto session = ( *iter );

			// recv
			if ( FD_ISSET( session->GetSocket(), &readSet ) )
			{
				retVal = session->Recv();
				if ( retVal == SOCKET_ERROR )
				{
					// 컨테이너에서 유저 소켓 삭제
					std::cout << str::errormsg::RECV << std::endl;
					std::cout << str::msg::CLIENT_LOGOUT << session->GetIp() << std::endl;
					server.m_userNames.erase( session->GetName() );
					delete ( *iter );
					iter = server.m_userSockets.erase( iter );
					continue;
				}
				else if ( retVal == 0 )
				{
					// 컨테이너에서 유저 소켓 삭제
					std::cout << str::msg::CLIENT_LOGOUT << session->GetIp() << std::endl;
					server.m_userNames.erase( session->GetName() );
					delete ( *iter );
					iter = server.m_userSockets.erase( iter );
					continue;
				}	
				// 사용자의 입력을 처리할 필요가 있을 경우
				if ( session->IsProcessing() )
					FD_SET( session->GetSocket(), &writeSet );
			}

			// send
			if ( FD_ISSET( session->GetSocket(), &writeSet ) )
			{
				// 현재 Scene에 맞는 입력 처리 -> false인 경우에는 플레이어 접속 종료
				if ( !session->ExecutionInput() )
				{
					// 컨테이너에서 유저 소켓 삭제
					std::cout << str::msg::CLIENT_LOGOUT << session->GetIp() << std::endl;
					server.m_userNames.erase( session->GetName() );
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
