#include "CreditsManager.h"
#include <iostream>
CreditsManager::CreditsManager(GameObject * gameObject, int velocity, int YLimit) : BehaviourComponent(gameObject), velocity(velocity), YLimit(YLimit)
{
	keyboard = MainApp::instance()->getKeyboard();
	GUIMgr = GUIManager::instance();
	posY = 0;
}

CreditsManager::~CreditsManager()
{
}

void CreditsManager::Update(float t)
{
	
	posY -= velocity;
	GUIMgr->getImage("CreditsImage")->setPosition(0, posY);
	if (posY <= YLimit){
		posY = 0;
		GameSceneManager::instance()->CloseScene();
	}
	else if (MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		GameSceneManager::instance()->CloseScene();
	}
}
