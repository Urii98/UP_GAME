#ifndef __SCENE_LOGO_H__
#define __SCENE_LOGO_H__
#define NUM_IMAGES 150

#include "Module.h"
#include "Animation.h"
#include "SDL_Timer.h"
#include "SceneTitle.h"

#include <iostream>

struct SDL_Texture;

class SceneLogo : public Module
{
public:

	SceneLogo(bool isActive);

	// Destructor
	virtual ~SceneLogo();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	// The scene sprite sheet loaded into an SDL_Texture
	//SDL_Texture* bgTexture[NUM_IMAGES];
	SDL_Texture* textureLogo;
	int frame;

	Animation* currentAnimationSceneLogo = nullptr;
	
	Animation sceneLogoKirby;

	

	//std::shared_ptr<Animation> cASL = std::make_shared<Animation>();

	SDL_Timer chrono;
	

};



#endif // !__SCENE_LOGO_H__



