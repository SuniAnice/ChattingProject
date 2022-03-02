

#pragma once


#include "Scene.h"


class LoginScene : public Scene
{
public:
	LoginScene( Session* p ) { parent = p; }
	virtual ~LoginScene() {}
	virtual void ExecutionInput( vector < Session* > container );
	virtual void ChangeScene();
};


