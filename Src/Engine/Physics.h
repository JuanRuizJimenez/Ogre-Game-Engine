#include<btBulletDynamicsCommon.h>
#include<vector>
#include<map>
#include<Ogre.h>

#ifndef _PHYSICS_H
#define _PHYSICS_H

class MainApp;
class OgreDebugUtils;
class OgreDebugDrawer;

class Physics {
private:
	static Physics* instance_;
	OgreDebugDrawer* mDebugDrawer;
	bool debuged = false;

	std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> objectsCollisions;

protected:
	//elementos necesarios para la configuracion inicial
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;

	//el propio mundo fisico
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::vector<btCollisionShape *> collisionShapes;
	std::map<std::string, btRigidBody *> physicsAccessors;
public:
	Physics();
	~Physics();
	void initPhysics();
	void releasePhysics();
	void updatePhysics(float t);
	void initDebuger(Ogre::SceneManager* sMrg);
	void removeRigidBody(btRigidBody* rb) { dynamicsWorld->removeRigidBody(rb); };

	bool getDebugState() { return debuged; };
	void setDebugState(bool b) { debuged = b; };

	void addRigidBodyToWorld(btRigidBody* bt,const std::string & btname);
	void trackAndChangeNameOfRigidBody(std::string oldName_, std::string newName_);
	std::string getRigidBodyName(btRigidBody* toFind);
	btRigidBody* getRigidBodyFromName(std::string toFind);

	std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> getObjectsCollisions() { return objectsCollisions; };
	void setObjectCollisions(std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> o) { objectsCollisions = o; };

	btDynamicsWorld* getDynamicWorld() { return dynamicsWorld; };
	int getNumberOfBodies() { return collisionShapes.size(); };
	std::vector<btCollisionShape*> getCollisionShapes() { return collisionShapes; };

	static Physics* getInstance() {
		if (instance_ == nullptr)
			instance_ = new Physics();
		return instance_;
	}
};

#endif