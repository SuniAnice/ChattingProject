

#pragma once


#include "ChattingRoom.h"
#include <unordered_map>
#include <vector>


class Session;

class ChattingServer
{
public:
	std::vector< Session* > m_userSockets;
	std::unordered_map < int, ChattingRoom > m_rooms;
	std::unordered_map < std::string, Session* > m_userNames;

	// 빈 번호에 방을 만들고, 방 번호를 리턴
	int MakeRoom( const std::string& name, int max );

	// 주체가 없으며, 컨테이너의 모든 플레이어에게 메시지를 보낼 때 쓰는 함수
	void SystemMessage( const std::vector< Session* > &container, const std::string& message );
};
