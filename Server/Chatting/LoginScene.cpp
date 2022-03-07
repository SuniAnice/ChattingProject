

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
	// 닉네임 설정에 성공했으면 씬 변경
	if ( parent->SetName() )
	{
		ChangeScene();
	}
	return true;
}

void LoginScene::ChangeScene()
{
	// 로그인에 성공하면 로비로 씬 전환
	parent->SetScene( std::make_unique< LobbyScene >( parent ) );
}
