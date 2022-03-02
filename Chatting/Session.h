

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
	// ���� ����� ���� ����
	SOCKET m_socket;
	PCSTR m_ip;
	char m_buffer[ BUFFER_SIZE + 1 ];
	bool m_isProcessing;
	int m_recvBytes;
	// �÷��̾� ����
	bool m_isNameSet;
	int m_roomNumber;
	ChattingServer* m_server;
	Scene* m_currentScene;
	bool m_isInLobby;
	string m_name;
public:
	Session() = delete;

	Session( SOCKET& sock, PCSTR& ip, ChattingServer& server );

	~Session();

	//getter �Լ���
	SOCKET& GetSocket();

	PCSTR& GetIp();

	char* GetBuffer();

	bool IsProcessing();

	int GetRoomNumber();

	ChattingServer* GetServer();

	Scene* GetCurrentScene();

	bool InInLobby();

	string& GetName();

	//Setter �Լ���
	void SetRoomNumber( int number );

	void SetScene( Scene* scene );

	void SetIsInLobby( bool isInLobby );

	bool SetName();

	// ������ ���� �Լ�
	int Recv();

	// �ش� ������ �÷��̾�� �޽����� �����ϴ� �Լ�
	int SendChat( const string& message ) const;

	// �ش� ������ �÷��̾ ���� �����̳ʿ� �ִ� �÷��̾�鿡�� �޽����� �����ϴ� �Լ�
	void BroadcastMessage();

	// �÷��̾�κ��� ���� �Է��� ��������, ��ɾ �����ϴ� �Լ�
	bool ProcessCommand();

private:
	// ���ۿ� �Էµ� ���� �ʱ�ȭ�ϴ� �Լ�
	void InitializeBuffer();
};
