

#include "ChattingScene.h"
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
	{
		sock.m_isInLobby = false;
		BroadcastMessage( container, sock.m_name + "님이 대화방에 입장했습니다.\r\n" );
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
		string message = "접속중인 플레이어들을 출력합니다.\r\n";
		for ( auto& player : container )
		{
			message = message + player->m_name + "	" + player->m_ip + "\r\n";
		}
		sock.SendChat( message );
	}
		break;
	default:
	{
		sock.SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n방 입장(A) 플레이어 목록(L) 나가기(X)\r\n" );
	}
		break;
	}
	sock.InitializeBuffer();
}
