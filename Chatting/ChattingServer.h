

#pragma once


#include "ChattingRoom.h"
#include "Session.h"
#include <unordered_map>
#include <vector>


class ChattingServer
{
public:
	vector< Session* > m_userSockets;
	unordered_map < int, ChattingRoom > m_rooms;

	int MakeRoom( string name, int max );
};
