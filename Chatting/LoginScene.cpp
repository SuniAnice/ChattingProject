

#include "ChattingServer.h"
#include "LobbyScene.h"
#include "LoginScene.h"
#include "Session.h"

LoginScene::LoginScene( Session* p )
{
	p->SendChat( "안녕하세요. 채팅 서버에 오신 것을 환영합니다.\r\n" );
	p->SendChat( "사용하실 닉네임을 입력해주세요.\r\n" );
	parent = p;
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
	parent->SetScene( new LobbyScene( parent ) );
	delete this;
}
