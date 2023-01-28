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
	SDL_Texture* mainMenuScreen;
	SDL_Texture* playSelected;
	SDL_Texture* newGameSelected;
	SDL_Texture* settingsSelected;
	SDL_Texture* exitSelected;

	SDL_Texture* defaultSettingsMenu;
	SDL_Texture* xCircle;
	SDL_Texture* xCircleSelected;


	//Audio Menu
	SDL_Texture* audioSelected;
	SDL_Texture* fxBar;
	SDL_Texture* musicBar;
	SDL_Texture* fxCircle;
	SDL_Texture* musicCircle;

	//Screen Menu
	SDL_Texture* screenSelected;
	SDL_Texture* fullScreenTrue;
	SDL_Texture* vSyncTrue;

	//Credits Menu
	SDL_Texture* creditsSelected;

	SDL_Timer chrono;
	SDL_Timer chronoFade;

	SString textureTitlePath;
	SString lvlSelectorPath;
	SString lvlOnePath;
	SString lvlTwoPath;

	//Menu
	SString mainMenuScreenPath;
	SString playSelectedPath;
	SString newGameSelectedPath;
	SString settingsSelectedPath;
	SString exitSelectedPath;

	SString defaultSettingsMenuPath;
	SString xCirclePath;
	SString xCircleSelectedPath;


	//Audio Menu
	SString audioSelectedPath;
	SString fxBarPath;
	SString musicBarPath;
	SString fxCirclePath;
	SString musicCirclePath;

	//Screen Menu
	SString screenSelectedPath;
	SString fullScreenTruePath;
	SString vSyncTruePath;

	//Credits Menu
	SString creditsSelectedPath;


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

	float fxCircle_X = 1000;
	float musicCircle_X = 1000;

};



#endif // !__SCENE_TITLE_H__



