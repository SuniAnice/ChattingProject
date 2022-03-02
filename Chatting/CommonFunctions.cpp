

#include "ChattingScene.h"
#include "ChattingServer.h"
#include "CommonFunctions.h"
#include <sstream>


void BroadcastMessage( const vector<Session*>& container, const string& message )
{
	for ( auto& user : container )
	{
		if ( !user->m_isInLobby )	user->SendChat( message );
	}
}

bool ProcessCommand( Session& session )
{
	stringstream stream;
	string op;
	stream.str( session.m_buffer );
	stream >> op;
	switch ( session.m_buffer[0] )
	{
	case 'a':
	case 'A':
	{
		// �� �����
		string name;
		int max = -1;
		stream >> max;
		if ( max <= 1 )
		{
			session.SendChat( "�� ����� ��ɾ� : a [�����ο�] [������] (�ּ� 2��)\r\n" );
			break;
		}
		stream >> name;
		if ( name.size() <= 1 )
		{
			session.SendChat( "�� ����� ��ɾ� : a [�����ο�] [������] (�ּ� ���� ���ڼ� 2)\r\n" );
			break;
		}
		// ������ �� ������ ��û
		session.m_roomNumber = session.m_server->MakeRoom( name, max );
		session.m_isInLobby = false;
		session.m_server->m_rooms[ session.m_roomNumber ].m_chatters.push_back( &session );
		BroadcastMessage( session.m_server->m_rooms[ session.m_roomNumber ].m_chatters , session.m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
		session.m_currentScene->ChangeScene();
	}
		break;
	case 'j':
	case 'J':
	{
		// �� ����
		int roomNum = -1;
		stream >> roomNum;
		// ���� �����ϴ��� üũ
		if ( roomNum >= 1 && session.m_server->m_rooms.count( roomNum ) )
		{
			// �� �ο����� ���� á���� üũ
			if ( session.m_server->m_rooms[ roomNum ].m_maxPeople > session.m_server->m_rooms[ roomNum ].m_chatters.size() )
			{
				session.m_roomNumber = roomNum;
				session.m_isInLobby = false;
				session.m_server->m_rooms[ roomNum ].m_chatters.push_back( &session );
				BroadcastMessage( session.m_server->m_rooms[ session.m_roomNumber ].m_chatters, session.m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
				session.m_currentScene->ChangeScene();
			}
			else
			{
				session.SendChat( "���� ���� á���ϴ�.\r\n" );
			}
		}
		else
		{
			session.SendChat( "�� ���� ��ɾ� : j [�� ��ȣ] (�� ���忡 �����߽��ϴ�.)\r\n" );
		}
	}
		break;
	case 'X':
	case 'x':
	{
		// ����
		if ( session.m_buffer[ 1 ] == '\r' && session.m_buffer[ 2 ] == '\n' )	closesocket( session.m_socket );
		return false;
	}
		break;
	case 'o':
	case 'O':
	{
		// �� ���
		string message = "�� ����� ����մϴ�.\r\n";
		for ( auto& room : session.m_server->m_rooms )
		{
			message = message + to_string( room.first ) + "	(" + to_string( room.second.m_chatters.size() ) + " / " + to_string( room.second.m_maxPeople ) + ")	" + room.second.m_name + "\r\n";
		}
		session.SendChat( message );
	}
		break;
	case 'l':
	case 'L':
	{
		// �������� �÷��̾�
		string message = "�������� �÷��̾���� ����մϴ�.\r\n";
		for ( auto& player : *session.m_allChatters )
		{
			message = message + player->m_name + "	" + player->m_ip + "\r\n";
		}
		session.SendChat( message );
	}
		break;
	default:
	{
		session.SendChat( "----------------------------------------------------\r\n�κ� ���� ���� ȯ���մϴ�\r\n----------------------------------------------------\r\n�� �����(A) �÷��̾� ���(L) �� ���(o) �� ����(J) ������(X)\r\n" );
	}
		break;
	}
	session.InitializeBuffer();
	return true;
}
