#include "GameObject.h"
#include <OgreSceneManager.h>
#include "Scene.h"
#include "Euler.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::calculateDirection()
{
	//A través de la orientación de ogre calcula la dirección
	Ogre::Quaternion q = ogreNode->getOrientation();
	Ogre::Vector3 dir= Ogre::Vector3::ZERO;
	dir.x = 2 * (q.x * q.z - q.w* q.y);
	dir.y = -2 * (q.y * q.z - q.w * q.x);
	dir.z = -1*(1- 2 * (q.x * q.x - q.y * q.y));
	transform.direction = dir;
}

Vec3 GameObject::getBoundingBox()
{
	Ogre::AxisAlignedBox auxAABB = ogreEntity->getWorldBoundingBox(true);
	Vec3 bb = auxAABB.getSize();
	return bb;
}

void GameObject::reciveMsg(Message * msg)
{
	//Llama al método receiveMsg de todos sus componentes
	if (rigidBody) rigidBody->reciveMsg(msg);
	for (auto &bComponent: behaviourComponents)
	{
		bComponent->reciveMsg(msg);
	}
}

void GameObject::asingFather(GameObject * father_)
{
	father = father_;
	ogreNode->getParent()->removeChild(ogreNode);
	father->reciveChild(ogreNode);
}
void GameObject::createEntity(std::string mesh, std::string name_, Scene* scene)
{
	//Crea una entidad en la escena 
	name = name_;
	Ogre::SceneManager* scnMgr = scene->getSceneManager();
	ogreEntity = scnMgr->createEntity(mesh);

	ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
	ogreNode->attachObject(ogreEntity);
}

void GameObject::createEmptyEntity(std::string name_, Scene * scene)
{
	//Crea una entidad vacía sin mesh
	name = name_;
	Ogre::SceneManager* scnMgr = scene->getSceneManager();

	ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
}

void GameObject::attachCamera(Ogre::Camera * cam)
{
	camera = cam;
	ogreNode->attachObject(cam);
}

void GameObject::attachLight(Ogre::Light * lig)
{
	light = lig;
	ogreNode->attachObject(light);

	ogreNode->setDirection(Ogre::Vector3(-1, 0, -1));
}
void GameObject::attachMovableObject(Ogre::MovableObject * ob)
{
	ogreNode->attachObject(ob);	
}
void GameObject::detachObject(std::string name)
{
	ogreNode->detachObject(name);
}
void GameObject::detachAllObjects()
{
	ogreNode->detachAllObjects();
}
void GameObject::setMaterial(std::string materialName) {
	ogreEntity->setMaterialName(materialName);
}

void GameObject::setName(std::string n)
{
	name = n;
}

string GameObject::getMeshName()
{
	return ogreEntity->getMesh()->getName();
}

Ogre::Vector3 GameObject::getDirection() 
{ 
	Euler tal;
	tal.fromQuaternion(ogreNode->getOrientation());

	return tal.forward();
}