

#include "ChattingScene.h"
#include "LobbyScene.h"
#include "Session.h"


LobbyScene::LobbyScene( Session* p )
{
	parent = p;
	parent->ProcessCommand();
}

bool LobbyScene::ExecutionInput()
{
	return parent->ProcessCommand();
}

void LobbyScene::ChangeScene()
{
	parent->m_currentScene = new ChattingScene( parent );
	delete this;
}
