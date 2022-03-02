

#include "ChattingServer.h"
#include "CommonFunctions.h"
#include "Session.h"
#include <sstream>


Session::~Session()
{
	m_currentScene->ExitScene();
	delete m_currentScene;
}

int Session::Recv()
{
	int retVal = recv( m_socket, m_buffer + m_recvBytes, BUFFER_SIZE - m_recvBytes, 0 );
	m_recvBytes += retVal;
	if ( m_buffer[ m_recvBytes - 1 ] == '\n' )
	{
		m_isProcessing = true;
	}
	return retVal;
}

int Session::SendChat( const string& message ) const
{
	int retVal = send( m_socket, message.c_str(), message.size(), 0 );
	return retVal;
}

bool Session::SetName()
{
	stringstream stream;
	stream.str( m_buffer );
	stream >> m_name;

	if ( m_server->m_userNames.count( m_name ) == 0 )
	{
		m_isNameSet = true;
		SendChat( m_name + "을 닉네임으로 사용합니다\r\n" );
		m_server->m_userNames[ m_name ] = this;
	}
	else
	{
		SendChat( "중복된 닉네임이 존재합니다\r\n" );
		InitializeBuffer();
		return false;
	}
	
	InitializeBuffer();

	return true;
}

void Session::BroadcastMessage()
{
	string chatting = m_name + " : " + m_buffer;
	for ( auto& user : m_server->m_rooms[ m_roomNumber ].m_chatters )
	{
		user->SendChat( chatting );
	}
	InitializeBuffer();
}

void Session::InitializeBuffer()
{
	m_recvBytes = 0;
	m_isProcessing = false;
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
}

bool Session::ProcessCommand()
{
	stringstream stream;
	string op;
	stream.str( m_buffer );
	stream >> op;
	switch ( m_buffer[ 0 ] )
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
			SendChat( "방 만들기 명령어 : a [제한인원] [방제목] (최소 2명)\r\n" );
			break;
		}
		stream >> name;
		if ( name.size() <= 1 )
		{
			SendChat( "방 만들기 명령어 : a [제한인원] [방제목] (최소 제목 글자수 2)\r\n" );
			break;
		}
		// 서버에 방 생성을 요청
		m_roomNumber = m_server->MakeRoom( name, max );
		m_isInLobby = false;
		m_server->m_rooms[ m_roomNumber ].m_chatters.push_back( this );
		SystemMessage( m_server->m_rooms[ m_roomNumber ].m_chatters, m_name + "님이 대화방에 입장했습니다.\r\n" );
		m_currentScene->ChangeScene();
	}
	break;
	case 'j':
	case 'J':
	{
		// 방 입장
		int roomNum = -1;
		stream >> roomNum;
		// 방이 존재하는지 체크
		if ( roomNum >= 1 && m_server->m_rooms.count( roomNum ) )
		{
			// 방 인원수가 가득 찼는지 체크
			if ( m_server->m_rooms[ roomNum ].m_maxPeople > m_server->m_rooms[ roomNum ].m_chatters.size() )
			{
				m_roomNumber = roomNum;
				m_isInLobby = false;
				m_server->m_rooms[ roomNum ].m_chatters.push_back( this );
				SystemMessage( m_server->m_rooms[ m_roomNumber ].m_chatters, m_name + "님이 대화방에 입장했습니다.\r\n" );
				m_currentScene->ChangeScene();
			}
			else
			{
				SendChat( "방이 가득 찼습니다.\r\n" );
			}
		}
		else
		{
			SendChat( "방 입장 명령어 : j [방 번호] (방 입장에 실패했습니다.)\r\n" );
		}
	}
	break;
	case 'X':
	case 'x':
	{
		// 종료
		if ( m_buffer[ 1 ] == '\r' && m_buffer[ 2 ] == '\n' )	closesocket( m_socket );
		return false;
	}
	break;
	case 'o':
	case 'O':
	{
		// 방 목록
		string message = "방 목록을 출력합니다.\r\n";
		for ( auto& room : m_server->m_rooms )
		{
			message = message + to_string( room.first ) + "	(" + to_string( room.second.m_chatters.size() ) + " / " + to_string( room.second.m_maxPeople ) + ")	" + room.second.m_name + "\r\n";
		}
		SendChat( message );
	}
	break;
	case 'l':
	case 'L':
	{
		// 접속중인 플레이어
		string message = "접속중인 플레이어들을 출력합니다.\r\n";
		for ( auto& player : m_server->m_userSockets )
		{
			message = message + player->m_name + "	" + player->m_ip + "\r\n";
		}
		SendChat( message );
	}
	break;
	default:
	{
		SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n방 만들기(A) 플레이어 목록(L) 방 목록(o) 방 입장(J) 나가기(X)\r\n" );
	}
	break;
	}
	InitializeBuffer();
	return true;
}