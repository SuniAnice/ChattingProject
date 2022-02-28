

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

int Session::Send()
{
	string chatting;
	chatting = m_name + " : " + m_buffer;
	int retVal = send( m_socket, chatting.c_str(), chatting.size(), 0 );
	if (chatting.size() == retVal)
	{
		m_recvBytes = 0;
		m_isProcessing = false;
		ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
	}
	return 0;
}

int Session::SendChat( const string message ) const
{
	int retVal = send( m_socket, message.c_str(), message.size(), 0 );
	return retVal;
}

bool Session::SetName()
{
	m_name = m_buffer;
	m_name = m_name.substr( 0, m_name.size() - 2 );
	m_recvBytes = 0;
	m_isProcessing = false;
	m_isNameSet = true;
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );

	return true;
}

