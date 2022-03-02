

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
		// 방 만들기
		string name;
		int max = -1;
		stream >> max;
		if ( max <= 1 )
		{
			session.SendChat( "방 만들기 명령어 : a [제한인원] [방제목] (최소 2명)\r\n" );
			break;
		}
		stream >> name;
		if ( name.size() <= 1 )
		{
			session.SendChat( "방 만들기 명령어 : a [제한인원] [방제목] (최소 제목 글자수 2)\r\n" );
			break;
		}
		// 서버에 방 생성을 요청
		session.m_roomNumber = session.m_server->MakeRoom( name, max );
		session.m_isInLobby = false;
		session.m_server->m_rooms[ session.m_roomNumber ].m_chatters.push_back( &session );
		BroadcastMessage( session.m_server->m_rooms[ session.m_roomNumber ].m_chatters , session.m_name + "님이 대화방에 입장했습니다.\r\n" );
		session.m_currentScene->ChangeScene();
	}
		break;
	case 'j':
	case 'J':
	{
		// 방 입장
		int roomNum = -1;
		stream >> roomNum;
		// 방이 존재하는지 체크
		if ( roomNum >= 1 && session.m_server->m_rooms.count( roomNum ) )
		{
			// 방 인원수가 가득 찼는지 체크
			if ( session.m_server->m_rooms[ roomNum ].m_maxPeople > session.m_server->m_rooms[ roomNum ].m_chatters.size() )
			{
				session.m_roomNumber = roomNum;
				session.m_isInLobby = false;
				session.m_server->m_rooms[ roomNum ].m_chatters.push_back( &session );
				BroadcastMessage( session.m_server->m_rooms[ session.m_roomNumber ].m_chatters, session.m_name + "님이 대화방에 입장했습니다.\r\n" );
				session.m_currentScene->ChangeScene();
			}
			else
			{
				session.SendChat( "방이 가득 찼습니다.\r\n" );
			}
		}
		else
		{
			session.SendChat( "방 입장 명령어 : j [방 번호] (방 입장에 실패했습니다.)\r\n" );
		}
	}
		break;
	case 'X':
	case 'x':
	{
		// 종료
		if ( session.m_buffer[ 1 ] == '\r' && session.m_buffer[ 2 ] == '\n' )	closesocket( session.m_socket );
		return false;
	}
		break;
	case 'o':
	case 'O':
	{
		// 방 목록
		string message = "방 목록을 출력합니다.\r\n";
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
		// 접속중인 플레이어
		string message = "접속중인 플레이어들을 출력합니다.\r\n";
		for ( auto& player : *session.m_allChatters )
		{
			message = message + player->m_name + "	" + player->m_ip + "\r\n";
		}
		session.SendChat( message );
	}
		break;
	default:
	{
		session.SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n방 만들기(A) 플레이어 목록(L) 방 목록(o) 방 입장(J) 나가기(X)\r\n" );
	}
		break;
	}
	session.InitializeBuffer();
	return true;
}
