#include "TurretBehaviour.h"
#include <GameObject.h>


TurretBehaviour::TurretBehaviour(GameObject* gameObject,GameObject* target) :BehaviourComponent(gameObject), target(target)
{
}


TurretBehaviour::~TurretBehaviour()
{
}

void TurretBehaviour::Update(float t)
{
	//Hace un lookAt pero sin hacer yaw (rotar en el eje y)
	Ogre::Vector3 dir = gameObject->getPosition() - target->getPosition();
	dir.normalise();
	Ogre::Vector3 right(dir.z, 0, -dir.x);
	right.normalise();
	Ogre::Vector3 up = dir.crossProduct(right);
	gameObject->setOrientation(Ogre::Quaternion(right, up, dir));
}

void TurretBehaviour::reciveMsg(Message * msg)
{
}
