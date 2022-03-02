

#include "Session.h"


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
	m_name = m_buffer;
	m_name = m_name.substr( 0, m_name.size() - 2 );
	m_isNameSet = true;
	SendChat( m_name + "을 닉네임으로 사용합니다\r\n" );

	InitializeBuffer();

	return true;
}

void Session::BroadcastMessage()
{
	string chatting = m_name + " : " + m_buffer;
	for ( auto& user : *m_container )
	{
		if ( !user->m_isInLobby )	user->SendChat( chatting );
	}
	InitializeBuffer();
}

void Session::InitializeBuffer()
{
	m_recvBytes = 0;
	m_isProcessing = false;
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
}
