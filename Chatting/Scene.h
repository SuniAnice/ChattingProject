

#pragma once


#include <vector>


using namespace std;


class Session;

class Scene
{
public:
	Session* parent;
	Scene() {}
	Scene( Session* p ) : parent( p ) {}
	virtual ~Scene() {}
	virtual void ExecutionInput( vector < Session* > container ) = 0;
	virtual void ChangeScene() = 0;
};

