

#include "ChattingScene.h"
#include "CommonFunctions.h"
#include "LobbyScene.h"
#include "Session.h"


LobbyScene::LobbyScene( Session* p )
{
	parent = p;
	ProcessCommand( *parent );
}

void LobbyScene::ExecutionInput()
{
	ProcessCommand( *parent );
}

void LobbyScene::ChangeScene()
{
	parent->m_currentScene = new ChattingScene( parent );
	delete this;
}
