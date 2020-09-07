#pragma once
#include <BehaviourComponent.h>
#include <SoundManager.h>

class GameManager : public BehaviourComponent
{
private:
	bool GameOver;
	bool nextLevel;
	static int currentLevel;
	int maxLevel;
	float nextLevelWaitTime;
	float nextLevelTimer;
	bool victory;

public:
	GameManager(GameObject* go, int _maxLevel, float _nextLevelWaitTime);
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
	virtual ~GameManager();
};

