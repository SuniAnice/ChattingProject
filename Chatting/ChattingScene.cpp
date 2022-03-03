

#include "ChattingServer.h"
#include "ChattingScene.h"
#include "ChattingRoom.h"
#include "LobbyScene.h"
#include "Scene.h"
#include "Session.h"
#include "StringTable.h"
#include <memory>


ChattingScene::ChattingScene( Session* p ) : Scene( p ) {}

bool ChattingScene::ExecutionInput()
{
	// 사용자의 입력의 첫 글자가 /인 경우는 명령어 처리 단계로
	if ( parent->GetBuffer()[0] != '/' )
	{
		parent->BroadcastMessage();
		return true;
	}
	else
	{
		return parent->ProcessCommand();
	}
}

void ChattingScene::ExitScene()
{
	// 플레이어가 채팅방에서 로비로 돌아갈 경우의 처리, 방의 모든 플레이어에게 퇴장 메시지 전송 후 컨테이너에서 제거
	std::vector<Session*>* chatters = &parent->GetServer()->m_rooms[ parent->GetRoomNumber() ].GetChatters();
	parent->GetServer()->SystemMessage( *chatters, parent->GetName() + str::msg::PLAYER_EXITROOM.data() );
	chatters->erase( remove(chatters->begin(), chatters->end(), parent), chatters->end() );
	if ( chatters->size() == 0 )	parent->GetServer()->m_rooms.erase( parent->GetRoomNumber() );
	parent->SetIsInLobby(true);
	parent->SetRoomNumber(0);
	parent->SetScene( std::make_shared< LobbyScene >( parent ) );
}
