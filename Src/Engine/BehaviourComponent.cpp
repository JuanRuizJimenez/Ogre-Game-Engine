#include "BehaviourComponent.h"
#include "GameObject.h"

BehaviourComponent::BehaviourComponent(GameObject* gameObject_) : BasicComponent(gameObject_)
{
	gameObject_->addBComponent(this);
}

BehaviourComponent::~BehaviourComponent()
{
}


