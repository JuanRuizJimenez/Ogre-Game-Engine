#include "Scene.h"
#include "ParticleManager.h"
#include "GUIManager.h"
Scene::Scene()
{
	mSceneMgr = MainApp::instance()->getRoot()->createSceneManager();
	mWindow = MainApp::instance()->getRenderWindow();
}

Scene::~Scene()
{
	clearScene();
}

void Scene::updateScene()
{
	Physics::getInstance()->updatePhysics(MainApp::instance()->deltaTime());
	MainApp::instance()->getParticleManager()->update(MainApp::instance()->deltaTime());
	
	int i = 0;
	while (i < components.size() &&!endFlag) {
		if (components[i]->isGOActive())
			components[i]->Update(MainApp::instance()->deltaTime());
		i++;
	}
}

void Scene::clearScene()
{
	for (BasicComponent* bc : components) 
	{
		delete bc;
	}
	if (MainApp::instance()->isRunning()) {
		//GUIManager::instance()->resetGUI();
	}
	components.clear();
	for (GameObject* go : gameObjects) 
	{
		delete go;
	}
	gameObjects.clear();
	for (MyGUI::Widget* w  : guiObjects) {
		GUIManager::instance()->destroyWidget(w);
	}
	guiObjects.clear();
}

void Scene::addGUIObject(MyGUI::Widget* ob)
{
	guiObjects.push_back(ob);
}

void Scene::addGameObject(GameObject* GO) 
{ 
	gameObjects.push_back(GO); 
}

void Scene::addComponent(BasicComponent* bc) 
{ 
	components.push_back(bc); 
}

void Scene::addCamera(Ogre::Camera* cam) 
{ 
	mCamera = cam; 
}