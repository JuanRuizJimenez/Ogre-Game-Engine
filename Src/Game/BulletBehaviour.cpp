#include "BulletBehaviour.h"
#include <GameObject.h>

BulletBehaviour::BulletBehaviour(GameObject* gameObject, Vec3 pos, Vec3 dir, int dmg, float _bLifeTime) : BehaviourComponent(gameObject), damage(dmg),bLifeTime(_bLifeTime)
{
	pos_.x = pos.x; pos_.y = pos.y; pos_.z = pos.z;
	dir_.x = dir.x; dir_.y = dir.y; dir_.z = dir.z;
	dir_.normalise();
	bTimer = bLifeTime;
}

BulletBehaviour::~BulletBehaviour()
{

}

void BulletBehaviour::Update(float t)
{
	bTimer -= t;

	pos_ += dir_ * vel_;
	gameObject->setPosition(pos_);

	if (pos_.x > mapWH || pos_.x < -mapWH || pos_.z > mapWH || pos_.z < -mapWH) {
		gameObject->setActive(false);
	}
	else if (bTimer <= 0) {
		gameObject->setActive(false);
	}
}

int BulletBehaviour::getDamage()
{
	return damage;
}

void BulletBehaviour::resetValues(Vec3 pos, Vec3 dir)
{
	pos_.x = pos.x; pos_.y = pos.y; pos_.z = pos.z;
	dir_.x = dir.x; dir_.y = dir.y; dir_.z = dir.z;
	bTimer = bLifeTime;
}