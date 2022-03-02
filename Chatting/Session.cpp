

#include "ChattingServer.h"
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
