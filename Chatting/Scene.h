

#pragma once


using namespace std;


class Session;

class Scene
{
protected:
	Session* parent;
public:
	Scene() = delete;
	Scene( Session* p );
	virtual ~Scene() {}
	// ������� �Է��� ó���ϴ� �Լ�
	virtual bool ExecutionInput() = 0;
	// ���� �����ϴ� �Լ�
	virtual void ChangeScene() = 0;
	// ���� ������ ������ �Լ�
	virtual void ExitScene() = 0;
};
