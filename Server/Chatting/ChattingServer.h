

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

	// �� ��ȣ�� ���� �����, �� ��ȣ�� ����
	int MakeRoom( const std::string& name, int max );

	// ��ü�� ������, �����̳��� ��� �÷��̾�� �޽����� ���� �� ���� �Լ�
	void SystemMessage( const std::vector< Session* > &container, const std::string& message );
};
