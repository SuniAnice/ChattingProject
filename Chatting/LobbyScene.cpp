

#include "ChattingScene.h"
#include "CommonFunctions.h"
#include "LobbyScene.h"
#include "Session.h"


LobbyScene::LobbyScene( Session* p )
{
	parent = p;
	p->SendChat( "----------------------------------------------------\r\n로비에 오신 것을 환영합니다\r\n----------------------------------------------------\r\n방 입장(A) 플레이어 목록(L) 나가기(X)\r\n" );
}

void LobbyScene::ExecutionInput()
{
	ProcessCommand( *parent->m_container, *parent );
}

void LobbyScene::ChangeScene()
{
	parent->m_currentScene = new ChattingScene( parent );
	delete this;
}
