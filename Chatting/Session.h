

#pragma once


#include <WS2tcpip.h>
#include <string>
#include <vector>


constexpr int BUFFER_SIZE = 1024;
constexpr int  SERVER_PORT = 4000;


using namespace std;


class Scene;

class ChattingServer;

class Session
{
private:
	int m_recvBytes;
	// �÷��̾� ����
	bool m_isNameSet = false;
public:
	// ���� ����� ���� ����
	SOCKET m_socket;
	PCSTR m_ip;
	char m_buffer[ BUFFER_SIZE + 1 ];
	bool m_isProcessing;
	// �÷��̾� ����
	int m_roomNumber = 0;
	ChattingServer* m_server;
	Scene* m_currentScene;
	bool m_isInLobby = true;
	string m_name;

	Session() = delete;

	Session( SOCKET sock, PCSTR ip );

	~Session();

	void InitializeBuffer();

	int Recv();

	int SendChat( const string& message ) const;

	bool SetName();

	void BroadcastMessage();

	bool ProcessCommand();
	
};
