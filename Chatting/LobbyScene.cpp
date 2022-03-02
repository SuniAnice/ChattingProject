

#include "ChattingScene.h"
#include "CommonFunctions.h"
#include "LobbyScene.h"
#include "Session.h"


LobbyScene::LobbyScene( Session* p )
{
	parent = p;
	p->SendChat( "----------------------------------------------------\r\n�κ� ���� ���� ȯ���մϴ�\r\n----------------------------------------------------\r\n�� ����(A) �÷��̾� ���(L) ������(X)\r\n" );
}

void LobbyScene::ExecutionInput( vector < Session* > container )
{
	ProcessCommand( container, *parent );
}

void LobbyScene::ChangeScene()
{
	parent->m_currentScene = new ChattingScene( parent );
	delete this;
}

