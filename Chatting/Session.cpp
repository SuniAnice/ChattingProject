

#include "Session.h"


int Session::Recv()
{
	int retVal = recv( m_socket, m_buffer + m_recvBytes, BUFFER_SIZE - m_recvBytes, 0 );
	m_recvBytes += retVal;
	if ( m_buffer[ m_recvBytes - 1 ] == '\n' )
	{
		// 개행 문자이면 패킷 처리
		if (!m_isNameSet)
		{
			SetName();
		}
		m_isProcessing = true;
	}
	return retVal;
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
	m_isNameSet = true;
	m_isInLobby = true;
	InitializeBuffer();

	return true;
}

void Session::BroadcastMessage( const vector<Session*> container)
{
	string chatting = m_name + " : " + m_buffer;
	for ( auto& user : container )
	{
		if ( !user->m_isInLobby )	user->SendChat( chatting );
	}
	InitializeBuffer();
}

void Session::ProcessCommand()
{
	switch ( m_buffer[ 0 ] )
	{
	case 'a':
	case 'A':
		m_isInLobby = false;
		break;
	case 'X':
	case 'x':
		if ( m_buffer[ 1 ] == '\r' && m_buffer[ 2 ] == '\n' )	closesocket( m_socket );
		break;
	default:
		SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n방 입장(A) 나가기(X)\r\n" );
		break;
	}
	InitializeBuffer();
}

void Session::InitializeBuffer()
{
	m_recvBytes = 0;
	m_isProcessing = false;
	ZeroMemory( m_buffer, BUFFER_SIZE + 1 );
}
