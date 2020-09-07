#pragma once
#include <Ogre.h>

#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h> 
#include "windows.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>


#include <OgreFileSystemLayer.h>
#include <OgreRenderTarget.h>
#include <OISException.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISJoyStick.h>

//Clase principal del motor

using namespace Ogre;
using namespace OIS;

class ParticleManager;
class SceneMachine;
class Scene;
class GameTime;
class MainApp
{
private:
	static MainApp* instance_;

	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::RenderSystem* renderSys;
	
	Ogre::Viewport* vp;
	
	SceneMachine* sceneMC;
	GameTime* time;
	ParticleManager* particleMgr;

	OIS::InputManager* mInputMgr;
	OIS::Mouse* mMouse;
	OIS::Keyboard* mKeyboard;

	bool appRunning;

	void messagePump();

public:	
	MainApp();
	~MainApp();
	int initApp();
	int initOgre();
	void initOIS();
	void locateResources();
	void closeApp();
	bool runGame();

	void AddScene(Scene* scene);
	void QuitScene();
	void setupViewport(Camera* camera);
	void addCompositor(std::string compositor);
	void addViewport(Ogre::Camera* camera);
	void changeCamera(Ogre::Camera* camera) {
		vp->setCamera(camera);
	}
	float deltaTime();
	float timeSinceStart();
	bool isRunning() { return appRunning; };
	INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) {}

	//getters
	Ogre::RenderWindow* getRenderWindow() { return mWindow; }
	Ogre::RenderSystem* getRenderSys() { return renderSys; }
	Ogre::Viewport* getViewport() { return mWindow->getViewport(0); }
	Ogre::Root* getRoot() { return mRoot; };
	Ogre::SceneManager* getCurrentSceneMgr();
	Scene* getCurrentScene();
	ParticleManager* getParticleManager();

	OIS::Mouse* getMouse() { return mMouse; }
	OIS::Keyboard* getKeyboard() { return mKeyboard; }


	static MainApp* instance() {
		if (instance_ == nullptr)
			instance_ = new MainApp();
		return instance_;
	}
};
