#include "json.hpp"
#include <string>
#include <map>
#include <vector>
#include <list>
#include "BehaviourComponent.h"
#include "EnemyBehaviour.h"
#include "GameObject.h"
#include "Scene.h"
#include "GUIManager.h"
#ifndef _SCENELOADER_H
#define _SCENELOADER_H
using json = nlohmann::json;
class SceneLoader
{
private:
	//Diccionario con el contenido de los prefabs  key=prefab type  value=prefab parameters
	//Con este diccionario no va a ser necesario abrir el archivo prefabs cada vez que queramos cargar uno
	std::map<std::string, std::string> prefabsMap;
	std::string scenesPath;
	std::map<std::string, Scene*> scenesMap;
	std::string playerShip = "SXRVengeance";;
	std::vector<int> shipStats;// 0 HP, 1 ATK , 2 VEL
	std::vector<EnemyBehaviour*> enemies;
public:
	SceneLoader(std::string scenesPath);
	bool loadPrefabsFromFile();
	bool loadSceneFromFile(std::string sceneName, Scene* scene);
	Scene* loadSceneFromMemory(std::string sceneName);
	bool sceneAlreadyLoaded(std::string sceneName);
	bool loadTestScene(Scene* scene);
	void deleteScene(std::string sceneName);
	void setPlayerShip(std::string shipName, std::vector<int> _shipStats);

	GameObject* createGameObject(json gameObject_json, Scene* scene, std::string fatherName="");
	GameObject* createGameObjectFromPrefab(json prefabs, json gameObject_json, Scene* scene);
	void createGUIObject(json gui_json, Scene* scene);
	void addComponent(json object_json, GameObject* go, Scene* scene);
	Scene* getScene(std::string sceneName);
	~SceneLoader();
};

#endif