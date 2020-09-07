#include "MainMenuManager.h"
#include <GameObject.h>
#include <math.h>
#include <iostream>
#define PI 3.14159265

MainMenuManager::MainMenuManager(GameObject* gameObject, GameObject* camera)
	: BehaviourComponent(gameObject), playProps("Play"), creditsProps("Credits"), exitProps("Exit"), activeProps("Play"), camera(camera)
{
	keyboard = MainApp::instance()->getKeyboard();
	GUIMgr = GUIManager::instance();
	//Guarda los atributos del título y de los botones
	titleImage = GUIMgr->getImage("Title");
	titleWidth = titleImage->getWidth();
	titleHeight = titleImage->getHeight();
	titleX = titleImage->getPosition().left;
	titleY = titleImage->getPosition().top;
	titleAmplitude = 0;
	titleSinPeriod = 0;
	activeButton = GUIMgr->getImage("Play");
	initTimer = 0.5;
}


MainMenuManager::~MainMenuManager()
{
}

void MainMenuManager::setButtonSinPeriod(float sinP)
{
	buttonSinPeriod = sinP;
}

void MainMenuManager::setButtonAmplitude(float amplitude)
{
	buttonAmplitude = amplitude;
}

void MainMenuManager::setTitleSinPeriod(float sinP)
{
	titleSinPeriod = sinP;
}

void MainMenuManager::setTitleAmplitude(float amplitude)
{
	titleAmplitude = amplitude;
}

void MainMenuManager::setCameraVelocity(float cameraVelocity)
{
	cameraVel = cameraVelocity;
}

void MainMenuManager::Update(float t)
{
	titleAnimation();
	buttonAnimation();
	cameraRotation();
	if (currentTime > initTimer) {
		//Timer que regula la velocidad de input
		if (lastKey == OIS::KC_W || lastKey == OIS::KC_S) {
			lastTimePressed += t;
			if (lastTimePressed > inputBufferTimer) {
				lastKey = OIS::KC_U;
				lastTimePressed = 0;
			}
		}
		if (keyboard->isKeyDown(OIS::KC_W) && lastKey != OIS::KC_W) {
			state--;
			handleStates();
			lastKey = OIS::KC_W;
		}
		else if (keyboard->isKeyDown(OIS::KC_S) && lastKey != OIS::KC_S) {
			state++;
			handleStates();
			lastKey = OIS::KC_S;
		}
		if (keyboard->isKeyDown(OIS::KC_SPACE) || keyboard->isKeyDown(OIS::KC_INSERT) || keyboard->isKeyDown(OIS::KC_RETURN)) {
			currentTime = 0;
			//Según el estado carga una escena u otra
			if (state == 0) {
				MainApp::instance()->getCurrentScene()->hideGUI();
				GameSceneManager::instance()->LoadScene("ShipSelection");

				SoundManager::instance()->GetEngine()->stopAllSounds();
				SoundManager::instance()->PlaySound2D("SynthSong3.mp3", true, false);
			}
			else if (state == 1) {
				MainApp::instance()->getCurrentScene()->hideGUI();
				GameSceneManager::instance()->LoadScene("Credits");
			}
			else if (state == 2) {
				MainApp::instance()->closeApp();
			}
		}
	}
	else {
		currentTime += MainApp::instance()->deltaTime();
	}
}
void MainMenuManager::handleStates()
{
	if (state < 0) state = 2;
	else if (state > 2) state = 0;
	//Según el estado del botón cambia su textura
	if (state == 0) //PLAY
	{
		activeButton = GUIMgr->getImage("Play");
		activeProps = playProps;
		GUIMgr->getImage("Play")->setImageTexture("PlaySelected.png");
		GUIMgr->getImage("Credits")->setImageTexture("CreditsUnSelected.png");
		GUIMgr->getImage("Exit")->setImageTexture("ExitUnSelected.png");
	}
	else if(state == 1) //CREDITS
	{
		GUIMgr->getImage("Play")->setImageTexture("PlayUnSelected.png");
		GUIMgr->getImage("Credits")->setImageTexture("CreditsSelected.png");
		activeButton = GUIMgr->getImage("Credits");
		activeProps = creditsProps;
		GUIMgr->getImage("Exit")->setImageTexture("ExitUnSelected.png");
	}
	else if (state == 2) //EXIT
	{
		GUIMgr->getImage("Play")->setImageTexture("PlayUnSelected.png");
		GUIMgr->getImage("Credits")->setImageTexture("CreditsUnSelected.png");
		GUIMgr->getImage("Exit")->setImageTexture("ExitSelected.png");
		activeButton = GUIMgr->getImage("Exit");
		activeProps = exitProps;
	}
}

void MainMenuManager::buttonAnimation()
{
	//El botón seleccionado se hace grande siguiendo una funcion senoidal
	int w = activeProps.w + buttonAmplitude * sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	int h = activeProps.h + buttonAmplitude * sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	//La x y la y se tienen que mover de acuerdo al tamaño para que el título crezca desde el centro
	int x = activeProps.x + buttonAmplitude / 2 * -sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	int y = activeProps.y + buttonAmplitude / 2 * -sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	activeButton->setSize(w, h);
	activeButton->setPosition(x, y);
}

void MainMenuManager::cameraRotation()
{
	camera->yaw(cameraVel);
}

void MainMenuManager::titleAnimation()
{
	//El título se hace grande siguiendo una funcion senoidal
	int w = titleWidth + titleAmplitude * sin(MainApp::instance()->timeSinceStart()*titleSinPeriod);
	int h = titleHeight + titleAmplitude * sin(MainApp::instance()->timeSinceStart()*titleSinPeriod);
	//La x y la y se tienen que mover de acuerdo al tamaño para que el título crezca desde el centro
	int x = titleX + titleAmplitude/2 * -sin(MainApp::instance()->timeSinceStart()*titleSinPeriod);
	int y = titleY + titleAmplitude/2 * -sin(MainApp::instance()->timeSinceStart()*titleSinPeriod);
	titleImage->setSize(w, h);
	titleImage->setPosition(x, y);
}


void MainMenuManager::reciveMsg(Message * msg)
{
	if (msg->id == "INIT_SCENE")
	{//Activa el GUI cuando inicia la escene
		titleImage->setVisible(true);
		GUIMgr->getImage("Play")->setVisible(true);
		GUIMgr->getImage("Credits")->setVisible(true);
		GUIMgr->getImage("Exit")->setVisible(true);
		GUIMgr->getImage("TitleCircle")->setVisible(true);
	}
}
