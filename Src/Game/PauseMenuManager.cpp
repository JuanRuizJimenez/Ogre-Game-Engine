#include "PauseMenuManager.h"

//Clase muy parecida al MainMenuManager

PauseMenuManager::PauseMenuManager(GameObject* gameObject, GameObject* camera, float _cameraVel) 
	: BehaviourComponent(gameObject), camera(camera), resumeProps("Resume"), mainMenuProps("MainMenu"), cameraVel(_cameraVel), activeProps("Resume")
{
	keyboard = MainApp::instance()->getKeyboard();
	GUIMgr = GUIManager::instance();
	activeButton = GUIMgr->getImage("Resume");
	initTimer = 0.5;
}


void PauseMenuManager::Update(float t)
{
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
			if (state == 0) {
				MainApp::instance()->getCurrentScene()->hideGUI();
				GameSceneManager::instance()->CloseScene();
			}
			else{
				MainApp::instance()->getCurrentScene()->hideGUI();
				GameSceneManager::instance()->ReturnToScene("MainMenu");

				SoundManager::instance()->GetEngine()->stopAllSounds();
				SoundManager::instance()->PlaySound2D("SynthSong0.mp3", true, false);
			}
		}
	}
	else {
		currentTime += MainApp::instance()->deltaTime();
	}
	
}


void PauseMenuManager::handleStates()
{
	if (state < 0) state = 1;
	else if (state > 1) state = 0;
	if (state == 0) //RESUME
	{
		activeButton = GUIMgr->getImage("Resume");
		activeProps = resumeProps;
		GUIMgr->getImage("Resume")->setImageTexture("ResumeSelected.png");
		GUIMgr->getImage("MainMenu")->setImageTexture("MainMenuUnSelected.png");
	}
	else //MAINMENU
	{
		GUIMgr->getImage("Resume")->setImageTexture("ResumeUnSelected.png");
		activeButton = GUIMgr->getImage("MainMenu");
		activeProps = mainMenuProps;
		GUIMgr->getImage("MainMenu")->setImageTexture("MainMenuSelected.png");
	}
}

void PauseMenuManager::buttonAnimation()
{
	//El botón se hace grande siguiendo una funcion senoidal
	int w = activeProps.w + buttonAmplitude * sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	int h = activeProps.h + buttonAmplitude * sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	//La x y la y se tienen que mover de acuerdo al tamaño para que el título crezca desde el centro
	int x = activeProps.x + buttonAmplitude / 2 * -sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	int y = activeProps.y + buttonAmplitude / 2 * -sin(MainApp::instance()->timeSinceStart()*buttonSinPeriod);
	activeButton->setSize(w, h);
	activeButton->setPosition(x, y);
}

void PauseMenuManager::cameraRotation()
{
	camera->yaw(cameraVel);
}
void PauseMenuManager::setButtonSinPeriod(float sinP)
{
	buttonSinPeriod = sinP;
}

void PauseMenuManager::setButtonAmplitude(float amplitude)
{
	buttonAmplitude = amplitude;
}




PauseMenuManager::~PauseMenuManager()
{
	
}
