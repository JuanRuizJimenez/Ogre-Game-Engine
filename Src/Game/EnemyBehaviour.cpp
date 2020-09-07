#include "EnemyBehaviour.h"
#include <GameObject.h>
#include "Messages.h"
#include <SoundManager.h>

EnemyBehaviour::EnemyBehaviour(GameObject* gameObject, int l) : BehaviourComponent(gameObject), life(l)
{
}

EnemyBehaviour::~EnemyBehaviour()
{
}

void EnemyBehaviour::Update(float t) 
{
	if (life <= 0) {
		EnemyDeadMsg msg;
		sendSceneMsg(&msg);
		this->gameObject->setActive(false);
		ISound* aux = SoundManager::instance()->PlaySound2D("Explotion.wav");
		aux->setVolume(0.9);
		
	}
}

void EnemyBehaviour::hit(int value)
{
	life -= value; 
}
