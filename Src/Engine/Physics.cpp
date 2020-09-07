#include "Physics.h"
#include "MainApp.h"
#include "OgreDebugUtils.h"

#include"GameObject.h"

Physics* Physics::instance_ = nullptr;

void CollisionCallBack(btDynamicsWorld* world, btScalar t) {
	//diccionario de objetos con colision en un determinado momento
	//manifolspoint es un punto de contacto entre dos objetos en colision
	std::map<const btCollisionObject*, std::vector<btManifoldPoint*>> objectsCollisions = Physics::getInstance()->getObjectsCollisions();
	//limpiamos las colisiones que se hubieran detectado anteriormente
	objectsCollisions.clear();
	//conseguimos el numero de puntos de contacto
	for (int i = 0; i < world->getDispatcher()->getNumManifolds(); i++) {
		//analizamos la informacion de cada punto
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		auto* obj1 = contactManifold->getBody0();
		auto* obj2 = contactManifold->getBody1();

		auto& collisionsA = objectsCollisions[obj1];
		auto& collisionsB = objectsCollisions[obj2];

		int j = 0;
		while(j < contactManifold->getNumContacts()){
			btManifoldPoint & pt = contactManifold->getContactPoint(j);
			collisionsA.push_back(&pt);
			collisionsB.push_back(&pt);
			j++;
		}
		if (j > 0) {
			if (static_cast<GameObject*>(obj1->getUserPointer())->isActive())
				static_cast<GameObject*>(obj1->getUserPointer())->getRigidBody()->onCollision(static_cast<GameObject*>(obj1->getUserPointer()), static_cast<GameObject*>(obj2->getUserPointer()), collisionsA);
			
			if (static_cast<GameObject*>(obj2->getUserPointer())->isActive())
				static_cast<GameObject*>(obj2->getUserPointer())->getRigidBody()->onCollision(static_cast<GameObject*>(obj2->getUserPointer()), static_cast<GameObject*>(obj1->getUserPointer()), collisionsB);

		}
	}
	Physics::getInstance()->setObjectCollisions(objectsCollisions);
}

Physics::Physics() {
}

Physics::~Physics() {
	releasePhysics();
}
void Physics::releasePhysics() 
{
	//limpiamos punteros
	delete dynamicsWorld;
	delete collisionConfiguration;
	delete dispatcher;
	delete overlappingPairCache;;
	delete solver;
	delete mDebugDrawer;
	//vaciamos vector de collisionShapes
	collisionShapes.clear();
	//limpiamos el map
	physicsAccessors.clear();
}


void Physics::initPhysics() {
	//inicializamos los elementos necesarios
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	//creamos el mundo fisico
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setInternalTickCallback(CollisionCallBack);
	//para poder hacer debug
	mDebugDrawer = new OgreDebugDrawer();
}

void Physics::updatePhysics(float t) {
	if (dynamicsWorld != NULL) {
		//actualizamos el tiempo
		dynamicsWorld->stepSimulation(t);
	}
	if (debuged) {
		mDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		dynamicsWorld->setDebugDrawer(mDebugDrawer);
		dynamicsWorld->debugDrawWorld();
	}
}

void Physics::addRigidBodyToWorld(btRigidBody* bt,const std::string & btname) {
	//conseguimos la forma
	btCollisionShape* auxShape = bt->getCollisionShape();
	//y la añadimos al mundo
	collisionShapes.push_back(auxShape);
	dynamicsWorld->addRigidBody(bt);
	physicsAccessors.insert(std::pair<std::string, btRigidBody*>(btname, bt));
}

void Physics::trackAndChangeNameOfRigidBody(std::string oName_, std::string nName_) {
	//buscamos el valor con la antigua clave
	const std::map<std::string,btRigidBody*>::iterator it = physicsAccessors.find(oName_);
	//si la encontramos
	if (it != physicsAccessors.end()) {
		//sacamos el valor
		btRigidBody* aux = it->second;
		//borramos la entrada del diccionario
		physicsAccessors.erase(it);
		//y metemos una entrada con la nueva pareja clave-valor
		physicsAccessors.insert(std::pair<std::string, btRigidBody*>(nName_, aux));
	}
}

std::string Physics::getRigidBodyName(btRigidBody* rb) {
	bool flag = false;
	//buscamos en todo el diccionario
	std::map<std::string, btRigidBody*>::iterator it = physicsAccessors.begin();	
	while (it != physicsAccessors.end() && !flag) {
		//si coincide
		if (it->second == rb)
			flag = true;
		else it++;
	}
	if (flag)
		return it->first;
	//si entra significa que el elemento no esta en el diccionario
	else return NULL;
}

btRigidBody* Physics::getRigidBodyFromName(std::string name) {
	return physicsAccessors.at(name);
}

void Physics::initDebuger(Ogre::SceneManager* sMrg) {
	mDebugDrawer->initDebugDrawer(sMrg);
}