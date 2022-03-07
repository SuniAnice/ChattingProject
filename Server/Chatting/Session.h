

#pragma once


#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <memory>
#include <queue>


constexpr int	BUFFER_SIZE = 1024;
constexpr int	SERVER_PORT = 4000;


class Scene;

class ChattingServer;

class Session
{
private:
	// ���� ����� ���� ����
	SOCKET m_socket;
	PCSTR m_ip;
	USHORT m_port;
	char m_buffer[ BUFFER_SIZE + 1 ];
	bool m_isProcessing;
	int m_recvBytes;
	std::queue < std::string > m_packets;
	// �÷��̾� ����
	bool m_isNameSet;
	int m_roomNumber;
	ChattingServer* m_server;
	std::unique_ptr<Scene> m_currentScene;
	bool m_isInLobby;
	std::string m_name;
	time_t m_enterTime;
public:
	Session() = delete;

	Session( SOCKET& sock, PCSTR& ip, USHORT port, ChattingServer& server );

	~Session();

	//getter �Լ���
	SOCKET& GetSocket();

	PCSTR& GetIp();

	char* GetBuffer();

	bool IsProcessing() const;

	int GetRoomNumber() const;

	ChattingServer* GetServer();

	std::unique_ptr< Scene >& GetCurrentScene();

	bool InInLobby() const;

	std::string& GetName();

	time_t GetTime();

	//Setter �Լ���
	void SetRoomNumber( int number );

	void SetScene( std::unique_ptr< Scene > scene );

	void SetIsInLobby( bool isInLobby );

	bool SetName();

	void SetTime( time_t time );

	// ������ ���� �Լ�
	int Recv();

	// �ش� ������ �÷��̾�� �޽����� �����ϴ� �Լ�
	int SendChat( const std::string& message ) const;

	// �ش� ������ �÷��̾ ���� �����̳ʿ� �ִ� �÷��̾�鿡�� �޽����� �����ϴ� �Լ�
	void BroadcastMessage();

	// �÷��̾�κ��� ���� �Է��� ��������, ��ɾ �����ϴ� �Լ�
	bool ProcessCommand();

private:
	// ���ۿ� �Էµ� ���� �ʱ�ȭ�ϴ� �Լ�
	void InitializeBuffer();
};
