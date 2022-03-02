

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

void ProcessCommand( Session& sock )
{
	stringstream stream;
	string op;
	stream.str( sock.m_buffer );
	stream >> op;
	switch ( sock.m_buffer[0] )
	{
	case 'a':
	case 'A':
	{
		string name;
		int max = -1;
		stream >> max;
		if ( max <= 1 )
		{
			sock.SendChat( "�� ����� ��ɾ� : a [�����ο�] [������] (�ּ� 2��)\r\n" );
			break;
		}
		stream >> name;
		if ( name.size() <= 1 )
		{
			sock.SendChat( "�� ����� ��ɾ� : a [�����ο�] [������] (�ּ� ���� ���ڼ� 2)\r\n" );
			break;
		}
		// ������ �� ������ ��û
		sock.m_roomNumber = sock.m_server->MakeRoom( name, max );
		sock.m_isInLobby = false;
		sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters.push_back( &sock );
		BroadcastMessage( sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters , sock.m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
		sock.m_currentScene->ChangeScene();
	}
		break;
	case 'j':
	case 'J':
	{
		int roomNum = -1;
		stream >> roomNum;
		// ���� �����ϴ��� üũ
		if ( roomNum >= 1 && sock.m_server->m_rooms.count( roomNum ) )
		{
			// �� �ο����� ���� á���� üũ
			if ( sock.m_server->m_rooms[ roomNum ].m_maxPeople > sock.m_server->m_rooms[ roomNum ].m_chatters.size() )
			{
				sock.m_roomNumber = roomNum;
				sock.m_isInLobby = false;
				sock.m_server->m_rooms[ roomNum ].m_chatters.push_back( &sock );
				BroadcastMessage( sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters, sock.m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
				sock.m_currentScene->ChangeScene();
			}
			else
			{
				sock.SendChat( "���� ���� á���ϴ�.\r\n" );
			}
		}
		else
		{
			sock.SendChat( "�� ���� ��ɾ� : j [�� ��ȣ] (�� ���忡 �����߽��ϴ�.)\r\n" );
		}
	}
		break;
	case 'X':
	case 'x':
	{
		if ( sock.m_buffer[ 1 ] == '\r' && sock.m_buffer[ 2 ] == '\n' )	closesocket( sock.m_socket );
	}
		break;
	case 'o':
	case 'O':
	{
		string message = "�� ����� ����մϴ�.\r\n";
		for ( auto& room : sock.m_server->m_rooms )
		{
			message = message + to_string( room.first ) + "	(" + to_string( room.second.m_chatters.size() ) + " / " + to_string( room.second.m_maxPeople ) + ")	" + room.second.m_name + "\r\n";
		}
		sock.SendChat( message );
	}
		break;
	case 'l':
	case 'L':
	{
		string message = "�������� �÷��̾���� ����մϴ�.\r\n";
		for ( auto& player : *sock.m_allChatters )
		{
			message = message + player->m_name + "	" + player->m_ip + "\r\n";
		}
		sock.SendChat( message );
	}
		break;
	default:
	{
		sock.SendChat( "----------------------------------------------------\r\n�κ� ���� ���� ȯ���մϴ�\r\n----------------------------------------------------\r\n�� �����(A) �÷��̾� ���(L) �� ���(o) �� ����(J) ������(X)\r\n" );
	}
		break;
	}
	sock.InitializeBuffer();
}
