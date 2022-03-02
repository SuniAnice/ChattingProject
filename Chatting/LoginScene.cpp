

#include "LobbyScene.h"
#include "LoginScene.h"
#include "Session.h"

void LoginScene::ExecutionInput( vector < Session* > container )
{
	parent->SetName();
	ChangeScene();
}

void LoginScene::ChangeScene()
{
	parent->m_currentScene = new LobbyScene( parent );
	delete this;
}
