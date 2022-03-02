

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
		SendChat( m_name + "�� �г������� ����մϴ�\r\n" );
		m_server->m_userNames[ m_name ] = this;
	}
	else
	{
		SendChat( "�ߺ��� �г����� �����մϴ�\r\n" );
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
	stream.str( m_buffer );
	string tmp;
	// ��Ʈ������ ��ɾ� ����
	stream >> tmp;

	bool isInRoom = (m_roomNumber != 0);
	char* ptr = &m_buffer[ 0 ];
	// �� ���� ��� / �ڸ� ��ɾ�� �ν���
	switch ( *(ptr + isInRoom) )
	{
	case 'a':
	case 'A':
	{
		// �� �����
		if ( isInRoom )
		{
			SendChat( "�߸��� ��ɾ� �Է��Դϴ�.\r\n" );
			break;
		}
		string name;
		int max = -1;
		stream >> max;
		if ( max <= 1 )
		{
			SendChat( "�� ����� ��ɾ� : a [�����ο�] [������] (�ּ� 2��)\r\n" );
			break;
		}
		stream >> name;
		if ( name.size() <= 1 )
		{
			SendChat( "�� ����� ��ɾ� : a [�����ο�] [������] (�ּ� ���� ���ڼ� 2)\r\n" );
			break;
		}
		// ������ �� ������ ��û
		m_roomNumber = m_server->MakeRoom( name, max );
		m_isInLobby = false;
		m_server->m_rooms[ m_roomNumber ].m_chatters.push_back( this );
		SystemMessage( m_server->m_rooms[ m_roomNumber ].m_chatters, m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
		m_currentScene->ChangeScene();
	}
	break;
	case 'j':
	case 'J':
	{
		// �� ����
		if ( isInRoom )
		{
			SendChat( "�߸��� ��ɾ� �Է��Դϴ�.\r\n" );
			break;
		}
		int roomNum = -1;
		stream >> roomNum;
		// ���� �����ϴ��� üũ
		if ( roomNum >= 1 && m_server->m_rooms.count( roomNum ) )
		{
			// �� �ο����� ���� á���� üũ
			if ( m_server->m_rooms[ roomNum ].m_maxPeople > m_server->m_rooms[ roomNum ].m_chatters.size() )
			{
				m_roomNumber = roomNum;
				m_isInLobby = false;
				m_server->m_rooms[ roomNum ].m_chatters.push_back( this );
				SystemMessage( m_server->m_rooms[ m_roomNumber ].m_chatters, m_name + "���� ��ȭ�濡 �����߽��ϴ�.\r\n" );
				m_currentScene->ChangeScene();
			}
			else
			{
				SendChat( "���� ���� á���ϴ�.\r\n" );
			}
		}
		else
		{
			SendChat( "�� ���� ��ɾ� : j [�� ��ȣ] (�� ���忡 �����߽��ϴ�.)\r\n" );
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
				m_currentScene->ExitScene();
				closesocket( m_socket );
			}
		}
		else
		{
			if ( *( ptr + 1 ) == '\r' && *( ptr + 2 ) == '\n' )	closesocket( m_socket );
		}
		return false;
	}
	break;
	case 'o':
	case 'O':
	{
		// �� ���
		string message = "�� ����� ����մϴ�.\r\n";
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
		// �������� �÷��̾�
		string message = "�������� �÷��̾���� ����մϴ�.\r\n";
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
		// �÷��̾� ���� ����
		string player;
		stream >> player;
		if ( m_server->m_userNames.count( player ) != 0 )
		{
			string message = "�ش� �÷��̾��� ������ ����մϴ�.\r\n";
			message = message + "�г��� : " + player + "	ip : " + m_server->m_userNames[player]->m_ip + "	��ġ : ";
			// �κ� �ִ��� �Ǵ�
			if ( m_server->m_userNames[ player ]->m_roomNumber == 0 )	message = message + "�κ�\r\n";
			else message = message + to_string( m_server->m_userNames[ player ]->m_roomNumber ) + "�� ä�ù� \r\n";

			SendChat( message );
		}
		else
		{
			SendChat( "�÷��̾� ���� ��ɾ� : i [�г���] (�ش� �г����� �������� �ʽ��ϴ�)\r\n" );
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
			string message = "�ش� ���� ������ ����մϴ�.\r\n";
			message = message + "�̸� : " + m_server->m_rooms[ roomNumber ].m_name + "	���� : " + to_string( m_server->m_rooms[ roomNumber ].m_maxPeople ) + "\r\n";
			message = message + "������ : " + to_string( m_server->m_rooms[ roomNumber ].m_chatters.size() ) + "\r\n";
			for ( auto& player : m_server->m_rooms[ roomNumber ].m_chatters )
			{
				message = message + player->m_name + "	" + player->m_ip + "\r\n";
			}
			SendChat( message );
		}
		else
		{
			SendChat( "�� ���� ��ɾ� : p [�� ��ȣ] (�ش� ä�ù��� �������� �ʽ��ϴ�)\r\n" );
		}
	}
		break;
	case 't':
	case 'T':
	{
		// �ӼӸ� ó��
		string receiver;
		string message;

		stream >> receiver;
		// �ش� �г����� ���� ����� ������ ���
		if ( m_server->m_userNames.count( receiver ) != 0 )
		{
			stream >> message;
			SendChat( receiver + "�Կ��� �ӼӸ� : " + message + "\r\n" );
			m_server->m_userNames[ receiver ]->SendChat( m_name + "���� �ӼӸ� : " + message + "\r\n" );
		}
		else
		{
			SendChat( "�ӼӸ� ��ɾ� : t [�г���] [�� ��] (�ش� �г����� �������� �ʽ��ϴ�)\r\n" );
		}

	}
		break;
	case 'q':
	case 'Q':
	{
		// �� ����
		if ( !isInRoom )
		{
			SendChat( "�߸��� ��ɾ� �Է��Դϴ�.\r\n" );
			break;
		}
		m_currentScene->ExitScene();
	}
		break;
	default:
	{
		if ( isInRoom )	SendChat( "�÷��̾� ���(L) �÷��̾� ����(i) �ӼӸ�(t) �� ���(o) �� ����(p) �� ����(q) ������(X)\r\n" );
		else			SendChat( "----------------------------------------------------\r\n�κ� ���� ���� ȯ���մϴ�\r\n----------------------------------------------------\r\n�� �����(a) �÷��̾� ���(L) �÷��̾� ����(i) �ӼӸ�(t) �� ���(o) �� ����(p) �� ����(j) ������(X)\r\n" );
	}
	break;
	}
	InitializeBuffer();
	return true;
}