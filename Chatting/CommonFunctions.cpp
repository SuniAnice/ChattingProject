

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
			sock.SendChat( "방 만들기 명령어 : a [제한인원] [방제목] (최소 2명)\r\n" );
			break;
		}
		stream >> name;
		if ( name.size() <= 1 )
		{
			sock.SendChat( "방 만들기 명령어 : a [제한인원] [방제목] (최소 제목 글자수 2)\r\n" );
			break;
		}
		// 서버에 방 생성을 요청
		sock.m_roomNumber = sock.m_server->MakeRoom( name, max );
		sock.m_isInLobby = false;
		sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters.push_back( &sock );
		BroadcastMessage( sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters , sock.m_name + "님이 대화방에 입장했습니다.\r\n" );
		sock.m_currentScene->ChangeScene();
	}
		break;
	case 'j':
	case 'J':
	{
		int roomNum = -1;
		stream >> roomNum;
		// 방이 존재하는지 체크
		if ( roomNum >= 1 && sock.m_server->m_rooms.count( roomNum ) )
		{
			// 방 인원수가 가득 찼는지 체크
			if ( sock.m_server->m_rooms[ roomNum ].m_maxPeople > sock.m_server->m_rooms[ roomNum ].m_chatters.size() )
			{
				sock.m_roomNumber = roomNum;
				sock.m_isInLobby = false;
				sock.m_server->m_rooms[ roomNum ].m_chatters.push_back( &sock );
				BroadcastMessage( sock.m_server->m_rooms[ sock.m_roomNumber ].m_chatters, sock.m_name + "님이 대화방에 입장했습니다.\r\n" );
				sock.m_currentScene->ChangeScene();
			}
			else
			{
				sock.SendChat( "방이 가득 찼습니다.\r\n" );
			}
		}
		else
		{
			sock.SendChat( "방 입장 명령어 : j [방 번호] (방 입장에 실패했습니다.)\r\n" );
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
		string message = "방 목록을 출력합니다.\r\n";
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
		string message = "접속중인 플레이어들을 출력합니다.\r\n";
		for ( auto& player : *sock.m_allChatters )
		{
			message = message + player->m_name + "	" + player->m_ip + "\r\n";
		}
		sock.SendChat( message );
	}
		break;
	default:
	{
		sock.SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n방 만들기(A) 플레이어 목록(L) 방 목록(o) 방 입장(J) 나가기(X)\r\n" );
	}
		break;
	}
	sock.InitializeBuffer();
}
