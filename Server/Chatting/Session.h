

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
	// 소켓 통신을 위한 정보
	SOCKET m_socket;
	PCSTR m_ip;
	USHORT m_port;
	char m_buffer[ BUFFER_SIZE + 1 ];
	bool m_isProcessing;
	int m_recvBytes;
	std::queue < std::string > m_packets;
	// 플레이어 정보
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

	//getter 함수들
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

	//Setter 함수들
	void SetRoomNumber( int number );

	void SetScene( std::unique_ptr< Scene > scene );

	void SetIsInLobby( bool isInLobby );

	bool SetName();

	void SetTime( time_t time );

	// 데이터 수신 함수
	int Recv();

	// 해당 세션의 플레이어에게 메시지를 전송하는 함수
	int SendChat( const std::string& message ) const;

	// 해당 세션의 플레이어가 같은 컨테이너에 있는 플레이어들에게 메시지를 전송하는 함수
	void BroadcastMessage();

	// 플레이어로부터 받은 입력을 바탕으로, 명령어를 실행하는 함수
	bool ProcessCommand();

private:
	// 버퍼에 입력된 값을 초기화하는 함수
	void InitializeBuffer();
};
