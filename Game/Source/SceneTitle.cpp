#include "SceneTitle.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include <iostream>

#include "Optick/include/optick.h"

#include "GuiButton.h"
#include "Window.h"
#include "GuiManager.h"
#include "SceneLogo.h"
#include "Scene.h"
#include "Render.h"
#include "ModuleFonts.h"

SceneTitle::SceneTitle(bool isActive) : Module(isActive) {
	name.Create("sceneTitle");
}

// Destructor
SceneTitle::~SceneTitle() {

}

bool SceneTitle::Awake(pugi::xml_node& config) {

	musicTitlePath = config.child("musicTitlePath").attribute("path").as_string();
	musicStopPath = config.child("musicStopPath").attribute("path").as_string();
	windowCreditPath = "Assets/Textures/Window.png";
	windowSettingsPath = "Assets/Textures/Window3.png";

	

	return true;
}

bool SceneTitle::Start() {
	OPTICK_EVENT();
	LOG("Loading background assets- SceneTitle");

	bool ret = true;

	char x[120];

	// ---------- Implementación previa a profilling
	// 
	//for (int i = 0; i <= 146; i++) //implementación de antes
	//{
	//	sprintf_s(x, "Assets/Textures/Title/ezgif-frame-%d.jpg", i + 1);
	//	bgTexture[i] = app->tex->Load(x);
	//}
	// ---------- 

	// ---------- Implementación posterior a profilling

	for (int i = 0; i < 8; i++)
	{
		sprintf_s(x, "Assets/Textures/Merge/Merge%d.png", i + 1);
		bgTexture[i] = app->tex->Load(x);
	}

	uint w, h;

	app->win->GetWindowSize(w, h);
	

	playButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, " Play ", { 50,(int)h / 2 + 100,
		app->win->buttonW,app->win->buttonH }, this);
	playButton->state = GuiControlState::NORMAL;

	continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, " Continue ", { 50,(int)h / 2 + 150,
		app->win->buttonW,app->win->buttonH }, this);
	continueButton->state = GuiControlState::NORMAL;

	creditButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, " Credit ", { 50,(int)h / 2 + 200,
		app->win->buttonW,app->win->buttonH }, this);
	creditButton->state = GuiControlState::NORMAL;

	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, " Settings ", { 50,(int)h / 2 + 250,
		app->win->buttonW,app->win->buttonH }, this);
	exitButton->state = GuiControlState::NORMAL;

	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, " Exit ", { 50,(int)h / 2 + 300,
		app->win->buttonW,app->win->buttonH }, this);
	settingsButton->state = GuiControlState::NORMAL;

	checkboxFullScreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 6, "", { 248, 208, 30, 30 }, this);
	checkboxFullScreen->state = GuiControlState::DISABLED;

	checkboxVSync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, "", { 200, 245, 30, 30 }, this);
	checkboxVSync->state = GuiControlState::DISABLED;

	sliderbarMusic = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, 8, "", { 201, 187, 90, 15 }, this, { 201, 186, 20, 15 });
	sliderbarMusic->state = GuiControlState::DISABLED;

	sliderbarFx = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, 9, "", { 172, 148, 90, 15 }, this, { 172, 147, 20, 15 });
	sliderbarFx->state = GuiControlState::DISABLED;

	windowCreditText = app->tex->Load(windowCreditPath);
	windowSettingsText = app->tex->Load(windowSettingsPath);
	
	app->LoadFromFileCheckPastGame();
	if (app->win->previousGame == 0)
	{
		continueButton->state = GuiControlState::DISABLED;
	}

	// ---------- 

	/*lvlSelectorTexture = app->tex->Load(lvlSelectorPath.GetString());
	lvlOneTexture = app->tex->Load(lvlOnePath.GetString());
	lvlTwoTexture = app->tex->Load(lvlTwoPath.GetString());*/

	frame = 0;
	rowFrame = 0;
	alphatoFade = 0.0f;
	alphaFromFade = 1.0f;
	toFade = false;
	fromFade = true;
	mapSelect = true; //no eliminar esta variable ni comentarla
	rowFrameBool = false;
	toFadeButton = false;
	boolExitButton = false;
	boolCreditButton = false;
	boolSettingsButton = true;
	deteleUI = false;

	sliderbarMusic->sliderBounds.x = sliderbarMusic->bounds.x + sliderbarMusic->bounds.w;
	sliderbarFx->sliderBounds.x = sliderbarFx->bounds.x + sliderbarFx->bounds.w;

	return true;
}

bool SceneTitle::Update(float dt)
{
	OPTICK_EVENT();

	chrono.Start(0.18); //el tiempo que pasa entre cada frame de la animación

	if (frame < 109 && chrono.Test() == FIN)
	{
		frame++;
		if (rowFrameBool)
		{
			rowFrame++;
		}
	}
		

	if (frame == 5)
	{
		app->audio->PlayMusic(musicTitlePath,0);
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || toFadeButton || boolContinueButton || deteleUI)
	{
		if (boolContinueButton)
		{
			app->scene->continueFromGame = true;


		}
		toFadeButton = false;
		boolContinueButton = false;
		app->fade->Fade(this, app->scene, 60);

		deteleUI = true;
	}


	//Activar/desactivar fullscreen
	if (checkboxFullScreen->state == GuiControlState::SELECTED) {
		app->win->fullscreen = true;
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
	}
	if (checkboxFullScreen->state == GuiControlState::NORMAL) {
		app->win->fullscreen = false;
		SDL_SetWindowFullscreen(app->win->window, 0);
	}



	//Activar/desactivar VSync
	if (checkboxVSync->state == GuiControlState::SELECTED) {
		app->render->isVsync = true;
		SDL_CreateRenderer(app->win->window, -1, SDL_RENDERER_PRESENTVSYNC);
	}
	if (checkboxVSync->state == GuiControlState::NORMAL) {
		app->render->isVsync = false;
		SDL_CreateRenderer(app->win->window, -1, SDL_RENDERER_ACCELERATED);
	}


	app->audio->volumenMusic = sliderbarMusic->sliderBounds.x - sliderbarMusic->bounds.x;
	if (app->audio->volumenMusic < 0) {
		app->audio->volumenMusic = 0;
	}

	app->audio->volumenFx = sliderbarFx->sliderBounds.x - sliderbarFx->bounds.x;
	if (app->audio->volumenFx < 0) {
		app->audio->volumenFx = 0;
	}

	
	return true;
}

bool SceneTitle::PostUpdate()
{
	OPTICK_EVENT();

	// ---------- Implementación previa a profilling
	//SDL_Rect rect = { 0,0,1024,768 };
	//app->render->DrawTexture(bgTexture[frame], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
	// ---------- 

	// ---------- Implementación posterior a profilling
	if (frame <= 15)
	{
		
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[0], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		rowFrameBool = true;
		//rowFrame++;
		if (frame == 15)
		{
			rowFrame = 0;
			frame++;
		}
	}
	else if (frame <= 30)
	{
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[1], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		//rowFrame++;
		if (frame == 30)
		{
			rowFrame = 0;
			frame++;
		}
		std::cout << "entrando en frame <=30" << std::endl;

	}
	else if (frame <= 45)
	{
		
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[2], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		//rowFrame++;
		if (frame == 45)
		{
			rowFrame = 0;
			frame++;
		}
		std::cout << "entrando en frame <=45" << std::endl;
	}
	else if (frame <= 60)
	{
		
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[3], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		//rowFrame++;
		if (frame == 60)
		{
			rowFrame = 0;
			frame++;
		}
		std::cout << "entrando en frame <=60" << std::endl;
	}
	else if (frame <= 75)
	{
		
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[4], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		//rowFrame++;
		if (frame == 75)
		{
			rowFrame = 0;
			frame++;
		}
		std::cout << "entrando en frame <=75" << std::endl;
	}
	else if (frame <= 90)
	{
		
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[5], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		//rowFrame++;
		if (frame == 90)
		{
			rowFrame = 0;
			frame++;
		}
		std::cout << "entrando en frame <=90" << std::endl;
	}
	else if (frame <= 105 )
	{
		
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[6], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		//rowFrame++;
		if (frame == 105)
		{
			rowFrame = 0;
			frame++;
		}
		std::cout << "entrando en frame <=105" << std::endl;
	}
	else if (frame <= 108)
	{
		
		SDL_Rect rect = { 1024 * rowFrame, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[7], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		//rowFrame++;
		if (frame == 105)
		{
			rowFrame = 0;
			frame++;
		}
		std::cout << "entrando en frame <=108" << std::endl;
	}
	else
	{
		SDL_Rect rect = { 1024 * 2, 0, 1024, 768 };
		app->render->DrawTexture(bgTexture[7], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
	}

	// ---------- 

	if (!boolSettingsButton)
	{
		app->render->DrawTexture(windowSettingsText, 115, 75);

	}

	app->guiManager->Draw();

	if (boolExitButton)
	{
		return false;
	}

	if (boolCreditButton)
	{
		app->render->DrawTexture(windowCreditText, 115, 75);
	}

	if (deteleUI)
	{

		playButton->state = GuiControlState::NONE;
		continueButton->state = GuiControlState::NONE;
		creditButton->state = GuiControlState::NONE;
		exitButton->state = GuiControlState::NONE;
		settingsButton->state = GuiControlState::NONE;

		checkboxFullScreen->state = GuiControlState::NONE;
		checkboxVSync->state = GuiControlState::NONE;

		sliderbarMusic->state = GuiControlState::NONE;
		sliderbarFx->state = GuiControlState::NONE;
	}

	return true;
}

bool SceneTitle::CleanUp()
{
	for (int i = 0; i <= 8; i++) {
		app->tex->UnLoad(bgTexture[i]);
	}

	app->tex->UnLoad(lvlSelectorTexture);
	app->tex->UnLoad(lvlOneTexture);
	app->tex->UnLoad(lvlTwoTexture);
	app->tex->UnLoad(windowCreditText);
	app->tex->UnLoad(windowSettingsText);
	
	return true;
}


bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		toFadeButton = true;
		break;
	case 2:
		LOG("Button 2 click");
		boolContinueButton = true;
		break;
	case 3:
		LOG("Button 1 click");
		if (boolCreditButton)
		{
			boolCreditButton = false;
		}
		else
		{
			boolCreditButton = true;
		}
		break;
	case 4:
		LOG("Button 1 click");
		if (boolSettingsButton)
		{
			if (app->win->fullscreen == true) checkboxFullScreen->state = GuiControlState::SELECTED;
			else checkboxFullScreen->state = GuiControlState::NORMAL;

			if (app->render->isVsync == true) checkboxVSync->state = GuiControlState::SELECTED;
			else checkboxVSync->state = GuiControlState::NORMAL;

			sliderbarMusic->state = GuiControlState::NORMAL;
			sliderbarFx->state = GuiControlState::NORMAL;
			boolSettingsButton = false;
		}
		else
		{
			boolSettingsButton = true;
			checkboxFullScreen->state = GuiControlState::DISABLED;
			checkboxVSync->state = GuiControlState::DISABLED;
			sliderbarMusic->state = GuiControlState::DISABLED;
			sliderbarFx->state = GuiControlState::DISABLED;
		}
		break;
	case 5:
		LOG("Button 2 click");
		boolExitButton = true;
		break;
	}

	return true;
}