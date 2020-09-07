#include "GameGUI.h"



GameGUI::GameGUI(GameObject* gameObject, int _fullHealth, int _fullOverload) : BehaviourComponent(gameObject), fullHealth(_fullHealth), fullOverload(_fullOverload)
{
	//Controla todos los elementos del GUI durante el juego menos las mirillas
	GUIMgr = GUIManager::instance();
	healthBar = GUIMgr->getImage("HealthBar");
	overloadBar = GUIMgr->getImage("OverloadBar");
	overloaded = GUIMgr->getImage("Overloaded");
	overloaded->setVisible(false);
	gameOver = GUIMgr->getImage("GameOver");
	missionA = GUIMgr->getImage("MissionAccomplished");
	warning = GUIMgr->getImage("Warning");
	victory = GUIMgr->getImage("Victory");
	healthbarFullW = healthBar->getSize().width;
	healthbarH = healthBar->getSize().height;
	maxOverload = overloadBar->getSize().width;
	overloadBar->setSize(0, overloadBar->getSize().height);
}


GameGUI::~GameGUI()
{
}

void GameGUI::Update(float t)
{
}

void GameGUI::reciveMsg(Message * msg)
{
	if (msg->id == "UPDATE_HEALTHBAR") {
		UpdateHealthBarMsg* uMsg = static_cast<UpdateHealthBarMsg*>(msg);
		int currentH = uMsg->currentHealth;
		int w = healthbarFullW*currentH / fullHealth;
		healthBar->setSize(w, healthbarH);
	}
	else if (msg->id == "UPDATE_OVERLOADBAR") {
		UpdateOverloadBarMsg* oMsg = static_cast<UpdateOverloadBarMsg*>(msg);
		int w = maxOverload * oMsg->currentOverload / fullOverload;
		overloadBar->setSize(w, overloadBar->getSize().height);
	}
	else if (msg->id == "OVERLOADED") {
		OverloadedMsg* oMsg = static_cast<OverloadedMsg*>(msg);
		overloadAux = oMsg->isOverloaded;
		if (oMsg->isOverloaded)
			overloaded->setVisible(true);
		else overloaded->setVisible(false);
	}
	else if (msg->id == "MISSION_ACCOMPLISHED") {
		if (!victory->isVisible()) {
			missionA->setVisible(true);
		}
	}
	else if (msg->id == "NEXT_LEVEL") {
		GUIMgr->destroyWidget("NextLevel");
		GUIMgr->destroyWidget("GameOver");
		GUIMgr->destroyWidget("HealthBar");
	}
	else if (msg->id == "GAME_OVER")
	{//Cuando reciba el mensaje de que el juego ha terminado 
	//Activa el GUI de GAME_OVER y su booleano
		//Si la misión no está cumplida entonces pierdes
		if (!missionA->isVisible() && !victory->isVisible()) {
			gameOver->setVisible(true);
		}
	}
	else if (msg->id == "ENTER_WARNING_ZONE")
	{
		warning->setVisible(true);
	}
	else if (msg->id == "EXIT_WARNING_ZONE")
	{
		warning->setVisible(false);
	}

	else if (msg->id == "INIT_SCENE")
	{//Activa el GUI cuando inicia la escene
		healthBar->setVisible(true);
		gameOver->setVisible(false);
		missionA->setVisible(false);
		warning->setVisible(false);
		victory->setVisible(false);
		GUIMgr->getImage("HealthBarFront")->setVisible(true);
		GUIMgr->getImage("MaxOverloadFront")->setVisible(true);
		overloadBar->setVisible(true);
		overloaded->setVisible(overloadAux);
	}
	else if (msg->id == "VICTORY") {
		missionA->setVisible(false);
		victory->setVisible(true);
	}
}
