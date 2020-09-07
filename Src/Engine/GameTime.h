#pragma once
#include <SDL.h>
#ifndef _GAMETIME_H
#define _GAMETIME_H
class GameTime
{
private:
	//Controls Ogre aplication tiem
	float time;
	//Time when the last frame ended
	float lastTime;
	//Time passed in last frame in seconds
	float deltaTime_;
public:
	GameTime();
	~GameTime();
	void Update();
	//Returns time passed in last frame in seconds
	float deltaTime() { return deltaTime_; }
	float timeSinceStart() { return time; }
	//Inicializar esta variable en el MainApp y actualizarlo lo primero
	//Pasar el tiempo del MainApp a las escenas
};
#endif

