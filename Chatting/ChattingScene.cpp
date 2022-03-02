

#include "CommonFunctions.h"
#include "ChattingScene.h"
#include "LobbyScene.h"
#include "Session.h"


void ChattingScene::ExecutionInput()
{
	parent->BroadcastMessage();
}

void ChattingScene::ExitScene()
{
	BroadcastMessage( *parent->m_container, parent->m_name + "���� ��ȭ�濡�� �������ϴ�.\r\n" );
	parent->m_isInLobby = true;
	parent->m_currentScene = new LobbyScene( parent );
	delete this;
}
