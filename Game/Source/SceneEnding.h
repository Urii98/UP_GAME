#ifndef __SCENE_ENDING_H__
#define __SCENE_ENDING_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneEnding : public Module
{
public:

	SceneEnding();

	// Destructor
	virtual ~SceneEnding();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();




public:
	SDL_Texture* bgTexture = nullptr;

	int timer;
	bool scape;

};



#endif // !__SCENE_ENDING_H__



