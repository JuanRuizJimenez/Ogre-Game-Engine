#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include "Messages.h"
#include <GUIManager.h>
class GameGUI :
	public BehaviourComponent
{
private:
	int healthbarFullW;
	int healthbarH;
	int fullHealth;
	int fullOverload;
	int maxOverload;

	bool overloadAux;
	GUIManager* GUIMgr = nullptr;
	MyGUI::ImageBox* healthBar = nullptr;
	MyGUI::ImageBox* overloadBar = nullptr;
	MyGUI::ImageBox* overloaded = nullptr;
	MyGUI::ImageBox* gameOver = nullptr;
	MyGUI::ImageBox* missionA = nullptr;
	MyGUI::ImageBox* warning = nullptr;
	MyGUI::ImageBox* victory = nullptr;
public:
	GameGUI(GameObject* gameObject, int _fullHealth, int _fullOverload);
	virtual ~GameGUI();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};

