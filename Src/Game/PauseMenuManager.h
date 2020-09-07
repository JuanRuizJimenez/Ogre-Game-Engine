#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <GUIManager.h>
#include "Messages.h"
#include "GameSceneManager.h"
class PauseMenuManager :
	public BehaviourComponent
{
private:
	//Struct para guardar la configuración inicial de los botones
	struct ButtonProps {
		string name; int x; int y; int w; int h;
		ButtonProps(string name) : name(name) {
			x = GUIManager::instance()->getImage(name)->getPosition().left;
			y = GUIManager::instance()->getImage(name)->getPosition().top;
			w = GUIManager::instance()->getImage(name)->getWidth();
			h = GUIManager::instance()->getImage(name)->getHeight();
		}
	};
	GUIManager* GUIMgr = nullptr;
	GameObject* camera = nullptr;
	//States
	int state = 0;// 0=Play  1=Credits  2=Exit
	void handleStates();
	//Buffer
	OIS::KeyCode lastKey;
	double inputBufferTimer = 0.2;
	double lastTimePressed = 0;
	//Animations
	MyGUI::ImageBox* activeButton = nullptr;
	ButtonProps resumeProps;
	ButtonProps mainMenuProps;
	ButtonProps activeProps;
	int buttonAmplitude = 0;
	float buttonSinPeriod = 0;
	float cameraVel = 0;

	//Timer para que haya un delay al cargar la pantalla
	float initTimer = 0.0;
	float currentTime = 0.0;

	void buttonAnimation();
	void cameraRotation();
	OIS::Keyboard* keyboard;
public:
	PauseMenuManager(GameObject* gameObject, GameObject* camera, float _cameraVel);
	virtual~PauseMenuManager();
	virtual void Update( float t);
	virtual void reciveMsg(Message* msg) {};
	void setButtonSinPeriod(float sinP);
	void setButtonAmplitude(float amplitude);
};

