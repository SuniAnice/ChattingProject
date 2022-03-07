

#include "ChattingScene.h"
#include "LobbyScene.h"
#include "Session.h"
#include "StringTable.h"


LobbyScene::LobbyScene( Session* p ) : Scene( p )
{
	// �κ� �޽��� ��¿�
	parent->SendChat( str::msg::DEFAULT_LOBBY );
}

bool LobbyScene::ExecutionInput()
{
	// ������� �Է°��� �������� ��ɾ� ó��, ���ϰ��� false�� ���� ����ڰ� ���Ḧ �Է��� ���.
	return parent->ProcessCommand();
}

void LobbyScene::ChangeScene()
{
	// ���� ä�ù����� ����
	parent->SetScene( std::make_unique< ChattingScene >( parent ) );
}
