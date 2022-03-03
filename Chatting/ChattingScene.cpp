

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
	// ������� �Է��� ù ���ڰ� /�� ���� ��ɾ� ó�� �ܰ��
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
	// �÷��̾ ä�ù濡�� �κ�� ���ư� ����� ó��, ���� ��� �÷��̾�� ���� �޽��� ���� �� �����̳ʿ��� ����
	std::vector<Session*>* chatters = &parent->GetServer()->m_rooms[ parent->GetRoomNumber() ].GetChatters();
	parent->GetServer()->SystemMessage( *chatters, parent->GetName() + str::msg::PLAYER_EXITROOM.data() );
	chatters->erase( remove(chatters->begin(), chatters->end(), parent), chatters->end() );
	if ( chatters->size() == 0 )	parent->GetServer()->m_rooms.erase( parent->GetRoomNumber() );
	parent->SetIsInLobby(true);
	parent->SetRoomNumber(0);
	parent->SetScene( std::make_shared< LobbyScene >( parent ) );
}
