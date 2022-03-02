

#include "LobbyScene.h"
#include "LoginScene.h"
#include "Session.h"

LoginScene::LoginScene( Session* p )
{
	p->SendChat( "�ȳ��ϼ���. ä�� ������ ���� ���� ȯ���մϴ�.\r\n" );
	p->SendChat( "����Ͻ� �г����� �Է����ּ���.\r\n" );
	parent = p;
}

void LoginScene::ExecutionInput()
{
	parent->SetName();
	ChangeScene();
}

void LoginScene::ChangeScene()
{
	parent->m_currentScene = new LobbyScene( parent );
	delete this;
}
