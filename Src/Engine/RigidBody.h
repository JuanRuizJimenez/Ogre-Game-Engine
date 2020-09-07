#ifndef _RIGIDBODY_H
#define _RIGIDBODY_H

#include "BasicComponent.h"
#include"Physics.h"
#include<Ogre.h>

typedef void(*OnCollisionCallback)(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints);

class RigidBody : public BasicComponent
{
protected:
	std::string name;
	btRigidBody* rigidBody;
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btScalar mass;
	bool isKinematic;

	//callback para que cada objeto pueda definir su metodo de colision
	OnCollisionCallback ocb;

	void setIniConf();
public:
	///constructora por defecto que deja la masa a 0 (objeto inamovible)
	RigidBody(GameObject* gameObject_, std::string name_);
	///constructora con masa por parametro
	RigidBody(GameObject* gameObject_, btScalar mass_, std::string name_, bool isKinematic_ = false);
	///contructora que calcula la masa propia con una densisad
	RigidBody(GameObject* gameObject_, std::string name_, float density_, bool isKinematic_ = false);
	void setName(const std::string newName);
	void setMass(const btScalar nmass_);
	void setActive(bool _active);
	bool isActive() { return active; };

	void setOffset(float x=0, float y=0, float z=0);

	void setGravity(btScalar x, btScalar y, btScalar z) { if (rigidBody != NULL) rigidBody->setGravity(btVector3(x,y,z)); };
	void setRigidBodyScale(btScalar x, btScalar y, btScalar z);

	virtual void reciveMsg(Message* msg_) {};
	virtual ~RigidBody();
	virtual void Update(float t);

	void setCollisionCallback(OnCollisionCallback cb) { ocb = cb; };
	virtual void onCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints);
};

#endif // _RIGIDBODY