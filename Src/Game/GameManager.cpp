#include "GameManager.h"
#include "Messages.h"
#include "GameSceneManager.h"
#include <string>

int GameManager::currentLevel = 1;

GameManager::GameManager(GameObject* go, int _maxLevel, float _nextLevelWaitTime) : BehaviourComponent(go), maxLevel(_maxLevel), nextLevelWaitTime(_nextLevelWaitTime)
{
	GameOver = false;
	nextLevel = false;
	nextLevelTimer = nextLevelWaitTime;
	if (MainApp::instance()->getCurrentScene()->getName() == "Scene1") {
		currentLevel = 1;
	}
}

void GameManager::Update(float t)
{
	if (nextLevel) {
		nextLevelTimer -= t;
		if (nextLevelTimer <= 0 || MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		
			std::string sceneName;
			sceneName = "Scene" + to_string(currentLevel);
			GameSceneManager::instance()->ChangeToNewScene(sceneName);

			SoundManager::instance()->GetEngine()->stopAllSounds();
			if (currentLevel % 2 == 0)
				SoundManager::instance()->PlaySound2D("SynthSong2.mp3", true, false);
			else
				SoundManager::instance()->PlaySound2D("SynthSong1.mp3", true, false);
		}
	}
	else if (GameOver) {
		nextLevelTimer -= t;
		if (nextLevelTimer <= 0 || MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
			GameSceneManager::instance()->ReturnToScene("MainMenu");

			SoundManager::instance()->GetEngine()->stopAllSounds();
			SoundManager::instance()->PlaySound2D("SynthSong0.mp3", true, false);
		}
	}
	else if (victory) {
		nextLevelTimer -= t;
		if (nextLevelTimer <= 0 || MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
			GameSceneManager::instance()->ReturnToScene("MainMenu");

			SoundManager::instance()->GetEngine()->stopAllSounds();
			SoundManager::instance()->PlaySound2D("SynthSong0.mp3", true, false);
		}
	}

	if (!nextLevel && !GameOver && !victory && MainApp::instance()->getKeyboard()->isKeyDown(OIS::KC_ESCAPE)) {
		MainApp::instance()->getCurrentScene()->hideGUI();
		GameSceneManager::instance()->LoadScene("PauseMenu");
	}
}

void GameManager::reciveMsg(Message * msg)
{
	//Activa el booleano que activa la cuenta atrás para cambiar de escena
	if (msg->id == "MISSION_ACCOMPLISHED") {
		GameOver = false;
		if (currentLevel == maxLevel) {
			victory = true;
			VictoryMsg msg;
			sendSceneMsg(&msg);
		}
		else {
			currentLevel++;
			nextLevel = true;
		}
	}

	else if (msg->id == "GAME_OVER")
	{//Cuando reciba el mensaje de que el juego ha terminado 
	//Activa el GUI de GAME_OVER y su booleano
		if (!nextLevel && !victory) {
			GameOverMsg* goMsg = static_cast<GameOverMsg*>(msg);
			GameOver = true;
			currentLevel = 1;
		}
	}
}


GameManager::~GameManager()
{
}
