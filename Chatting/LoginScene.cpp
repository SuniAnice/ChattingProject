

#include "ChattingServer.h"
#include "LobbyScene.h"
#include "LoginScene.h"
#include "Session.h"

LoginScene::LoginScene( Session* p )
{
	p->SendChat( "�ȳ��ϼ���. ä�� ������ ���� ���� ȯ���մϴ�.\r\n" );
	p->SendChat( "����Ͻ� �г����� �Է����ּ���.\r\n" );
	parent = p;
}

bool LoginScene::ExecutionInput()
{
	// �г��� ������ ���������� �� ����
	if ( parent->SetName() )
	{
		ChangeScene();
	}
	return true;
}

void LoginScene::ChangeScene()
{
	// �α��ο� �����ϸ� �κ�� �� ��ȯ
	parent->SetScene( new LobbyScene( parent ) );
	delete this;
}
