#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"

class Vec3;
class BulletBehaviour : public BehaviourComponent
{	
private:
	Ogre::Vector3 dir_;
	Ogre::Vector3 pos_;

	int vel_ = 40;
	int mapWH = 3000;
	int index = 0;
	int damage;
	float bLifeTime;
	float bTimer;
public:
	BulletBehaviour(GameObject* gameObject, Vec3 pos, Vec3 dir, int dmg, float _bLifeTime);
	virtual ~BulletBehaviour();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
	/**@Vel is default set at 40 */
	void setVel(int nVel) { vel_ = nVel; };
	int getDamage();

	void resetValues(Vec3 pos, Vec3 dir);
};

