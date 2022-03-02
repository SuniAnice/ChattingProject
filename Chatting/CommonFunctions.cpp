

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
	switch ( sock.m_buffer[ 0 ] )
	{
	case 'a':
	case 'A':
	{
		stringstream stream;
		string name;
		int max = -1;
		stream.str( sock.m_buffer );
		stream >> name;
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
		sock.m_roomNumber = sock.m_server->MakeRoom( name, max );
		sock.m_isInLobby = false;
		sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters.push_back( &sock );
		BroadcastMessage( sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters , sock.m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
		sock.m_currentScene->ChangeScene();
	}
		break;
	case 'X':
	case 'x':
	{
		if ( sock.m_buffer[ 1 ] == '\r' && sock.m_buffer[ 2 ] == '\n' )	closesocket( sock.m_socket );
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
		sock.SendChat( "----------------------------------------------------\r\n�κ� ���� ���� ȯ���մϴ�\r\n----------------------------------------------------\r\n�� �����(A) �÷��̾� ���(L) ������(X)\r\n" );
	}
		break;
	}
	sock.InitializeBuffer();
}
