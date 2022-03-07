

#include "ChattingServer.h"
#include "LobbyScene.h"
#include "LoginScene.h"
#include "Session.h"
#include "StringTable.h"
#include <memory>


LoginScene::LoginScene( Session* p ) : Scene( p )
{
	p->SendChat( str::msg::PLAYER_ENTERLOGINSCENE );
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
	parent->SetScene( std::make_unique< LobbyScene >( parent ) );
}
