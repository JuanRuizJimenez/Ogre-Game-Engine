#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"
class TurretBehaviour :
	public BehaviourComponent
{
private:
	GameObject* target;
public:
	TurretBehaviour(GameObject* gameObject, GameObject* target);
	virtual ~TurretBehaviour();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

