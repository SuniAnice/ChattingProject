

#include "ChattingServer.h"

int ChattingServer::MakeRoom( string name, int max )
{
	for ( int i = 1; ; i++ )
	{
		if ( !m_rooms.count( i ) )
		{
			m_rooms[ i ].m_name = name;
			m_rooms[ i ].m_maxPeople = max;
			return i;
		}
	}
}
