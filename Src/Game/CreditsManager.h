#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <GUIManager.h>
#include "Messages.h"
#include "GameSceneManager.h"
class CreditsManager :
	public BehaviourComponent
{
private:
	GUIManager* GUIMgr = nullptr;
	OIS::Keyboard* keyboard;
	int velocity;
	int posY;
	int YLimit;
public:
	CreditsManager(GameObject* gameObject, int velocity, int YLimit);
	virtual ~CreditsManager();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
};

