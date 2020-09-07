#ifndef _BEHAVIOURCOMPONENT_H
#define _BEHAVIOURCOMPONENT_H

#include "BasicComponent.h"

class BehaviourComponent : public BasicComponent
{
public:
	BehaviourComponent(GameObject* gameObject_);
	virtual ~BehaviourComponent();

	virtual void Update(float t)=0;

};

#endif // _BEHAVIOURCOMPONENT

