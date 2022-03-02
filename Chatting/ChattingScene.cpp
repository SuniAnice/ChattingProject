

#include "CommonFunctions.h"
#include "ChattingServer.h"
#include "ChattingScene.h"
#include "LobbyScene.h"
#include "Session.h"


bool ChattingScene::ExecutionInput()
{
	parent->BroadcastMessage();
	return true;
}

void ChattingScene::ExitScene()
{
	vector<Session*>* chatters = &parent->m_server->m_rooms[ parent->m_roomNumber ].m_chatters;
	SystemMessage( *chatters, parent->m_name + "님이 대화방에서 나갔습니다.\r\n" );
	chatters->erase( remove(chatters->begin(), chatters->end(), parent), chatters->end() );
	if ( chatters->size() == 0 )	parent->m_server->m_rooms.erase( parent->m_roomNumber );
	parent->m_isInLobby = true;
	parent->m_roomNumber = 0;
	parent->m_currentScene = new LobbyScene( parent );
	delete this;
}
