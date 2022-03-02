

#include "ChattingScene.h"
#include "ChattingServer.h"
#include "CommonFunctions.h"


void SystemMessage( const vector<Session*>& container, const string& message )
{
	for ( auto& user : container )
	{
		if ( !user->m_isInLobby )	user->SendChat( message );
	}
}
