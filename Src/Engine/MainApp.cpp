#include "MainApp.h"
#include <OgreConfigFile.h>
#include <iostream>
#include "SceneMachine.h"
#include "GameTime.h"

//Otros Singleton
#include "Physics.h"
#include "GUIManager.h"
#include "SoundManager.h"

#include <ParticleManager.h>
MainApp* MainApp::instance_ = nullptr;

MainApp::MainApp() : mRoot(0), mResourcesCfg(Ogre::BLANKSTRING), mPluginsCfg(Ogre::BLANKSTRING)
{
	sceneMC = new SceneMachine();
	particleMgr = new ParticleManager();
}

MainApp::~MainApp()
{
	mInputMgr->destroyInputObject(mMouse);
	mInputMgr->destroyInputObject(mKeyboard);
	mInputMgr->destroyInputSystem(mInputMgr);
	mMouse = 0;
	mKeyboard = 0;
	mMouse = 0;
	SoundManager::instance()->Drop();
	delete SoundManager::instance();
	delete sceneMC;
	delete particleMgr;
	delete time;
	delete Physics::getInstance();
	delete GUIManager::instance();
	vp->clear();
	mWindow->destroy();
	mRoot->shutdown();
	delete mRoot;
}

int MainApp::initApp() 
{
	
	initOgre();
	initOIS();	
	appRunning = true;
	time = new GameTime();
	Physics::getInstance()->initPhysics();
	return 1;
}
void MainApp::messagePump()
{
	MSG  msg;
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

int MainApp::initOgre()
{
	//INICIALIZACION DE OGRE
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	mRoot = new Ogre::Root(mPluginsCfg);

	if (mRoot == NULL)
	{
		return 0;
	}
	//Esconde la consola
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, 0);

	Ogre::RenderSystemList rsl = mRoot->getAvailableRenderers();
	renderSys = *rsl.begin();
	mRoot->setRenderSystem(renderSys);

	// initialise root
	locateResources();

	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog(NULL)))
		return false;

	renderSys->setConfigOption("Full Screen", "No");
	renderSys->setConfigOption("Video Mode", "1080 x 720 @ 32-bit colour");

	mWindow = mRoot->initialise(true, "Retrowave Spaceship FoxMcDonald");


	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	
}

void MainApp::initOIS()
{
	//Inicialización de OIS
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList paramList;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));

	mInputMgr = OIS::InputManager::createInputSystem(paramList);

	mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, false));
	mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, false));
}

void MainApp::locateResources()
{
	// load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);
	Ogre::String sec, type, name;
	// go through all specified resource groups
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci) {
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		// go through all resource paths
		for (i = settings.begin(); i != settings.end(); i++)
		{
			type = i->first;
			name = Ogre::FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, type, sec);
		}
	}
}

void MainApp::closeApp()
{
	appRunning = false;
}

bool MainApp::runGame()
{
	//Bucle principal del motor
	while (appRunning) {
		messagePump();
		if (appRunning) {
			mWindow->update();
			time->Update();
		}
		if (mWindow->isClosed()) return false;

		mKeyboard->capture();
		mMouse->capture();

		if (!mRoot->renderOneFrame()) return false;
		sceneMC->update();
	}
}

void MainApp::AddScene(Scene* scene)
{
	//Añade una escena a la pila
	sceneMC->addScene(scene);
}
void MainApp::QuitScene() {
	//Borra una escena de la pila
	sceneMC->delScene();	
}

void MainApp::setupViewport(Camera* camera)
{
	//Inicializa el viewport con una cámara dada
	mWindow->removeAllViewports();
	
	Ogre::Viewport *vp = mWindow->addViewport(camera); //Our Viewport linked to the camera

	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
	camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	
}
void MainApp::addCompositor(std::string compositor) {
	//Añade un compositor
	CompositorManager::getSingleton().addCompositor(mWindow->getViewport(0), compositor);
	CompositorManager::getSingleton().setCompositorEnabled(mWindow->getViewport(0), compositor, true);
}

void MainApp::addViewport(Ogre::Camera* camera)
{
	vp = mWindow->addViewport(camera);
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
}

float MainApp::deltaTime()
{
	return time->deltaTime();
}

float MainApp::timeSinceStart()
{
	return time->timeSinceStart();
}

Ogre::SceneManager * MainApp::getCurrentSceneMgr()
{
	return sceneMC->currentScene()->getSceneManager();
}

Scene * MainApp::getCurrentScene()
{
	return sceneMC->currentScene();
}

ParticleManager * MainApp::getParticleManager()
{
	return particleMgr;
}
