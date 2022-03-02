

#pragma once


#include <vector>


using namespace std;


class Session;

class Scene
{
public:
	Session* parent;
	Scene() {}
	Scene( Session* p );
	virtual ~Scene() {}
	virtual bool ExecutionInput() = 0;
	virtual void ChangeScene() = 0;
	virtual void ExitScene() = 0;
};
