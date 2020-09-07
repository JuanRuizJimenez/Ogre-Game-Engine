#include "GameSceneManager.h"
#include "GUIManager.h"
#include <Physics.h>
#include "Messages.h"

GameSceneManager* GameSceneManager::instance_ = nullptr;
GameSceneManager::GameSceneManager()
{
}


GameSceneManager::~GameSceneManager()
{
	delete loader;
}
void GameSceneManager::setPlayerShip(string shipName, std::vector<int> shipStats)
{
	loader->setPlayerShip(shipName,shipStats);
}
//Carga las escenas a través del SceneLoader que lee los json
bool GameSceneManager::LoadGame()
{
	loader = new SceneLoader("./Assets/Scenes/");
	if (!loader->loadPrefabsFromFile())
		return false;	

	return true;
}

//Carga una escena con un nombre determinado
bool GameSceneManager::LoadScene(string sceneName)
{
	//Si la escena ya ha sido cargada porque está debajo en la pila de escenas
	//entonces la carga de memoria
	if (loader->sceneAlreadyLoaded(sceneName)) {
		Scene* escena = loader->loadSceneFromMemory(sceneName);
		InitSceneMsg msg;
		escena->reciveMsg(&msg);
	}
	else {		
		//Si no está en memoria entonces la lee de json
		Scene* escena = new Scene();
		MainApp::instance()->AddScene(escena);
		if (!loader->loadSceneFromFile(sceneName, escena))
			return false;
		InitSceneMsg msg;
		escena->reciveMsg(&msg);
	}
}
//Borra la escena actual y carga una nueva
void GameSceneManager::ChangeToNewScene(string nextScene)
{
	//Esconde el GUI de la escena anterior
	MainApp::instance()->getCurrentScene()->hideGUI();
	string sceneName = MainApp::instance()->getCurrentScene()->getName();
	//Borra de memoria la escena
	loader->deleteScene(sceneName);
	//La quita de la pila de escenas
	MainApp::instance()->QuitScene();
	LoadScene(nextScene);
}
//Borra las escenas necesarias hasta llegar a la indicada y cargarla
void GameSceneManager::ReturnToScene(string sceneName)
{
	//Esconde el GUI de la escena anterior
	string currentScene = MainApp::instance()->getCurrentScene()->getName();
	MainApp::instance()->getCurrentScene()->hideGUI();
	do {
		//Borra de memoria la escena
		loader->deleteScene(currentScene);
		//La quita de la pila de escenas
		MainApp::instance()->QuitScene();
		currentScene = MainApp::instance()->getCurrentScene()->getName();
	} while (currentScene != sceneName);
	LoadScene(sceneName);
}
//Cierra la escena actual y carga la anterior en la pila
void GameSceneManager::CloseScene()
{
	//Esconde el GUI de la escena anterior
	MainApp::instance()->getCurrentScene()->hideGUI();
	string sceneName = MainApp::instance()->getCurrentScene()->getName();
	//Borra de memoria la escena
	loader->deleteScene(sceneName);
	//La quita de la pila de escenas
	MainApp::instance()->QuitScene();
	//Carga la siguiente escena en la pila
	Scene* scene = MainApp::instance()->getCurrentScene();
	string nextSceneName = scene->getName();
	LoadScene(nextSceneName);
}
