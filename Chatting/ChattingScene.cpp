

#include "ChattingScene.h"
#include "Session.h"


void ChattingScene::ExecutionInput( vector<Session*> container )
{
	parent->BroadcastMessage( container );
}
