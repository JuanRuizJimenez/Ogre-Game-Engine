#include "RigidBody.h"
#include"GameObject.h"

#include<OgreMeshManager.h>
#include<OgreMesh.h>

RigidBody::RigidBody(GameObject* gameObject_, std::string name_) : BasicComponent(gameObject_), name(name_)
{
	mass = 0;
	setIniConf();
}

RigidBody::RigidBody(GameObject* gameObject_, btScalar mass_, std::string name_, bool mov) : BasicComponent(gameObject_), mass(mass_), name(name_), isKinematic(mov)
{
	setIniConf();
}

RigidBody::RigidBody(GameObject* gameObject_, std::string name_, float density, bool mov) : BasicComponent(gameObject_), name(name_), isKinematic(mov)
{
	//masa = dimension del gameObject * densidad establecida
	Vec3 scale = gameObject->getBoundingBox();
	mass = scale.x * scale.y * scale.z * density;
	setIniConf();
	rigidBody->setRestitution(0);
}

RigidBody::~RigidBody()
{
	Physics::getInstance()->removeRigidBody(rigidBody);
	delete shape;
	delete motionState;
	delete rigidBody;
}

void RigidBody::setIniConf() {
	gameObject->addRigidbody(this);
	//forma del collider en funcion de la bounding box del GO
	Vec3 scale = gameObject->getBoundingBox();
	btVector3 auxScale{ btScalar(scale.x * 0.5), btScalar(scale.y * 0.5), btScalar(scale.z * 0.5) };
	//de momento solo haremos collider con forma de cubos
	shape = new btBoxShape(auxScale);

	//posicion y rotacion
	btTransform startTransform;
	startTransform.setIdentity();

	//rotation del collider para que este a la par que la del GO
	btQuaternion auxDir{ gameObject->getYaw(), gameObject->getPitch(), gameObject->getRoll() };
	startTransform.setRotation(auxDir);
	//inercia inicial por defecto a 0
	btVector3 localInertia{ 0,0,0 };
	
	shape->calculateLocalInertia(mass, localInertia);
	btVector3 auxPos{ gameObject->getGlobalPosition().x, gameObject->getGlobalPosition().y, gameObject->getGlobalPosition().z };
	startTransform.setOrigin(auxPos);

	//estado inicial del cuerpo
	motionState = new btDefaultMotionState(startTransform);
	//construimos el rigidbody con toda la informacion 
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, localInertia);
	rigidBody = new btRigidBody(rbInfo);
	rigidBody->setRestitution(1);
	rigidBody->setUserPointer(gameObject);

	if (isKinematic)
		rigidBody->setGravity(btVector3(0, 0, 0));

	//a�adimos el cuerpo al mundo fisico
	Physics::getInstance()->addRigidBodyToWorld(rigidBody, name);
}

void RigidBody::setName(const std::string newName) {
	name = newName;
}

void RigidBody::setMass(const btScalar nmass_) {
	//necesitamos la inercia local
	btVector3 localInertia = rigidBody->getLocalInertia();
	mass = nmass_;
	//asignamos al rigidbody
	rigidBody->setMassProps(mass, localInertia);
}
void RigidBody::setActive(bool _active) {
	active = _active;
}

void RigidBody::setOffset(float x, float y, float z)
{
	btTransform aux = rigidBody->getWorldTransform();
	btVector3 newPos = aux.getOrigin() + btVector3(x,y,z);
	aux.setOrigin(newPos);
	rigidBody->setWorldTransform(aux);
}

void RigidBody::Update(float t)
{
	//posicion del body
	btTransform trans;
	//condicion de objetos movibles
	if (rigidBody && rigidBody->getMotionState()) {
		//objetos cinematicos, el RB domina el movimiento del GO
		if (!isKinematic) {
			rigidBody->getMotionState()->getWorldTransform(trans);
			//control sobre el gameObject
			btQuaternion rotation = trans.getRotation();
			gameObject->setPosition(Vec3(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z()));
			gameObject->setOrientation(Ogre::Quaternion(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ()));
		}
		//objetos kinematicos, el movimiento del GO domina al RB
		else {
			//conseguimos los datos del rb
			rigidBody->getMotionState()->getWorldTransform(trans);
			//actualizamos el transfrom,
			trans.setRotation(btQuaternion(gameObject->getYaw(), gameObject->getPitch(), gameObject->getRoll()));
			trans.setOrigin(btVector3(gameObject->getGlobalPosition().x, gameObject->getGlobalPosition().y, gameObject->getGlobalPosition().z));
			//seteamos el rigidBody ya actualizado
			rigidBody->setWorldTransform(trans);
		}
	}
	//si estamos debugeando pintamos la caja del rb
	if (Physics::getInstance()->getDebugState()) {
		btDynamicsWorld* dw = Physics::getInstance()->getDynamicWorld();
		btTransform wt = rigidBody->getWorldTransform();
		btCollisionShape* cs = rigidBody->getCollisionShape();
		btVector3 colour = { 0, 1, 0 };
		dw->debugDrawObject(wt, cs, colour);
	}
}

void RigidBody::onCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints) {
	//si esta definido el callback
	if (0 != ocb) {
		//lo llamamos
		(*ocb)(one, other, contactPoints);
	}
}

void RigidBody::setRigidBodyScale(btScalar x, btScalar y, btScalar z) {
	//actualizamos caja de colision
	Vec3 scale = gameObject->getBoundingBox();
	btVector3 auxScale{ btScalar(scale.x * 0.5 * x), btScalar(scale.y * 0.5 * y), btScalar(scale.z * 0.5 * z) };
	delete rigidBody->getCollisionShape();
	shape = new btBoxShape(auxScale);
	rigidBody->setCollisionShape(shape);
}


