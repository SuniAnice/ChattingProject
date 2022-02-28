

#include "socketinfo.h"


int SocketInfo::Recv()
{
	int retVal = recv( m_socket, m_buffer, BUFFER_SIZE, 0 );
	m_recvBytes = retVal;
	return retVal;
}

int SocketInfo::Send()
{
	//int retVal = send( m_socket, m_buffer + m_sendBytes, m_recvBytes - m_sendBytes, 0 );
	//m_sendBytes += retVal;
	//if ( m_recvBytes == m_sendBytes )
	//{
	//	m_recvBytes = m_sendBytes = 0;
	//}
	return 0;
}

int SocketInfo::SendChat( const string message ) const
{
	int retVal = send( m_socket, message.c_str(), message.size(), 0 );
	return retVal;
}

