

#pragma once


using namespace std;


class Session;

class Scene
{
public:
	Session* parent;
	Scene() {}
	Scene( Session* p );
	virtual ~Scene() {}
	// 사용자의 입력을 처리하는 함수
	virtual bool ExecutionInput() = 0;
	// 씬을 변경하는 함수
	virtual void ChangeScene() = 0;
	// 현재 씬에서 나오는 함수
	virtual void ExitScene() = 0;
};
