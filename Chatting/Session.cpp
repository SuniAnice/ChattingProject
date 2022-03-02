

#include "ChattingServer.h"
#include "Scene.h"
#include "Session.h"
#include <sstream>


Session::Session( SOCKET& sock, PCSTR& ip, ChattingServer& server ) : m_socket( sock ), m_ip( ip ), m_recvBytes( 0 ), m_isProcessing( false ), m_isNameSet( false ), m_server( &server ), m_roomNumber( 0 )
{
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
}

Session::~Session()
{
	m_currentScene->ExitScene();
	delete m_currentScene;
}

SOCKET& Session::GetSocket()
{
	return m_socket;
}

PCSTR& Session::GetIp()
{
	return m_ip;
}

char* Session::GetBuffer()
{
	return m_buffer;
}

bool Session::IsProcessing()
{
	return m_isProcessing;
}

int Session::GetRoomNumber()
{
	return 0;
}

ChattingServer* Session::GetServer()
{
	return m_server;
}

Scene* Session::GetCurrentScene()
{
	return m_currentScene;
}

bool Session::InInLobby()
{
	return m_isInLobby;
}


string& Session::GetName()
{
	return m_name;
}

void Session::SetRoomNumber( int number )
{
	m_roomNumber = number;
}

void Session::SetScene( Scene* scene )
{
	m_currentScene = scene;
}

void Session::SetIsInLobby( bool isInLobby )
{
	m_isInLobby = isInLobby;
}

void Session::InitializeBuffer()
{
	m_recvBytes = 0;
	m_isProcessing = false;
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
}

int Session::Recv()
{
	int retVal = recv( m_socket, m_buffer + m_recvBytes, BUFFER_SIZE - m_recvBytes, 0 );
	m_recvBytes += retVal;
	// 엔터키가 입력되었을 경우
	if ( m_buffer[ m_recvBytes - 1 ] == '\n' )
	{
		m_isProcessing = true;
	}
	// 백스페이스가 입력되었을 경우
	if ( m_buffer[ m_recvBytes - 1 ] == '\b' && m_recvBytes > 1 )
	{
		m_buffer[ m_recvBytes - 1 ] = '\0';
		m_buffer[ m_recvBytes - 2 ] = '\0';
		m_recvBytes -= 2;
		return retVal;
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

	// 중복된 닉네임이 있는지 체크 후, 없다면 컨테이너에 등록
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
	// 현재 플레이어가 접속한 대화방의 모든 플레이어에게 버퍼의 내용 전송
	for ( auto& user : m_server->m_rooms[ m_roomNumber ].m_chatters )
	{
		user->SendChat( chatting );
	}
	InitializeBuffer();
}

bool Session::ProcessCommand()
{
	stringstream stream;
	stream.str( m_buffer );
	string tmp;
	// 스트림에서 명령어 제거
	stream >> tmp;

	bool isInRoom = (m_roomNumber != 0);
	char* ptr = &m_buffer[ 0 ];
	// 방 안일 경우 / 뒤를 명령어로 인식함
	switch ( *(ptr + isInRoom) )
	{
	case 'a':
	case 'A':
	{
		// 방 만들기
		if ( isInRoom )
		{
			SendChat( "잘못된 명령어 입력입니다.\r\n" );
			break;
		}
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
		m_server->SystemMessage( m_server->m_rooms[ m_roomNumber ].m_chatters, m_name + "님이 대화방에 입장했습니다.\r\n" );
		m_currentScene->ChangeScene();
	}
	break;
	case 'j':
	case 'J':
	{
		// 방 입장
		if ( isInRoom )
		{
			SendChat( "잘못된 명령어 입력입니다.\r\n" );
			break;
		}
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
				m_server->SystemMessage( m_server->m_rooms[ m_roomNumber ].m_chatters, m_name + "님이 대화방에 입장했습니다.\r\n" );
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
		if ( isInRoom )
		{
			if ( *( ptr + 2 ) == '\r' && *( ptr + 3 ) == '\n' )
			{
				m_currentScene->ExitScene();
				closesocket( m_socket );
				return false;
			}
		}
		else
		{
			if ( *( ptr + 1 ) == '\r' && *( ptr + 2 ) == '\n' )
			{
				closesocket( m_socket );
				return false;
			}
		}
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
	case 'i':
	case 'I':
	{
		// 플레이어 정보 보기
		string player;
		stream >> player;
		if ( m_server->m_userNames.count( player ) != 0 )
		{
			string message = "해당 플레이어의 정보를 출력합니다.\r\n";
			message = message + "닉네임 : " + player + "	ip : " + m_server->m_userNames[player]->m_ip + "	위치 : ";
			// 로비에 있는지 판단
			if ( m_server->m_userNames[ player ]->m_roomNumber == 0 )	message = message + "로비\r\n";
			else message = message + to_string( m_server->m_userNames[ player ]->m_roomNumber ) + "번 채팅방 \r\n";

			SendChat( message );
		}
		else
		{
			SendChat( "플레이어 정보 명령어 : i [닉네임] (해당 닉네임이 존재하지 않습니다)\r\n" );
		}
	}
		break;
	case 'p':
	case 'P':
	{
		// 방 정보 보기
		int roomNumber = 0;
		stream >> roomNumber;
		// 방이 존재할 경우
		if ( roomNumber != 0 && m_server->m_rooms.count( roomNumber ) != 0 )
		{
			string message = "해당 방의 정보를 출력합니다.\r\n";
			message = message + "이름 : " + m_server->m_rooms[ roomNumber ].m_name + "	정원 : " + to_string( m_server->m_rooms[ roomNumber ].m_maxPeople ) + "\r\n";
			message = message + "참가자 : " + to_string( m_server->m_rooms[ roomNumber ].m_chatters.size() ) + "\r\n";
			for ( auto& player : m_server->m_rooms[ roomNumber ].m_chatters )
			{
				message = message + player->m_name + "	" + player->m_ip + "\r\n";
			}
			SendChat( message );
		}
		else
		{
			SendChat( "방 정보 명령어 : p [방 번호] (해당 채팅방이 존재하지 않습니다)\r\n" );
		}
	}
		break;
	case 't':
	case 'T':
	{
		// 귓속말 처리
		string receiver;
		string message;

		stream >> receiver;
		// 해당 닉네임을 가진 사람이 존재할 경우
		if ( m_server->m_userNames.count( receiver ) != 0 )
		{
			stream >> message;
			SendChat( receiver + "님에게 귓속말 : " + message + "\r\n" );
			m_server->m_userNames[ receiver ]->SendChat( m_name + "님의 귓속말 : " + message + "\r\n" );
		}
		else
		{
			SendChat( "귓속말 명령어 : t [닉네임] [할 말] (해당 닉네임이 존재하지 않습니다)\r\n" );
		}

	}
		break;
	case 'q':
	case 'Q':
	{
		// 방 퇴장
		if ( !isInRoom )
		{
			SendChat( "잘못된 명령어 입력입니다.\r\n" );
			break;
		}
		m_currentScene->ExitScene();
	}
		break;
	case 'h':
	case 'H':
	{
		string message = "----------------------------------------------------\r\n명령어 목록을 출력합니다.\r\n----------------------------------------------------\r\n";
		if ( isInRoom )
		{
			message = message + "플레이어 목록 :		L\r\n플레이어 정보 :		i [닉네임]\r\n귓속말 :		t [닉네임] [할 말]\r\n방 목록 :		o\r\n방 정보 :		p [방 번호]\r\n방 나가기 :		q\r\n종료 :			x\r\n";
			SendChat( message );
		}
		else
		{
			message = message + "방 만들기 :		a [인원수] [방제목]\r\n플레이어 목록 :		L\r\n플레이어 정보 :		i [닉네임]\r\n귓속말 :		t [닉네임] [할 말]\r\n방 목록 :		o\r\n방 정보 :		p [방 번호]\r\n방 입장 :		j\r\n종료 :			x\r\n";
			SendChat( message );
		}
	}
		break;
	default:
	{
		if ( isInRoom )	SendChat( "명령어 보기(h) 나가기(X)\r\n" );
		else			SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n명령어 보기(h) 나가기(X)\r\n" );
	}
	break;
	}
	InitializeBuffer();
	return true;
}