#pragma once
#include "NodeStack.h"
#include "Scene.h"
#include <stack>
//Clase que hace de pila de escenas
class SceneMachine
{
protected:
	stack<Scene*> sceneStack;

public:
	SceneMachine();
	~SceneMachine();

	void addScene(Scene* s);
	void delScene();
	void update();
	Scene* currentScene();
};

