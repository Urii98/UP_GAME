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
	SDL_Texture* bgTexture[NUM_IMAGES];
	SDL_Texture* lvlSelectorTexture;
	SDL_Texture* lvlOneTexture;
	SDL_Texture* lvlTwoTexture;
	int frame;

	//Menu

	SDL_Texture* mainMenuScreen = nullptr;
	SDL_Texture* playSelected = nullptr;
	SDL_Texture* newGameSelected = nullptr;
	SDL_Texture* settingsSelected = nullptr;
	SDL_Texture* exitSelected = nullptr;

	SDL_Texture* defaultSettingsMenu = nullptr;
	SDL_Texture* xCircle = nullptr;
	SDL_Texture* xCircleSelected = nullptr;


	//Audio Menu
	SDL_Texture* audioSelected = nullptr;
	SDL_Texture* fxBar = nullptr;
	SDL_Texture* musicBar = nullptr;
	SDL_Texture* fxCircle = nullptr;
	SDL_Texture* musicCircle = nullptr;

	//Screen Menu
	SDL_Texture* screenSelected = nullptr;
	SDL_Texture* fullScreenTrue = nullptr;
	SDL_Texture* vSyncTrue = nullptr;

	//Credits Menu
	SDL_Texture* creditsSelected = nullptr;

	SDL_Timer chrono;
	SDL_Timer chronoFade;

	SString textureTitlePath;
	SString lvlSelectorPath;
	SString lvlOnePath;
	SString lvlTwoPath;

	const char* musicTitlePath;
	const char* musicStopPath;
	float alphatoFade;
	float alphaFromFade;
	bool toFade;
	bool fromFade;
	bool levelSelected;
	bool mapSelect;
	bool menuSettings;
	int opcion;
	int opcionSettings;
	int opcionVolumen;
	int opcionScreen;

};



#endif // !__SCENE_TITLE_H__



