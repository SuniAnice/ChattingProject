

#include "CommonFunctions.h"


void BroadcastMessage( const vector<Session*>& container, const string& message )
{
	for ( auto& user : container )
	{
		if ( !user->m_isInLobby )	user->SendChat( message );
	}
}

void ProcessCommand( const vector<Session*>& container, Session& sock )
{
	switch ( sock.m_buffer[ 0 ] )
	{
	case 'a':
	case 'A':
		sock.m_isInLobby = false;
		BroadcastMessage( container, sock.m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
		break;
	case 'X':
	case 'x':
		if ( sock.m_buffer[ 1 ] == '\r' && sock.m_buffer[ 2 ] == '\n' )	closesocket( sock.m_socket );
		break;
	default:
		sock.SendChat( "----------------------------------------------------\r\n�κ� ���� ���� ȯ���մϴ�\r\n----------------------------------------------------\r\n�� ����(A) ������(X)\r\n" );
		break;
	}
	sock.InitializeBuffer();
}
