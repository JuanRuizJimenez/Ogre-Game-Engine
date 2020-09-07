#include "EnemyManager.h"
#include <GameObject.h>
#include "Messages.h"
#include<SoundManager.h>

EnemyManager::EnemyManager(GameObject* go) : BehaviourComponent(go)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Update(float t)
{
	
}

void EnemyManager::reciveMsg(Message* msg) 
{
	if (msg->id == "QUITA_VIDA") 
	{
		DownLifeMsg* dlm = static_cast<DownLifeMsg*>(msg);
		int i = 0;
		bool found = false;
		//Busca el enemigo que ha recibido el golpe y llama a su método hit
		while(i < enemies_c.size() && !found) {
			if (enemies_c[i]->getGameObject()->getName() == dlm->name && enemies_c[i]->getGameObject()->isActive())
			{
				enemies_c[i]->hit(dlm->power);
				found = true;
			}
			i++;
		}
	}
	else if (msg->id == "ENEMY_DEAD") {
		enemies--;
		if (enemies <= 0) {
			MissionAccomplishedMsg mam;
			sendSceneMsg(&mam);
			SoundManager::instance()->PlaySound2D("LevelCompleted.wav");
		}
	}
}