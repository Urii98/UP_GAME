#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__

#include "Module.h"
#include "Animation.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class SceneLogo : public Module
{
public:

	SceneLogo();

	// Destructor
	virtual ~SceneLogo();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Timer chrono;
	bool onOffSwitch;

};



#endif // !__SCENE_LOGO_H__



