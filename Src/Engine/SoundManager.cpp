#include "SoundManager.h"

// Singleton, we will access this variable with de instance function
SoundManager* SoundManager::instance_ = nullptr;

// Principal class constructor
SoundManager::SoundManager()
{
}

// Principal class deconstructor
SoundManager::~SoundManager()
{
}

int SoundManager::Init()
{
	// start the sound engine with default parameters
	engine_ = createIrrKlangDevice();

	// error starting up the engine
	if (!engine_)
		return -1;
}

int SoundManager::Drop()
{
	// error if there is no engine
	if (!engine_)
		return -1;

	// clean resources
	for (std::list<ISound*>::const_iterator iterator = mem_sounds_.begin(), end = mem_sounds_.end(); iterator != end; ++iterator) {
		(*iterator)->drop();
	}
	mem_sounds_.clear();

	// delete engine
	engine_->drop(); 

	return 0;
}

ISound* SoundManager::PlaySound2D(std::string file, bool loop, bool startPaused)
{
	// get the final path
	std::string finalPath = (path_ + file);

	// play some sound stream
	ISound* is_aux = engine_->play2D(finalPath.c_str(), loop, startPaused, true);

	// add it to the list so we can clean it later
	mem_sounds_.push_back(is_aux);

	return is_aux;
}

ISound* SoundManager::PlaySound3D(std::string file, vec3df pos, float minDist, bool loop, bool startPaused)
{
	// get the final path
	std::string finalPath = (path_ + file);

	// play some sound stream on a position
	ISound* is_aux = engine_->play3D(finalPath.c_str(), pos, loop, startPaused, true);

	// set the min distance where we can hear the sound
	is_aux->setMinDistance(minDist);

	// add it to the list so we can clean it later
	mem_sounds_.push_back(is_aux);

	return is_aux;
}

void SoundManager::SetListenerPos(vec3df pos, vec3df dir)
{
	engine_->setListenerPosition(pos, dir);
}