#include "ParticleManager.h"
#include <iostream>
ParticleManager::ParticleManager()
{
	particleNum = 0;
}


ParticleManager::~ParticleManager()
{
	for (std::map<std::string, std::vector<ParticleInfo>>::iterator iter = particlesMap.begin(); iter != particlesMap.end(); ++iter) 
	{
		for (ParticleInfo pI : iter->second) 
		{
			pI.release();
		}
	}
	particlesMap.clear();
}
void ParticleManager::update(float t)
{
	//Recorre el diccionario de particulas
	for (std::map<std::string, std::vector<ParticleInfo>>::iterator iter = particlesMap.begin(); iter != particlesMap.end(); ++iter)
	{
		std::string particleName = iter->first;
		std::vector<ParticleInfo> particles = iter->second;
		for (int i = 0; i < particles.size(); i++) {
			//Si la partícula está activa hay que actualizar el tiempo que lleva viva
			if (particles[i].parSys->isVisible()) {
				particles[i].currentDuration += t;

				//Si el tiempo que lleva viva es mayor a su duración entonces se desactiva la partícula
				if (particles[i].currentDuration > particles[i].maxDuration) {
					particles[i].parSys->setVisible(false);
					particles[i].parSys->setEmitting(false);
					particles[i].currentDuration = 0;
				}
			}
		}
		particlesMap[particleName] = particles;
	}
}

void ParticleManager::createParticle(Ogre::Vector3 position, string particleName, float duration, string materialName)
{
	Scene* scene = MainApp::instance()->getCurrentScene();
	std::vector<ParticleInfo> particles = particlesMap[particleName];
	
	//Crea una partícula nueva y la mete en el diccionario
	particleNum++;
	GameObject* particleOb = new GameObject();
	string realName = particleName + to_string(particleNum);

	ParticleSystem* parSys = scene->getSceneManager()->createParticleSystem(realName, particleName);
	if (materialName != "None") {
		parSys->setMaterialName(materialName);
	}
	particleOb->createEmptyEntity(realName, scene);
	particleOb->setPosition(position);
	particleOb->attachMovableObject(parSys);
	
	//Usamos un struct ParticleInfo para reunir toda la info de las partículas y acceder a ella más fácilmente
	ParticleInfo pInfo;
	pInfo.gameObject = particleOb;
	pInfo.parSys = parSys;
	pInfo.maxDuration = duration;
	pInfo.currentDuration = 0;
		
	particles.push_back(pInfo);		
	
	particlesMap[particleName] = particles;
}
