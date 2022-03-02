

#pragma once


#include "Scene.h"


class LobbyScene : public Scene
{
public:
	LobbyScene() = delete;
	LobbyScene( Session* p );
	virtual ~LobbyScene() {}
	virtual void ExecutionInput();
	virtual void ChangeScene();
	virtual void ExitScene() {}
};
