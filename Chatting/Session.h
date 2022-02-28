

#pragma once


#include <WS2tcpip.h>
#include <string>
#include <vector>


#define BUFFER_SIZE 1024
#define SERVER_PORT 4000


using namespace std;


#pragma comment( lib, "Ws2_32.lib" )


class Session
{
private:
	int m_recvBytes;
	// 플레이어 정보
	bool m_isNameSet = false;
	int m_roomNumber = 0;
public:
	// 소켓 통신을 위한 정보
	SOCKET m_socket;
	PCSTR m_ip;
	char m_buffer[ BUFFER_SIZE + 1 ];
	bool m_isProcessing;
	// 플레이어 정보
	bool m_isInLobby = true;
	string m_name;

	Session() = delete;

	Session( SOCKET sock, PCSTR ip ) : m_socket( sock ), m_ip( ip ), m_recvBytes( 0 )
	{
		ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
	}

	~Session() {}

	void InitializeBuffer();

	int Recv();

	int SendChat( const string& message ) const;

	bool SetName();

	void BroadcastMessage( const vector< Session* > &container);
	
};
