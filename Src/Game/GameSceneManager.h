#pragma once
#include <string>
#include <SceneLoader.h>
using namespace std;
class GameSceneManager
{
private:
	static GameSceneManager* instance_;
	SceneLoader* loader;
	bool loaded = false;
public:
	GameSceneManager();
	~GameSceneManager();
	void setPlayerShip(string shipName, std::vector<int> shipStats);
	bool LoadGame();
	bool LoadScene(string sceneName);
	void CloseScene();
	void ChangeToNewScene(string nextScene);
	void ReturnToScene(string sceneName);
	static GameSceneManager* instance() {
		if (instance_ == nullptr)
			instance_ = new GameSceneManager();
		return instance_;
	}
};

