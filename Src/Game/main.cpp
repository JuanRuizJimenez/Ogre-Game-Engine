#include <iostream>
#include <MainApp.h>
#include <Scene.h>
#include <SceneLoader.h>
#include "PlayerController.h"
#include "GameSceneManager.h"
#include <SoundManager.h>

int main() {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	srand(time(NULL));

	//Carga los recursos e inicializa el motor
	MainApp::instance()->initApp();

	//Carga el json de prefabs
	if (!GameSceneManager::instance()->LoadGame()) 
		return false;

	//Carga la primera escena que es MainMenu
	GameSceneManager::instance()->LoadScene("MainMenu");

	SoundManager::instance()->Init();
	SoundManager::instance()->GetEngine()->stopAllSounds();
	SoundManager::instance()->PlaySound2D("SynthSong0.mp3", true, false);

	//Ejecuta el bucle principal del motor
	MainApp::instance()->runGame();


	delete GameSceneManager::instance();
	delete MainApp::instance();

	
	return 0;
}