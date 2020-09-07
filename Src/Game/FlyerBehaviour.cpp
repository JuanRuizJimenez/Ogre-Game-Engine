#include "FlyerBehaviour.h"
#include <GameObject.h>

FlyerBehaviour::FlyerBehaviour(GameObject* go, GameObject* target_, FlyerRoute route_, float radius_, float height_): BehaviourComponent(go), target(target_),
								radius(radius_), height(height_), route(route_){
	//centre = go->getGlobalPosition();
	cx = go->getGlobalPosition().x;
	cy = go->getGlobalPosition().y;
	cz = go->getGlobalPosition().z;
}

FlyerBehaviour::~FlyerBehaviour() {
	target = nullptr;
}

void FlyerBehaviour::Update(float t) {
	timePassed += t;
	if (timePassed >= 360)
		timePassed -= 360;

	Vec3 newPos;
	//la coordenada y es la altura
	switch (route)
	{
	case XCircular:
		newPos = Vec3(cx, cy + (Math::Sin(timePassed)*radius) / 2, cz + (Math::Cos(timePassed)*radius) / 2);
		break;
	case YCircular:
		newPos = Vec3(cx + (Math::Sin(timePassed) * radius) / 2, cy, cz + (Math::Cos(timePassed)*radius) / 2);
		break;
	case ZCircular:
		newPos = Vec3(cx + (Math::Sin(timePassed) * radius) / 2, cy + (Math::Cos(timePassed)*radius)/2, cz);
		break;
	case Vertical:
		newPos = Vec3(cx, cy + (Math::Sin(timePassed)*height), cz);
		break;
	case Horizontal:
		newPos = Vec3(cx + (Math::Sin(timePassed)*height), cy , cz);
		break;
	case Transversal:
		newPos = Vec3(cx, cy, cz + (Math::Sin(timePassed)*height));
		break;
	case Sinusoidal:
		newPos = Vec3(cx + (Math::Sin(timePassed) * radius) / 2, cy + (Math::Sin(timePassed*2.5)*height), cz + (Math::Cos(timePassed)*radius) / 2);
		break;
	default:
		break;
	}

	gameObject->setPosition(newPos);
	//Hace un lookAt pero sin hacer yaw (rotar en el eje y)
	Ogre::Vector3 dir = gameObject->getPosition() - target->getPosition();
	dir.normalise();
	Ogre::Vector3 right(dir.z, 0, -dir.x);
	right.normalise();
	Ogre::Vector3 up = dir.crossProduct(right);
	gameObject->setOrientation(Ogre::Quaternion(right, up, dir));
}

void FlyerBehaviour::reciveMsg(Message* msg) {

}