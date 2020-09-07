#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <irrKlang.h>
#include <string>
#include <vector>
#include <list>
#include <iostream>

using namespace irrklang;

class SoundManager {
private:
	// Basic variables of our manager, the instance ptr, the irrKlang engine variable & the root path 
	static SoundManager* instance_;
	ISoundEngine* engine_;
	std::string path_ = "./Assets/Audio/";

	// List of the pointers we used, we need it to clean the memory leaks
	std::list<ISound*> mem_sounds_;

public:	
	// Constructor | Destructor
	SoundManager();
	~SoundManager();

	// Initialize & destruct the internal engine
	int Init();
	int Drop();

	// Plays a sound and returns the internal variable, we can access it to change the volume, pan & stuff
	ISound* PlaySound2D(std::string file, bool loop = false, bool startPaused = false);
	ISound* PlaySound3D(std::string file, vec3df pos, float minDist = 0.5f, bool loop = false, bool startPaused = false);
	ISoundEngine* GetEngine() { return engine_; }

	// We have to determine where is the listener for 3D sounds
	void SetListenerPos(vec3df pos, vec3df dir);

	// Singleton logic, if not created, create the instance, else return our existing instance
	static SoundManager* instance() {
		if (instance_ == nullptr)
			instance_ = new SoundManager();
		return instance_;
	}
};

#endif