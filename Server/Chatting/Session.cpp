

#include "ChattingServer.h"
#include "ChattingScene.h"
#include "LoginScene.h"
#include "LobbyScene.h"
#include "Scene.h"
#include "Session.h"
#include "StringTable.h"
#include <iostream>
#include <sstream>
#include <locale.h>


std::wstring mbs_to_wcs( std::string str )
{
	const char* strs = str.c_str();
	wchar_t wcs[ BUFFER_SIZE ] = { 0 };
	mbstate_t shiftState = mbstate_t();
	setlocale( LC_ALL, "" );
	size_t ret;
	mbsrtowcs_s( &ret, wcs, &strs, sizeof( wcs ), &shiftState );
	return std::wstring( wcs );
}

Session::Session( SOCKET& sock, PCSTR& ip, USHORT port, ChattingServer& server ) : 
	m_socket( sock ), m_ip( ip ), m_port( port ), m_recvBytes( 0 ), m_isProcessing( false ),
	m_isNameSet( false ), m_server( &server ), m_roomNumber( 0 ), m_enterTime( 0 )
{
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
}

Session::~Session()
{
	m_currentScene->ExitScene();
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

bool Session::IsProcessing() const
{
	return m_isProcessing;
}

int Session::GetRoomNumber() const
{
	return m_roomNumber;
}

ChattingServer* Session::GetServer()
{
	return m_server;
}

bool Session::InInLobby() const
{
	return m_isInLobby;
}


std::string& Session::GetName()
{
	return m_name;
}

time_t Session::GetTime()
{
	return m_enterTime;
}

void Session::SetRoomNumber( int number )
{
	m_roomNumber = number;
}
void Session::SetIsInLobby( bool isInLobby )
{
	m_isInLobby = isInLobby;
}

bool Session::SetName()
{
	m_packets.pop();
	std::stringstream stream;
	stream.str( m_buffer );
	stream >> m_name;

	// �����ڵ�� ���� üũ
	std::wstring wstr;
	wstr = mbs_to_wcs( m_name );

	if ( wstr.size() < 2 || wstr.size() > 8 )
	{
		SendChat( str::msg::PLAYER_NICKNAMESIZEERROR );
		InitializeBuffer();
		return false;
	}

	// �ߺ��� �г����� �ִ��� üũ ��, ���ٸ� �����̳ʿ� ���
	if ( m_server->m_userNames.count( m_name ) == 0 )
	{
		m_isNameSet = true;
		SendChat( m_name + str::msg::PLAYER_USINGNICKNAME );
		m_server->m_userNames[ m_name ] = this;
	}
	else
	{
		SendChat( str::msg::PLAYER_NICKNAMEEXIST );
		InitializeBuffer();
		return false;
	}

	InitializeBuffer();
	return true;
}

void Session::SetTime( time_t time )
{
	m_enterTime = time;
}

bool Session::ExecutionInput()
{
	return m_currentScene->ExecutionInput();
}

void Session::ChangeToLoginScene()
{
	m_currentScene = std::make_unique< LoginScene >( this );
}

void Session::ChangeToLobbyScene()
{
	m_currentScene = std::make_unique< LobbyScene >( this );
}

void Session::ChangeToChattingScene()
{
	m_currentScene = std::make_unique< ChattingScene >( this );
}

int Session::Recv()
{
	int retVal = recv( m_socket, m_buffer + m_recvBytes, BUFFER_SIZE - m_recvBytes, 0 );
	m_recvBytes += retVal;
	if ( retVal <= 0 ) return retVal;
	std::string tmp = m_buffer;
	char* prev = m_buffer;
	char* ptr = strstr( prev, "\n");

	
	// ����Ű�� �ԷµǾ��� ���
	while ( ptr != NULL )
	{
		auto packet = tmp.substr( prev - m_buffer, ptr - prev + 1 );
		m_packets.push( packet );
		prev = ptr + 1;
		ptr = strstr( ptr + 1, "\n" );
		std::cout << "Data received from " << m_ip << ":"  << m_port << " - " << packet << std::endl;
		m_isProcessing = true;
	}
	// ���� ������ �ӽ� ����
	m_leftovers = tmp.substr( prev - m_buffer );

	
	// �齺���̽��� �ԷµǾ��� ���
	if ( m_buffer[ m_recvBytes - 1 ] == '\b' && m_recvBytes > 1 )
	{
		m_buffer[ m_recvBytes - 1 ] = '\0';
		m_buffer[ m_recvBytes - 2 ] = '\0';
		m_recvBytes -= 2;
		return retVal;
	}
	return retVal;
}

int Session::SendChat( const std::string& message )
{
	int retVal = 0;
	m_senddata.push( message );
	// ť�� ������ �����Ͱ� ������ ����
	while ( m_senddata.size() != 0 )
	{
		retVal = send( m_socket, ( m_senddata.front() + '\r' ).c_str(), m_senddata.front().size() + 1, 0 );
		if ( retVal == SOCKET_ERROR )
		{
			return retVal;
		}
		// ������ �߰��� �߷����� ť�� �߸� �κ��� �ְ� ������ ���� �����Ѵ�.
		if ( retVal != m_senddata.front().size() + 1 )
		{
			m_senddata.front() = m_senddata.front().substr( retVal );
			return retVal;
		}
		m_senddata.pop();
	}
	return retVal;
}

void Session::BroadcastMessage()
{
	std::string chatting = m_name + " : " + m_buffer;
	// ���� �÷��̾ ������ ��ȭ���� ��� �÷��̾�� ������ ���� ����
	for ( auto& user : m_server->m_rooms[ m_roomNumber ].GetChatters() )
	{
		user->SendChat( chatting );
	}
	m_packets.pop();
	InitializeBuffer();
}

bool Session::ProcessCommand()
{
	while ( m_packets.size() != 0 )
	{
		std::string buf = m_packets.front();
		m_packets.pop();
		std::stringstream stream;
		stream.str( buf );
		std::string tmp;
		// ��Ʈ������ ��ɾ� ����
		stream >> tmp;
		bool isInRoom = ( m_roomNumber != 0 );

		if ( ( tmp.size() - isInRoom ) != 1 )
		{
			if ( isInRoom )	SendChat( str::msg::DEFAULT_ROOM );
			else			SendChat( str::msg::DEFAULT_LOBBY );
			continue;
		}

		char* ptr = &buf[ 0 ];
		// �� ���� ��� / �ڸ� ��ɾ�� �ν���
		switch ( *( ptr + isInRoom ) )
		{
		case 'a':
		case 'A':
		{
			// �� �����
			if ( isInRoom )
			{
				SendChat( str::errormsg::COMMAND );
				break;
			}
			std::string name;
			int max = -1;
			stream >> max;
			// �ִ� �ο��� üũ
			if ( max <= 1 || max > 20 )
			{
				SendChat( str::msg::PLAYER_FAILMAKEROOM_MAX );
				break;
			}
			stream >> name;
			stream.get();
			auto cur = stream.tellg();
			if ( cur == -1 )
			{
				SendChat( str::msg::PLAYER_FAILMAKEROOM_NAME );
				break;
			}
			name = name + " " + buf.substr(cur);
			name.erase( std::remove_if( name.begin(), name.end(),
				[]( char n )
				{
					return ( n == '\n' || n == '\r' );
				}
			), name.end() );
			// �����ڵ�� ���� üũ
			std::wstring wstr;
			wstr = mbs_to_wcs( name );
			// �� ���� ���� üũ
			if ( wstr.size() <= 1 || wstr.size() > 20 )
			{
				SendChat( str::msg::PLAYER_FAILMAKEROOM_NAME );
				break;
			}
			
			// ������ �� ������ ��û
			m_roomNumber = m_server->MakeRoom( name, max );
			m_isInLobby = false;
			// ä�ù� �����̳ʿ� �ڽ��� �ְ�, �ý��� �޽��� ��� �� ��� �̵�
			std::string message = str::msg::PLAYER_JOINROOM + m_server->m_rooms[ m_roomNumber ].GetName() + "\r\n";
			SendChat( message );
			m_server->m_rooms[ m_roomNumber ].GetChatters().push_back( this );
			m_server->SystemMessage( m_server->m_rooms[ m_roomNumber ].GetChatters(), m_name + str::msg::PLAYER_ENTERROOM );
			m_currentScene->ChangeScene();
			m_enterTime = time( NULL );
		}
		break;
		case 'j':
		case 'J':
		{
			// �� ����
			if ( isInRoom )
			{
				SendChat( str::errormsg::COMMAND );
				break;
			}
			int roomNum = -1;
			stream >> roomNum;
			// ���� �����ϴ��� üũ
			if ( roomNum >= 1 && m_server->m_rooms.count( roomNum ) )
			{
				// �� �ο����� ���� á���� üũ
				if ( m_server->m_rooms[ roomNum ].GetMaxPeople() > m_server->m_rooms[ roomNum ].GetChatters().size() )
				{
					std::string message = str::msg::PLAYER_JOINROOM + m_server->m_rooms[ roomNum ].GetName() + "\r\n";
					SendChat( message );
					// ä�ù� �����̳ʿ� �ڽ��� �ְ�, �ý��� �޽��� ��� �� ��� �̵�
					m_roomNumber = roomNum;
					m_isInLobby = false;
					m_server->m_rooms[ roomNum ].GetChatters().push_back( this );
					m_server->SystemMessage( m_server->m_rooms[ m_roomNumber ].GetChatters(), m_name + str::msg::PLAYER_ENTERROOM );
					m_currentScene->ChangeScene();
					m_enterTime = time( NULL );
				}
				else
				{
					SendChat( str::msg::PLAYER_ROOMISFULL );
				}
			}
			else
			{
				SendChat( str::msg::PLAYER_FAILENTERROOM );
			}
		}
		break;
		case 'X':
		case 'x':
		{
			// ����
			if ( isInRoom )
			{
				if ( *( ptr + 2 ) == '\r' && *( ptr + 3 ) == '\n' )
				{
					// ������ ���Ḧ ���� ä�ù濡�� ���� �� ����
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
			// �� ���
			std::string message = str::msg::PRINT_ROOMLIST;
			for ( auto& room : m_server->m_rooms )
			{
				message = message + std::to_string( room.first ) + "	(" + std::to_string( room.second.GetChatters().size() )
					+ " / " + std::to_string( room.second.GetMaxPeople() ) + ")	" + room.second.GetName() + "\r\n";
			}
			SendChat( message );
		}
		break;
		case 'l':
		case 'L':
		{
			// �������� �÷��̾�
			std::string message = str::msg::PRINT_PLAYERLIST;
			for ( auto& player : m_server->m_userSockets )
			{
				message = message + player->m_name + "	" + player->m_ip + ":" + std::to_string( player->m_port ) + "\r\n";
			}
			SendChat( message );
		}
		break;
		case 'i':
		case 'I':
		{
			// �÷��̾� ���� ����
			std::string player;
			stream >> player;
			if ( m_server->m_userNames.count( player ) != 0 )
			{
				std::string message = str::msg::PRINT_PLAYERINFO;
				message = message + str::msg::PRINT_PLAYERINFO_NAME + player + str::msg::PRINT_PLAYERINFO_IP +
					m_server->m_userNames[ player ]->m_ip + str::msg::PRINT_PLAYERINFO_LOCATION;
				// �κ� �ִ��� �Ǵ�
				if ( m_server->m_userNames[ player ]->m_roomNumber == 0 )
				{
					message = message + str::msg::PRINT_PLAYERINFO_LOBBY;
				}
				else 
				{ 
					message = message + std::to_string( m_server->m_userNames[ player ]->m_roomNumber ) + str::msg::PRINT_PLAYERINFO_ROOM; 
				}

				SendChat( message );
			}
			else
			{
				SendChat( str::msg::PRINT_FAILPLAYERINFO );
			}
		}
		break;
		case 'p':
		case 'P':
		{
			// �� ���� ����
			int roomNumber = 0;
			stream >> roomNumber;
			// ���� ������ ���
			if ( roomNumber != 0 && m_server->m_rooms.count( roomNumber ) != 0 )
			{
				std::string message = str::msg::PRINT_ROOMINFO;
				message = message + str::msg::PRINT_ROOMINFO_NAME + m_server->m_rooms[ roomNumber ].GetName() 
					+ str::msg::PRINT_ROOMINFO_CAPACITY + std::to_string( m_server->m_rooms[ roomNumber ].GetMaxPeople() ) + "\r\n";

				auto time = m_server->m_rooms[ roomNumber ].GetTime();
				struct tm localt;
				localtime_s( &localt, &time );
				message = message + str::msg::PRINT_ROOMINFO_PLAYER + std::to_string( m_server->m_rooms[ roomNumber ].GetChatters().size() ) + "\r\n" 
					+ str::msg::PRINT_ROOMINFO_TIME + std::to_string( localt.tm_hour ) + ":" + std::to_string( localt.tm_min ) + ":" + std::to_string( localt.tm_sec ) + "\r\n";
				for ( auto& player : m_server->m_rooms[ roomNumber ].GetChatters() )
				{
					time = player->GetTime();
					localtime_s( &localt, &time );
					message = message + player->m_name + "	" + player->m_ip + ":" + std::to_string( player->m_port ) + "	" 
						+ std::to_string( localt.tm_hour ) + ":" + std::to_string( localt.tm_min ) + ":" + std::to_string( localt.tm_sec ) + "\r\n";
				}
				SendChat( message );
			}
			else
			{
				SendChat( str::msg::PRINT_FAILROOMINFO );
			}
		}
		break;
		case 'w':
		case 'W':
		{
			// �ʴ� ���
			if ( !isInRoom )
			{
				SendChat( str::errormsg::COMMAND );
				break;
			}
			std::string receiver;
			stream >> receiver;
			if ( receiver == m_name )
			{
				SendChat( str::errormsg::SELFERROR );
				break;
			}
			// �ش� �г����� ���� ����� ������ ��� �ʴ� �޽��� ����
			if ( m_server->m_userNames.count( receiver ) != 0 )
			{
				SendChat( receiver + str::msg::INVITE );
				m_server->m_userNames[ receiver ]->SendChat( m_name + str::msg::INVITE_FROM + std::to_string( m_roomNumber ) + str::msg::INVITE_TO );
			}
			else
			{
				SendChat( str::msg::INVITE_FAIL );
			}
		}
		break;
		case 't':
		case 'T':
		{
			// �ӼӸ� ó��
			std::string receiver;
			std::string message;

			stream >> receiver;
			if ( receiver == m_name )
			{
				SendChat( str::errormsg::SELFERROR );
				break;
			}
			// �ش� �г����� ���� ����� ������ ��� �ӼӸ� ����
			if ( m_server->m_userNames.count( receiver ) != 0 )
			{
				stream.get();
				auto cur = stream.tellg();
				if ( cur == -1 )
				{
					SendChat( str::msg::WISPER_FAIL );
					break;
				}
				message = buf.substr( cur );
				message.erase( std::remove_if( message.begin(), message.end(),
					[]( char n ) 
					{
						return ( n == '\n' || n == '\r' );
					}
				), message.end() );
				SendChat( receiver + str::msg::WISPER_TO + message + "\r\n" );
				m_server->m_userNames[ receiver ]->SendChat( m_name + str::msg::WISPER_FROM + message + "\r\n" );
			}
			else
			{
				SendChat( str::msg::WISPER_FAIL );
			}

		}
		break;
		case 'q':
		case 'Q':
		{
			// �� ����
			if ( !isInRoom )
			{
				SendChat( str::errormsg::COMMAND );
				break;
			}
			SendChat( str::msg::PLAYER_QUITROOM );
			m_currentScene->ExitScene();
		}
		break;
		case 'h':
		case 'H':
		{
			std::string message = str::msg::INFO_COMMAND;
			if ( isInRoom )
			{
				message = message + str::msg::INFO_COMMAND_ROOM;
				SendChat( message );
			}
			else
			{
				message = message + str::msg::INFO_COMMAND_LOBBY;
				SendChat( message );
			}
		}
		break;
		default:
		{
			if ( isInRoom )	SendChat( str::msg::DEFAULT_ROOM );
			else			SendChat( str::msg::DEFAULT_LOBBY );
		}
		break;
		}
	}
	InitializeBuffer();
	return true;
}


void Session::InitializeBuffer()
{
	m_isProcessing = false;
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
	// ���� �����͸� ���ۿ� ����
	m_recvBytes = m_leftovers.size();
	if ( m_leftovers.size() != 0 )
	{
		memcpy_s( m_buffer, BUFFER_SIZE, m_leftovers.c_str(), m_leftovers.size() );
		m_leftovers.clear();
	}
	return;
}