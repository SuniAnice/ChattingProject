

#include "Session.h"


int Session::Recv()
{
	int retVal = recv( m_socket, m_buffer + m_recvBytes, BUFFER_SIZE, 0 );
	m_recvBytes += retVal;
	if (m_buffer[m_recvBytes - 1] == '\n')
	{
		m_isProcessing = true;
	}
	return retVal;
}

int Session::Send()
{
	int retVal = send( m_socket, m_buffer, m_recvBytes, 0 );
	m_sendBytes += retVal;
	if ( m_recvBytes == m_sendBytes )
	{
		m_recvBytes = m_sendBytes = 0;
		m_isProcessing = false;
	}
	return 0;
}

int Session::SendChat( const string message ) const
{
	int retVal = send( m_socket, message.c_str(), message.size(), 0 );
	return retVal;
}

