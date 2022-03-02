

#pragma once


#include "Scene.h"


class ChattingScene : public Scene
{
public:
	ChattingScene() = delete;
	ChattingScene( Session* p );
	virtual ~ChattingScene() {}
	virtual bool ExecutionInput();
	virtual void ChangeScene() {}
	virtual void ExitScene();
};
