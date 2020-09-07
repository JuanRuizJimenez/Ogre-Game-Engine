#pragma once
#include "GameObject.h"
#include <vector>
#include <string>
#include "Scene.h"
#include <map>
//Clase para crear part�culas y a�adirlas a la escena
using namespace std;
class ParticleManager
{
private:
	//Tiene dentro la informaci�n b�sica de una part�cula
	struct ParticleInfo {
		GameObject* gameObject;
		ParticleSystem* parSys;
		float maxDuration;
		float currentDuration;

		void release() {
			delete gameObject;
		}
	};
	int particleNum;
	//Diccionario con un vector de los distintos tipos de part�culas activos
	std::map<std::string, std::vector<ParticleInfo>> particlesMap;
public:
	void update(float t);
	ParticleManager();
	~ParticleManager();
	void createParticle(Ogre::Vector3  position, string particleName, float duration, string materialName = "None");
};

