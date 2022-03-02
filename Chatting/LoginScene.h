

#pragma once


#include "Scene.h"


class LoginScene : public Scene
{
public:
	LoginScene() = delete;
	LoginScene( Session* p );
	virtual ~LoginScene() {}
	virtual void ExecutionInput();
	virtual void ChangeScene();
	virtual void ExitScene() {}
};
