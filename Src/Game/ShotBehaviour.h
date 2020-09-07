#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Physics.h>
#include <SceneMachine.h>
#include "BulletBehaviour.h"
#include "Messages.h"

class ShotBehaviour : public BehaviourComponent
{
private:
	OIS::Keyboard* keyboard;
	Scene* scn;
	int damage;

	int bulletCount = 0;
	float cooldown = 1.0/7.0,
		currentCd = 0;
	bool keyDown = false;
	//controladores para la sobrecarga
	float maxOverload = 10,
		cOverload = 0;
	float ovRechargeMultiplier = 1.0;
	float bLifeTime;
	bool overloaded = false;
	bool isRolling = false;
	std::string shipName_;
	std::string bulletMeshName;
	std::string bulletParticleCollisionName;

	std::vector<GameObject*> bullets_;
	std::vector<BulletBehaviour*> bComponents_;

public:
	ShotBehaviour(GameObject* gameObject, std::string shipName, int damage, int maxOverload_, float shotCD_, float ovRechargeMultiplier_, float _bLifeTime);
	virtual ~ShotBehaviour();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);

	void getBullet(int id);
	void shoot();
	void situateBullet(GameObject*b, int id, bool created = false, int i = -1);
};

