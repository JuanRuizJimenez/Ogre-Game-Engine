#include "SceneMachine.h"

SceneMachine::SceneMachine()
{
}

SceneMachine::~SceneMachine()
{
	while (!sceneStack.empty()) {
		delete sceneStack.top();
		sceneStack.pop();
	}
}

void SceneMachine::addScene(Scene* s) {
	sceneStack.push(s);
}

void SceneMachine::delScene() {
	sceneStack.pop();
}

void SceneMachine::update() {
	sceneStack.top()->updateScene();;
}

Scene * SceneMachine::currentScene()
{
	return sceneStack.top();
}
