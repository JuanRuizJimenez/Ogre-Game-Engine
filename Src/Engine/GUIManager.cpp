#include "GUIManager.h"
#include "MainApp.h"

GUIManager* GUIManager::instance_ = nullptr;

void GUIManager::initScene(Scene* escena)
{	
	mPlatform->getRenderManagerPtr()->setSceneManager(escena->getSceneManager());
	
}

void GUIManager::createTextbox(string text, int x, int y, int w, int h, string skinName, string name, int fontHeight)
{
	MyGUI::TextBox* t = mGUI->createWidget<MyGUI::TextBox>(skinName, MyGUI::IntCoord(x, y, w, h), MyGUI::Align::Default, "Main", name);
	t->setCaption(text);
	t->setFontHeight(fontHeight);
}

MyGUI::ImageBox* GUIManager::createImage(string fileName, int x, int y, int w, int h, string skinName, string name, string anchor)
{
	MyGUI::ImageBox* image = mGUI->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(x,y,w,h), MyGUI::Align::Default, "Main", name);
	image->setImageTexture(fileName);
	//Si queremos tomar el punto de referencia desde abajo de la pantalla
	if (anchor == "DOWN") {
		int wH = MainApp::instance()->getRenderWindow()->getHeight();
		image->setPosition(x, wH-y);
	}
	return image;
}

MyGUI::TextBox * GUIManager::getTextBox(string name)
{
	return mGUI->findWidget<MyGUI::TextBox>(name);
}
MyGUI::ImageBox * GUIManager::getImage(string name)
{
	return mGUI->findWidget<MyGUI::ImageBox>(name);
}

GUIManager::GUIManager()
{
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(MainApp::instance()->getRenderWindow(), MainApp::instance()->getCurrentSceneMgr()); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();
	MyGUI::PointerManager::getInstance().setVisible(false);
	
}

void GUIManager::destroyWidget(string name) {
	MyGUI::ImageBox* w = mGUI->findWidget<MyGUI::ImageBox>(name);
	mGUI->destroyWidget(w);
}
void GUIManager::destroyWidget(MyGUI::Widget* name) {
	mGUI->destroyWidget(name);
}

void GUIManager::resetGUI() 
{
	mGUI->shutdown();
	delete mGUI;
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(MainApp::instance()->getRenderWindow(), MainApp::instance()->getCurrentSceneMgr()); // mWindow is Ogre::RenderWindow*, mSceneManager is Ogre::SceneManager*
	mGUI = new MyGUI::Gui();
	mGUI->initialise();
	MyGUI::PointerManager::getInstance().setVisible(false);
}

GUIManager::~GUIManager()
{
	mGUI->shutdown();
	delete mGUI;
	mGUI = 0;
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = 0;
}
