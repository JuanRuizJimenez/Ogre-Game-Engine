#pragma once
#include <BehaviourComponent.h>
#include "EnemyBehaviour.h"
#include <vector>

class EnemyManager : public BehaviourComponent
{
private:
	std::vector<EnemyBehaviour*> enemies_c;
	int enemies=0;
public:
	EnemyManager(GameObject* go);
	virtual ~EnemyManager();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);

	void addEnemy(EnemyBehaviour* enemy) { enemies_c.push_back(enemy); enemies++; }
};

