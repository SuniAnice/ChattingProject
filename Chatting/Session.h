

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
	// 소켓 통신을 위한 정보
	SOCKET m_socket;
	PCSTR m_ip;
	char m_buffer[ BUFFER_SIZE + 1 ];
	bool m_isProcessing;
	int m_recvBytes;
	// 플레이어 정보
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

	//getter 함수들
	SOCKET& GetSocket();

	PCSTR& GetIp();

	char* GetBuffer();

	bool IsProcessing();

	int GetRoomNumber();

	ChattingServer* GetServer();

	Scene* GetCurrentScene();

	bool InInLobby();

	string& GetName();

	//Setter 함수들
	void SetRoomNumber( int number );

	void SetScene( Scene* scene );

	void SetIsInLobby( bool isInLobby );

	bool SetName();
	// 버퍼에 입력된 값을 초기화하는 함수
	void InitializeBuffer();

	// 데이터 수신 함수
	int Recv();

	// 해당 세션의 플레이어에게 메시지를 전송하는 함수
	int SendChat( const string& message ) const;

	// 해당 세션의 플레이어가 같은 컨테이너에 있는 플레이어들에게 메시지를 전송하는 함수
	void BroadcastMessage();

	// 플레이어로부터 받은 입력을 바탕으로, 명령어를 실행하는 함수
	bool ProcessCommand();
};
