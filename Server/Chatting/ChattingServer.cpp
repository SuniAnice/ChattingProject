

#include "ChattingRoom.h"
#include "ChattingServer.h"
#include "Session.h"
#include <ctime>

int ChattingServer::MakeRoom( const std::string& name, int max )
{
	for ( int i = 1; ; i++ )
	{
		if ( !m_rooms.count( i ) )
		{
			m_rooms[ i ].SetName( name );
			m_rooms[ i ].SetMaxPeople( max );
			m_rooms[ i ].SetTime( time( NULL ) );
			return i;
		}
	}
}

void ChattingServer::SystemMessage( const std::vector< Session* >& container, const std::string& message )
{
	for ( auto& user : container )
	{
		user->SendChat( message );
	}
}
