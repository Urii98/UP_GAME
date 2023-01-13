#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__
#define NUM_IMAGES 150

#include "Module.h"
#include "Animation.h"
#include "SDL_Timer.h"
#include "Scene.h"
#include "EntityManager.h"

struct SDL_Texture;

class SceneTitle : public Module
{
public:

	SceneTitle(bool isActive);

	// Destructor
	virtual ~SceneTitle();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	SDL_Texture* bgTexture[8];
	SDL_Texture* lvlSelectorTexture;
	SDL_Texture* lvlOneTexture;
	SDL_Texture* lvlTwoTexture;

	SDL_Texture* intentoProfiling;
	int frame;
	int rowFrame;
	bool rowFrameBool;

	SDL_Timer chrono;
	SDL_Timer chronoFade;

	const char* musicTitlePath;
	const char* musicStopPath;
	float alphatoFade;
	float alphaFromFade;
	bool toFade;
	bool fromFade;
	bool levelSelected;
	bool mapSelect;

};



#endif // !__SCENE_TITLE_H__



