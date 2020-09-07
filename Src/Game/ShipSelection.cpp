#include "ShipSelection.h"
#define PI 3.14159265

#include <iostream>

ShipSelection::ShipSelection(GameObject* gameObject, float shipDistance, GameObject* pivot)
	: BehaviourComponent(gameObject), distance(shipDistance), shipsPivot(pivot)
{
	keyboard = MainApp::instance()->getKeyboard();
	GUIMgr = GUIManager::instance();
	initTimer = 0.5;
}

void ShipSelection::setInitialShipsPosition()
{
	//Coloca automaticamente las naves
	for (int i = 0; i < ships.size(); i++) {
		float x = -distance * i;
		float y = ships[i]->getPosition().y;
		float z = distance;
		ships[i]->setPosition(Vec3(x, y, z));
	}
	distanceBetweenShips = -distance;
	speed = 3;
}

void ShipSelection::shipsAnimation()
{
	//Las naves rotan lentamente
	for (int i = 0; i < ships.size(); i++) {
		ships[i]->yaw(0.03);
	}
}

void ShipSelection::selectShip()
{
	GameSceneManager::instance()->setPlayerShip(shipNames[state], shipStats[state]);
	GameSceneManager::instance()->LoadScene("Scene1");

	SoundManager::instance()->GetEngine()->stopAllSounds();
	SoundManager::instance()->PlaySound2D("SynthSong1.mp3", true, false);
}

void ShipSelection::addShipName(string name)
{
	shipNames.push_back(name);
}


void ShipSelection::addShipModel(GameObject * go)
{
	ships.push_back(go);
	shipsNum++;
}

void ShipSelection::addShipStats(std::vector<std::vector<int>> _shipStats)
{
	shipStats = _shipStats;
}

void ShipSelection::updateGUI()
{
	if (state >= shipsNum) {
		state = shipsNum - 1;
	}
	else if (state < 0) {
		state = 0;
	}
	if (state == shipsNum - 1) {
		GUIMgr->getImage("RightArrow")->setImageTexture("UnSelectedArrowR.png");
	}
	else {
		GUIMgr->getImage("RightArrow")->setImageTexture("SelectedArrowR.png");
	}
	if (state == 0) {
		GUIMgr->getImage("LeftArrow")->setImageTexture("UnSelectedArrowL.png");
	}
	else {
		GUIMgr->getImage("LeftArrow")->setImageTexture("SelectedArrowL.png");
	}
	GUIMgr->getImage("ShipTitle")->setImageTexture(shipNames[state]+ ".png");
	GUIMgr->getImage("ShipStats")->setImageTexture(shipNames[state] + "Stats.png");
}



void ShipSelection::moveShips()
{
	
	//Se desplaza con una velocidad hacia 
	if (direction != 0 && currentPos < distanceBetweenShips) {
		Ogre::Vector3 curr = shipsPivot->getPosition();
		curr.x += direction * speed;
		shipsPivot->setPosition(curr);

		currentPos += speed;
	}
	else {
		direction = 0;
		currentPos = 0;
	}
	
}

ShipSelection::~ShipSelection()
{
}

void ShipSelection::Update(float t)
{
	if (currentTime > initTimer) {
		//Timer que regula la velocidad de input
		if (lastKey == OIS::KC_A || lastKey == OIS::KC_D) {
			lastTimePressed += t;
			if (lastTimePressed > inputBufferTimer) {
				lastKey = OIS::KC_U;
				lastTimePressed = 0;
			}
		}
		//Cuando pulsas A baja el estado y rota hacia la izquierda
		if (keyboard->isKeyDown(OIS::KC_A) && lastKey != OIS::KC_A && state>0 && direction==0) {
			state--;
			direction = 1;
			lastKey = OIS::KC_A;
			updateGUI();
		}
		else if (keyboard->isKeyDown(OIS::KC_D) && lastKey != OIS::KC_D && state<shipsNum-1 && direction==0 ) {
			//Con D aumenta el estado y rota a la derecha
			state++;
			direction = -1;
			lastKey = OIS::KC_D;
			updateGUI();
		}
		if (keyboard->isKeyDown(OIS::KC_SPACE) || keyboard->isKeyDown(OIS::KC_INSERT) || keyboard->isKeyDown(OIS::KC_RETURN)) {
			MainApp::instance()->getCurrentScene()->hideGUI();
			selectShip();
		}
		moveShips();
	}
	else {
		currentTime += MainApp::instance()->deltaTime();
	}
	shipsAnimation();
}

void ShipSelection::reciveMsg(Message * msg)
{
	if (msg->id == "INIT_SCENE")
	{//Activa el GUI cuando inicia la escene
		GUIMgr->getImage("RightArrow")->setVisible(true);
		GUIMgr->getImage("LeftArrow")->setVisible(true);
		GUIMgr->getImage("ShipTitle")->setVisible(true);
		GUIMgr->getImage("ShipStats")->setVisible(true);
		GUIMgr->getImage("SelectYourShip")->setVisible(true);
	}
}
