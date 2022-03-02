

#include "ChattingRoom.h"
#include "ChattingServer.h"
#include "Session.h"

int ChattingServer::MakeRoom( const string& name, int max )
{
	for ( int i = 1; ; i++ )
	{
		if ( !m_rooms.count( i ) )
		{
			m_rooms[ i ].SetName( name );
			m_rooms[ i ].SetMaxPeople( max );
			return i;
		}
	}
}

void ChattingServer::SystemMessage( const vector< Session* >& container, const string& message )
{
	for ( auto& user : container )
	{
		user->SendChat( message );
	}
}
