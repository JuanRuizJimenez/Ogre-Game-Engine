#include "GameTime.h"



GameTime::GameTime()
{
	time = SDL_GetTicks();
	lastTime = SDL_GetTicks();
}


GameTime::~GameTime()
{
}

void GameTime::Update()
{
	time = SDL_GetTicks();
	deltaTime_ = (time - lastTime)/1000;
	lastTime = SDL_GetTicks();
}
