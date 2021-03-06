

#include "ChattingScene.h"
#include "LobbyScene.h"
#include "Session.h"
#include "StringTable.h"


LobbyScene::LobbyScene( Session* p ) : Scene( p )
{
	// 로비 메시지 출력용
	parent->SendChat( str::msg::DEFAULT_LOBBY );
}

bool LobbyScene::ExecutionInput()
{
	// 사용자의 입력값을 바탕으로 명령어 처리, 리턴값이 false인 경우는 사용자가 종료를 입력한 경우.
	return parent->ProcessCommand();
}

void LobbyScene::ChangeScene()
{
	// 씬을 채팅방으로 변경
	parent->ChangeToChattingScene();
}
