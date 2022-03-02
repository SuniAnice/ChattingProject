

#pragma once


#include "Scene.h"


class ChattingScene :public Scene
{
public:
	ChattingScene( Session* p ) { parent = p; }
	virtual ~ChattingScene() {}
	virtual void ExecutionInput( vector < Session* > container );
	virtual void ChangeScene() {}
};
