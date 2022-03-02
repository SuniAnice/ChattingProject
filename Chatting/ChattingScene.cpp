

#include "CommonFunctions.h"
#include "ChattingServer.h"
#include "ChattingScene.h"
#include "LobbyScene.h"
#include "Session.h"


void ChattingScene::ExecutionInput()
{
	parent->BroadcastMessage();
}

void ChattingScene::ExitScene()
{
	vector<Session*>* chatters = &parent->m_server->m_rooms[ parent->m_roomNumber ].m_chatters;
	BroadcastMessage( *chatters, parent->m_name + "���� ��ȭ�濡�� �������ϴ�.\r\n" );
	chatters->erase( remove(chatters->begin(), chatters->end(), parent), chatters->end() );
	if ( chatters->size() == 0 )	parent->m_server->m_rooms.erase( parent->m_roomNumber );
	parent->m_isInLobby = true;
	parent->m_currentScene = new LobbyScene( parent );
	delete this;
}
