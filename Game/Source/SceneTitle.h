#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneTitle : public Module
{
public:

	SceneTitle(bool isActive);

	// Destructor
	virtual ~SceneTitle();

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



#endif // !__SCENE_TITLE_H__



