#include "BasicComponent.h"
#include "GameObject.h"
#include "Scene.h"

BasicComponent::BasicComponent(GameObject * gameObject_): gameObject(gameObject_)
{
}

BasicComponent::~BasicComponent()
{
}

void BasicComponent::sendObjectMsg(Message * msg_)
{
	gameObject->reciveMsg(msg_);
}

void BasicComponent::sendSceneMsg(Message * msg_)
{
	gameObject->getScene()->reciveMsg(msg_);
}

bool BasicComponent::isGOActive() { return gameObject->isActive(); }

GameObject* BasicComponent::getGameObject() { return gameObject; }