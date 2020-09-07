#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"

enum FlyerRoute {
	XCircular,
	YCircular,
	ZCircular,
	Vertical,
	Horizontal,
	Transversal,
	Sinusoidal,
};

class FlyerBehaviour :
	public BehaviourComponent
{
private:
	FlyerRoute route;
	GameObject* target;
	//Vec3 centre;
	float cx,
		cy,
		cz;
	float radius,
		height;
	//controlador para calcular funciones senoidales
	float timePassed = 0;
public:
	/**/
	FlyerBehaviour(GameObject* gameObject, GameObject* target,FlyerRoute route_, float radius_ = 0, float height_ =0);
	virtual ~FlyerBehaviour();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};
