

#pragma once


#include "Scene.h"


class LobbyScene : public Scene
{
public:
	LobbyScene( Session* p );
	virtual ~LobbyScene() {}
	virtual void ExecutionInput( vector < Session* > container );
	virtual void ChangeScene();
};

