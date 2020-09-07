#include "SceneLoader.h"
#include <fstream>
#include <iostream>
#include "PlayerController.h"
#include "CameraMovement.h"
#include "ShipController.h"
#include "MainMenuManager.h"
#include <vector>
#include "ShipSelection.h"
#include "TurretBehaviour.h"
#include "ShotBehaviour.h"
#include "FlyerBehaviour.h"
#include "TargetController.h"
#include "EnemyBehaviour.h"
#include "EnemyManager.h"
#include "CreditsManager.h"
#include "GameGUI.h"
#include "GameManager.h"
#include "PauseMenuManager.h"

#include "EnemyShoot.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
using json = nlohmann::json;

SceneLoader::SceneLoader(std::string scenesPath) : scenesPath(scenesPath)
{
	shipStats.push_back(3);
	shipStats.push_back(3);
	shipStats.push_back(3);
}

void OnCuboCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints) {
	//std::cout << "Soy un cubo y he chocado" << std::endl;
}

void OnCubo2Collision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints) {
	//std::cout << "Juan wapo" << std::endl;
}

bool SceneLoader::loadPrefabsFromFile()
{
	std::cout << "Cargando prefabs..." << std::endl;
	//Carga el json de los prefabs
	std::string prefabsFilename = scenesPath + "Prefabs.json";
	std::ifstream prefabsFile;
	prefabsFile.open(prefabsFilename.c_str());
	if (prefabsFile.fail())
	{
		std::cerr << "Los prefabs no se pudieron cargar " <<std::endl;
		return false;
	}
	json prefabs_json;
	prefabsFile >> prefabs_json;

	//Obtiene el valor del "tipo" de prefab y lo utiliza como key para el diccionario
	//El valor se queda como un string en formato json
	for (json::iterator it = prefabs_json["Prefabs"].begin(); it != prefabs_json["Prefabs"].end(); ++it) {
		std::string key = (*it)["Type"].get<std::string>();
		std::string value = (*it).dump();
		prefabsMap.insert(std::pair<std::string, std::string>(key, value));
	}

	prefabsFile.close();
	std::cout << "Prefabs cargados con exito!" << std::endl;
	return true;
}



bool SceneLoader::loadSceneFromFile(std::string sceneName, Scene* scene)
{
	//Carga una escena de un archivo json
	scene->setName(sceneName);

	//Si el nombre es TestScene carga del método loadTestScene, que usamos al principio para debuguear antes
	//de haber implementado totalmente la carga de escenas, se podría borrar perfectamente
	if (sceneName == "TestScene") {
		loadTestScene(scene);
		return true;
	}
	std::cout << "********Cargando " << sceneName << "********" << std::endl;
	//Carga el archivo que contiene la escena
	std::string sceneFilename;
	sceneFilename = scenesPath + sceneName + ".json";
	std::ifstream sceneFile;
	sceneFile.open(sceneFilename.c_str());
	if (sceneFile.fail())
	{
		std::cerr << "No se ha podido cargar " << sceneFilename << std::endl;
		return false;
	}
	json scene_json;
	sceneFile >> scene_json;

	//Mete la escena en el diccionario
	scenesMap.insert(pair<std::string, Scene*>(sceneName, scene));

	//Lee el apartado SkyBox y lo crea
	if (scene_json.find("SkyBox") != scene_json.end()) {
		scene->getSceneManager()->setSkyBox(true, scene_json["SkyBox"], 10);
	}


	//Se cargan las opciones de la escena
	//Primero carga la cámara para evitar conflictos con la inicialización de MyGUI
	std::string cameraName = scene_json["Camera"].get<std::string>();
	Ogre::Camera* cam = scene->getSceneManager()->createCamera(cameraName);
	scene->addCamera(cam);
	
	//Set del viewport
	MainApp::instance()->setupViewport(scene->getCamera());
	MainApp::instance()->addCompositor("Interference");

	//Inicializa la escena en ogre
	GUIManager::instance()->initScene(scene);
	//Itera sobre los elementos del GUI
	for (json::iterator it = scene_json["GUI"].begin(); it != scene_json["GUI"].end(); ++it) {
		createGUIObject(*it, scene);
	}


	//Itera sobre los gameobjects en el json de la escena
	for (json::iterator it = scene_json["GameObjects"].begin(); it != scene_json["GameObjects"].end(); ++it) {

		//Guarda el valor de su posici�n y de su tipo
		std::string prefabType = (*it)["Type"].get<std::string>();
		//Si el tipo de prefab existe entonces crea el gameobject a partir del diccionario
		if (prefabsMap.find(prefabType) != prefabsMap.end()) {
			json prefab_json = json::parse(prefabsMap[(*it)["Type"]]);
			GameObject* go = createGameObjectFromPrefab(prefab_json, (*it),  scene);
			scene->addGameObject(go);		

		}
		else {
			//Si no existe entonces lo crea leyendo el json de la escena
			GameObject* go = createGameObject((*it), scene);
			scene->addGameObject(go);
		}
		
	}

	//Cierra el archivo de la escena
	sceneFile.close();
	std::cout << sceneName << " cargado con exito!" << std::endl;
	return true;
}

Scene* SceneLoader::loadSceneFromMemory(std::string sceneName)
{
	//Asigna la escena de memoria
	//Esto ocurre en los casos en los que se hace pop de la pila de escenas
	//para cargar el estado de la escena anterior 
	Scene* scene = scenesMap[sceneName];
	MainApp::instance()->setupViewport(scene->getCamera());
	GUIManager::instance()->initScene(scene);
	cout << sceneName << "cargado" << endl;
	return scene;
}

bool SceneLoader::sceneAlreadyLoaded(std::string sceneName)
{
	
	//Si el nombre de la escena existe eso quiere decir que ya está cargada
	if (scenesMap.find(sceneName) != scenesMap.end()) {
		return true;
	}
	else return false;
}

//Escenas de prueba para meterlas desde c�digo aqu�
bool SceneLoader::loadTestScene(Scene* scene)
{
	//Aquí teníamos metido todo el código de creación de gameobjects y componentes
	//Lo hemos borrado ahora que no lo utilizábamos porque ocupaba muchas líneas

	return true;
}

void SceneLoader::deleteScene(std::string sceneName)
{
	delete scenesMap[sceneName];
	scenesMap.erase(sceneName);
	enemies.clear();
}

void SceneLoader::setPlayerShip(std::string shipName, std::vector<int> _shipStats)
{
	playerShip = shipName;
	shipStats = _shipStats;
}

GameObject* SceneLoader::createGameObject(json gameObject_json, Scene* scene, std::string fatherName)
{
	//Crea un gameobject leyendo sus atributos del json
	GameObject* ob = new GameObject();
	std::string name= fatherName + gameObject_json["Name"].get<std::string>();
	std::string meshName = gameObject_json["Mesh"].get<std::string>();
	std::vector<float> position = gameObject_json["Position"].get<std::vector<float>>();

	
	if (meshName == "SelectedShip") {
		meshName = playerShip + ".mesh";
	}
	//Si no tiene mesh crea una entidad vacía
	if (meshName == "None") {
		ob->createEmptyEntity(name, scene);
	}
	else ob->createEntity(meshName, name, scene);

	Vec3 pos(position[0], position[1], position[2]);
	ob->setPosition(pos);
	//Si tiene el parámetro escala lo asigna
	if (gameObject_json.find("Scale") != gameObject_json.end()) {
		std::vector<float> scale = gameObject_json["Scale"].get<std::vector<float>>();
		Vec3 sc(scale[0], scale[1], scale[2]);
		ob->setScale(sc);
	}


	//Si tiene childs los recorre y va creando sus gameobjects y componentes
	for (json::iterator iter = gameObject_json["Childs"].begin(); iter != gameObject_json["Childs"].end(); ++iter) {
		GameObject* child = createGameObject((*iter), scene, ob->getName());
		child->asingFather(ob);
		scene->addGameObject(child);
	}

	//Recorre su lista de componentes para crearlos
	for (json::iterator itComponent = gameObject_json["Components"].begin(); itComponent != gameObject_json["Components"].end(); ++itComponent) {
		addComponent(*itComponent, ob, scene);
	}
	
	return ob;
}

GameObject * SceneLoader::createGameObjectFromPrefab(json prefabs, json gameObject_json, Scene * scene)
{
	//Crea gameobjects desde el diccionario de prefabs, 
	//recibe los dos parámetros por si queremos sobrescribir algo
	//de los prefabs
	GameObject* ob = new GameObject();
	std::string name = gameObject_json["Name"].get<std::string>();

	//Si el objeto de escena tiene parámetro mesh ignora el del prefab
	std::string meshName;
	if (gameObject_json.find("Mesh") != gameObject_json.end()) {
		meshName = gameObject_json["Mesh"].get<std::string>();
	}
	else {
		meshName = prefabs["Mesh"].get<std::string>();
	}
	std::vector<float> position;
	//Ignora la posicion del prefab si existe
	if (gameObject_json.find("Position") != gameObject_json.end()) {
		position = gameObject_json["Position"].get<std::vector<float>>();
	}
	else {
		position = prefabs["Position"].get<std::vector<float>>();
	}


	if (meshName == "SelectedShip") {
		meshName = playerShip + ".mesh";
	}
	//Si no tiene mesh crea una entidad vacía
	if (meshName == "None") {
		ob->createEmptyEntity(name, scene);
	}
	else ob->createEntity(meshName, name, scene);

	Vec3 pos(position[0], position[1], position[2]);
	ob->setPosition(pos);
	//Si tiene el parámetro escala lo asigna
	if (prefabs.find("Scale") != prefabs.end()) {
		std::vector<float> scale = prefabs["Scale"].get<std::vector<float>>();
		Vec3 sc(scale[0], scale[1], scale[2]);
		ob->setScale(sc);
	}


	//Si tiene childs los recorre y va creando sus gameobjects y componentes
	for (json::iterator iter = prefabs["Childs"].begin(); iter != prefabs["Childs"].end(); ++iter) {
		GameObject* child = createGameObject((*iter), scene, ob->getName());
		child->asingFather(ob);
		scene->addGameObject(child);
	}
	   	
	//Recorre la lista de componentes del prefab
	for (json::iterator pitComponent = prefabs["Components"].begin(); pitComponent != prefabs["Components"].end(); ++pitComponent) {
		std::string prefabComponentName = (*pitComponent)["Name"];
		bool overwrite = false;
		//Si el nombre del component del prefab coincide con el de la escena entonces se creará el componente de la escena
		json::iterator itComponent = gameObject_json["Components"].begin();
		while (itComponent != gameObject_json["Components"].end() && !overwrite) {
			std::string componentName = (*itComponent)["Name"];
			if (prefabComponentName == componentName) {
				overwrite = true;
			}
			else ++itComponent;
		}
		if (overwrite) {
			addComponent(*itComponent, ob, scene);
		}
		else {
			addComponent(*pitComponent, ob, scene);
		}
	}

	return ob;
}

void SceneLoader::createGUIObject(json gui_json, Scene* scene)
{
	//Atributos comunes de los objetos GUI
	std::string guiName = gui_json["Name"];
	std::string guiType = gui_json["Type"];
	std::vector<float> position = gui_json["Position"];
	std::vector<float> size = gui_json["Size"];
	bool visible = true;
	if (gui_json.find("Visible") != gui_json.end()) {
		visible = gui_json["Visible"];
	}
	std::string anchor = "UP";
	if (gui_json.find("Anchor") != gui_json.end()) {
		anchor = gui_json["Anchor"].get<string>();
	}
	if (guiType == "ImageBox") {
		std::string source = gui_json["Src"];
		MyGUI::ImageBox* b = GUIManager::instance()->createImage(source, position[0], position[1], size[0], size[1], guiType, guiName, anchor);
		b->setVisible(visible);
		scene->addGUIObject(b);
	}
	else if (guiType == "TextBox") {
		std::string caption = gui_json["Caption"];
		int fontHeight = gui_json["FontHeight"];
		GUIManager::instance()->createTextbox(caption, position[0], position[1], size[0], size[1], guiType, guiName, fontHeight);
	}
}

void SceneLoader::addComponent(json object_json, GameObject * go, Scene* scene)
{	
	//Cadena de else if que dependiendo del nombre del componente a meter inicializa de una forma u otra
	std::string componentName = object_json["Name"];
	if (componentName == "Camera") {
		//Adjuntamos un gameobject a la cámara creada
		std::string cameraName = object_json["CameraName"];
		Ogre::Camera* cam = scene->getSceneManager()->getCamera(cameraName);
		int dist = object_json["NearClipDistance"].get<int>();
		std::vector<int> look = object_json["LookAt"].get<std::vector<int>>();
		go->lookAt(Vec3(look[0], look[1], look[2]));
		cam->setNearClipDistance(dist);			
		go->attachCamera(cam);		
	}
	else if (componentName == "CameraMovement") {
		std::string pName = object_json["Pivot"];
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		GameObject* pivot = scene->getGameObject(pName);
		CameraMovement* cM = new CameraMovement(go, target, pivot);

		float horizontalLimit = object_json["horizontalLimit"];
		float horizontaVel = object_json["horizontaVel"];
		float horizontalBackVel = object_json["horizontalBackVel"];
		float verticalLimit = object_json["verticalLimit"];
		float verticalVel = object_json["verticalVel"];
		float aceleratedCameraOffset = object_json["aceleratedCameraOffset"];
		float deceleratedCameraOffset = object_json["deceleratedCameraOffset"];
		float cameraDefaulOffset = object_json["cameraDefaulOffset"];
		float cameraAceleratedVel = object_json["cameraAceleratedVel"];
		float cameraDeceletatedVel = object_json["cameraDeceletatedVel"];
		float maxShakeDuration = object_json["maxShakeDuration"];
		float shakeAmount = object_json["shakeAmount"];

		cM->setInitialValues(horizontalLimit, horizontaVel, horizontalBackVel, verticalLimit, verticalVel, aceleratedCameraOffset, deceleratedCameraOffset,
			cameraDefaulOffset, cameraAceleratedVel, cameraDeceletatedVel, maxShakeDuration, shakeAmount);

		scene->addComponent(cM);
	}
	else if (componentName == "Rigidbody") {
		bool kinematic = false;
		RigidBody* rb;
		int y = 0;
		int x = 0;
		if (object_json.find("Kinematic") != object_json.end()) {
			kinematic = object_json["Kinematic"];
		}
		if (object_json.find("Density") != object_json.end()) {
			int density = object_json["Density"];
			rb = new RigidBody(go, go->getName(), density, kinematic);
		}
		else {
			rb = new RigidBody(go, go->getName());
		}
		if (object_json.find("YPivot") != object_json.end()) {
			std::string yPiv = object_json["YPivot"];
			if (yPiv == "Down") {
				y = go->getBoundingBox().y / 2;
			}
			else if (yPiv == "Up") {
				y = -go->getBoundingBox().y / 2;
			}
		}
		if (object_json.find("XPivot") != object_json.end()) {
			std::string yPiv = object_json["XPivot"];
			if (yPiv == "Right") {
				x = -go->getBoundingBox().x / 2;
			}
			else if (yPiv == "Left") {
				x = go->getBoundingBox().x / 2;
			}
		}
		rb->setOffset(x, y);
		scene->addComponent(rb);
	}
	else if (componentName == "ShipController") {
		//Las naves tienen una vida base, y a partir de ahí aumenta según los stats de la anve
		int baseHealth = object_json["BaseHealth"];
		int health = object_json["Health"];
		float rollingC = object_json["RollingCooldown"];
		int WZL = object_json["WarningZoneLength"];
		int DZL = object_json["DeadZoneLength"];
		ShipController* sc = new ShipController(go, baseHealth + shipStats[0]*health, playerShip, rollingC, WZL, DZL);

		scene->addComponent(sc);
	}
	else if (componentName == "PlayerController") {
		int velocity = object_json["Velocity"];
		PlayerController* pc = new PlayerController(go, shipStats[2] * velocity);
		scene->addComponent(pc);
	}
	else if (componentName == "ShotBehaviour") {
		//Le pasamos el nombre de la nave actual y los pivotes de donde saldran las balas
		std::string lName = object_json["LeftPivot"];
		GameObject* lPivot = scene->getGameObject(lName);

		std::string rName = object_json["RightPivot"];
		GameObject* rPivot = scene->getGameObject(rName);

		int damage = object_json["Damage"],
			maxOv = object_json["MaxOverload"];
		float shotCD = object_json["ShootCooldown"],
			ovRecharge = object_json["OverloadRechargeMultiplier"];
		float bLifeTime = object_json["BLifeTime"];
		ShotBehaviour* sb = new ShotBehaviour(go, playerShip, shipStats[1]*damage, maxOv, shotCD, ovRecharge, bLifeTime);
		scene->addComponent(sb);
	}
	else if (componentName == "TargetController") {
		std::string cameraName = object_json["CameraName"];
		Ogre::Camera* mCamera = scene->getSceneManager()->getCamera(cameraName);
		std::string blueImg = object_json["BlueImage"];
		std::string redImg = object_json["RedImage"];
		std::string guiN = object_json["GUIName"];
		int w = object_json["Width"];
		int h = object_json["Height"];
		TargetController* tc = new TargetController(go, mCamera, blueImg,redImg,guiN,w, h);
		scene->addComponent(tc);
	}
	else if (componentName == "EnemyManager") {
	EnemyManager* em = new EnemyManager(go);
	for (int i = 0; i < enemies.size(); i++) {
		em->addEnemy(enemies[i]);
	}
	scene->addComponent(em);
	}
	else if (componentName == "EnemyShoot") {
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		std::string bulletMesh = object_json["BulletMesh"];
		std::string eType = object_json["EnemyType"];
		int damage = object_json["Damage"];
		int range = object_json["Range"];
		float cd = object_json["ShootCd"];
		enemyType type;
		if (eType == "GroundTurret") {
			type = enemyType::groundTurret;
		}
		else type = enemyType::Flyer;
		float bLifeTime = object_json["BLifeTime"];
		EnemyShoot* FES = new EnemyShoot(go, type, target,damage, range, bulletMesh,cd, bLifeTime);
		scene->addComponent(FES);
	}
	else if (componentName == "TurretBehaviour") {
		//Le pasamos el nombre de la nave actual
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		TurretBehaviour* tB = new TurretBehaviour(go, target);
		scene->addComponent(tB);
	}
	else if (componentName == "FlyerBehaviour") {
		//Le pasamos el nombre de la nave actual
		std::string tName = object_json["Target"];
		GameObject* target = scene->getGameObject(tName);
		int r = object_json["Radius"];
		int h = object_json["Height"];

		std::string route = object_json["Route"];

		FlyerRoute fRoute = Sinusoidal;
		if (route == "XCircular") {
			fRoute = XCircular;
		}
		else if (route == "YCircular") {
			fRoute = YCircular;
		}
		else if (route == "ZCircular") {
			fRoute = ZCircular;
		}
		else if (route == "Vertical") {
			fRoute = Vertical;
		}
		else if (route == "Horizontal") {
			fRoute = Horizontal;
		}
		else if (route == "Transversal") {
			fRoute = Transversal;
		}
		else if (route == "Sinusoidal") {
			fRoute = Sinusoidal;
		}
		FlyerBehaviour* tB = new FlyerBehaviour(go, target, fRoute, r, h);
		scene->addComponent(tB);
	}	
	else if (componentName == "EnemyBehaviour") {
		int health = object_json["Health"];
		EnemyBehaviour* enem = new EnemyBehaviour(go, health);
		enemies.push_back(enem);
		scene->addComponent(enem);
	}		
	else if (componentName == "GameGUI") {
		int baseHealth = object_json["BaseHealth"];
		int fH = object_json["Health"];
		int fO = object_json["Overload"];
		GameGUI* GG = new GameGUI(go, baseHealth +shipStats[0] * fH, fO);
		scene->addComponent(GG);
	}
	else if (componentName == "GameManager") {
		int levels = object_json["MaxLevels"];
		int time = object_json["WaitTime"];
		GameManager* gm = new GameManager(go,levels, time);
		scene->addComponent(gm);
	}
	else if (componentName == "PauseMenuManager") {
	std::string cameraName = object_json["CameraObject"];
	float cameraVel = object_json["CameraVel"];
	int buttonAmp = object_json["ButtonAmplitude"];
	float buttonSinPeriod = object_json["ButtonSinPeriod"];
	GameObject* cam = scene->getGameObject(cameraName);
	PauseMenuManager* enem = new PauseMenuManager(go, cam, cameraVel);
	enem->setButtonAmplitude(buttonAmp);
	enem->setButtonSinPeriod(buttonSinPeriod);
	scene->addComponent(enem);
	}
	else if (componentName == "MainMenuManager") {
		int titleAmp = object_json["TitleAmplitude"];
		float titleSinPeriod = object_json["TitleSinPeriod"];
		int buttonAmp = object_json["ButtonAmplitude"];
		float buttonSinPeriod = object_json["ButtonSinPeriod"];
		std::string cameraName = object_json["CameraObject"];
		float cameraVel = object_json["CameraVel"];
		GameObject* cam = scene->getGameObject(cameraName);
		MainMenuManager* MMM = new MainMenuManager(go, cam);
		MMM->setTitleAmplitude(titleAmp);
		MMM->setTitleSinPeriod(titleSinPeriod);
		MMM->setButtonAmplitude(buttonAmp);
		MMM->setButtonSinPeriod(buttonSinPeriod);
		MMM->setCameraVelocity(cameraVel);
		scene->addComponent(MMM);
	}
	else if (componentName == "CreditsManager") {
		int vel = object_json["Velocity"];
		int YLimit = object_json["YLimit"];
		CreditsManager* CM = new CreditsManager(go, vel, YLimit);
		scene->addComponent(CM);
	}
	else if (componentName == "ShipSelection") {
		int shipDist = object_json["ShipDistance"];
		std::string circlePivot = object_json["Pivot"];
		GameObject* ob = scene->getGameObject(circlePivot);
		ShipSelection* SS = new ShipSelection(go, shipDist, ob);
		std::vector<std::string> models = object_json["Models"];
		for (auto modelName:models) {
			GameObject* ob = scene->getGameObject(modelName);
			SS->addShipModel(ob);
		}
		std::vector<std::string> names = object_json["Names"];
		for (auto name : names) {
			SS->addShipName(name);
		}
		std::vector<std::vector<int>> stats = object_json["Stats"];
		SS->addShipStats(stats);
		SS->setInitialShipsPosition();
		scene->addComponent(SS);
	}
	else if (componentName == "Light") {
	Ogre::Light* luz = scene->getSceneManager()->createLight("Luz");
	if (object_json["Type"] == "Directional") {
		luz->setType(Ogre::Light::LT_DIRECTIONAL);
	}
	luz->setDiffuseColour(0.75, 0.75, 0.75);
	go->attachLight(luz);
	}
	
}

Scene* SceneLoader::getScene(std::string sceneName)
{
	Scene* escena = scenesMap[sceneName];
	return escena;
}


SceneLoader::~SceneLoader()
{
	scenesMap.clear();
}
