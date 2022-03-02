

#include "ChattingScene.h"
#include "Session.h"


void ChattingScene::ExecutionInput()
{
	parent->BroadcastMessage();
}

void ChattingScene::ExitScene()
{
}
