#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Physics.h>
#include <SceneMachine.h>
#include "BulletBehaviour.h"
#include "Messages.h"

enum enemyType {
	groundTurret,
	Flyer
};

class EnemyShoot: public BehaviourComponent
{
protected:
	enemyType type;
	std::vector<GameObject*>bullets;
	std::vector<BulletBehaviour*> bulletComponents;
	std::string bulletMeshName;

	GameObject* target;

	float ShootCd = 1,
		auxTemp = 0;
	float range;
	int damage;
	float bLifeTime;
public:
	EnemyShoot(GameObject* go, enemyType type_, GameObject* target_, int _damage, int _range, std::string bulletMesh, float shootCd, float _bLifeTime);
	virtual ~EnemyShoot();
	void changeRange(float nRange) { range = nRange; };
	void shoot();

	bool inRange();

	void situateBullet(GameObject*b, int id, bool created = false,int i =-1);
	
	void getBullet(int id=0);

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

