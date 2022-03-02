

#pragma once


#include "Scene.h"


class ChattingScene :public Scene
{
public:
	ChattingScene() = delete;
	ChattingScene( Session* p ) { parent = p; }
	virtual ~ChattingScene() {}
	virtual bool ExecutionInput();
	virtual void ChangeScene() {}
	virtual void ExitScene();
};
