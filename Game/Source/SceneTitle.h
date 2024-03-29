#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__
#define NUM_IMAGES 150

#include "Module.h"
#include "Animation.h"
#include "SDL_Timer.h"
#include "Scene.h"
#include "EntityManager.h"
#include "GuiCheckBox.h"
#include "GuiSliderBar.h"

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

	bool OnGuiMouseClickEvent(GuiControl* control);

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
	bool toFadeButton;
	bool boolExitButton;
	bool boolCreditButton;
	bool boolContinueButton;
	bool boolSettingsButton;
	
	bool deteleUI;

	GuiButton* playButton;
	GuiButton* continueButton;
	GuiButton* creditButton;
	GuiButton* exitButton;
	GuiButton* settingsButton;

	GuiCheckBox* checkboxFullScreen;
	GuiCheckBox* checkboxVSync;

	GuiSliderBar* sliderbarMusic;
	GuiSliderBar* sliderbarFx;

	SDL_Texture* windowCreditText;
	SDL_Texture* windowSettingsText;
	const char* windowCreditPath;
	const char* windowSettingsPath;



};



#endif // !__SCENE_TITLE_H__



