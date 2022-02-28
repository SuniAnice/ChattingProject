

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
		BroadcastMessage( container, sock.m_name + "님이 대화방에 입장했습니다.\r\n" );
		break;
	case 'X':
	case 'x':
		if ( sock.m_buffer[ 1 ] == '\r' && sock.m_buffer[ 2 ] == '\n' )	closesocket( sock.m_socket );
		break;
	default:
		sock.SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n방 입장(A) 나가기(X)\r\n" );
		break;
	}
	sock.InitializeBuffer();
}
