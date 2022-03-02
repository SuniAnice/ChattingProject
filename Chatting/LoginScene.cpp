

#include "LobbyScene.h"
#include "LoginScene.h"
#include "Session.h"

LoginScene::LoginScene( Session* p )
{
	p->SendChat( "안녕하세요. 채팅 서버에 오신 것을 환영합니다.\r\n" );
	p->SendChat( "사용하실 닉네임을 입력해주세요.\r\n" );
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
