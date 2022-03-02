

#include "ChattingScene.h"
#include "LobbyScene.h"
#include "Session.h"


LobbyScene::LobbyScene( Session* p )
{
	parent = p;
	// �κ� �޽��� ��¿�
	parent->ProcessCommand();
}

bool LobbyScene::ExecutionInput()
{
	// ������� �Է°��� �������� ��ɾ� ó��, ���ϰ��� false�� ���� ����ڰ� ���Ḧ �Է��� ���.
	return parent->ProcessCommand();
}

void LobbyScene::ChangeScene()
{
	// ���� ä�ù����� ����
	parent->m_currentScene = new ChattingScene( parent );
	delete this;
}
