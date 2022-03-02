

#pragma once


#include "ChattingRoom.h"
#include <unordered_map>
#include <vector>

class Session;

class ChattingServer
{
public:
	vector< Session* > m_userSockets;
	unordered_map < int, ChattingRoom > m_rooms;
	unordered_map < string, Session* > m_userNames;

	// 빈 번호에 방을 만들고, 방 번호를 리턴
	int MakeRoom( string name, int max );

	// 주체가 없으며, 컨테이너의 모든 플레이어에게 메시지를 보낼 때 쓰는 함수
	void SystemMessage( vector< Session* > container, const string& message );
};
