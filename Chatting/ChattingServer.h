

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

	// �� ��ȣ�� ���� �����, �� ��ȣ�� ����
	int MakeRoom( string name, int max );

	// ��ü�� ������, �����̳��� ��� �÷��̾�� �޽����� ���� �� ���� �Լ�
	void SystemMessage( vector< Session* > container, const string& message );
};
