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
	

	playButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, " Play ", { 50,(int)h/2 + 100,
		app->win->buttonW,app->win->buttonH }, this);
	playButton->state = GuiControlState::NORMAL;

	continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, " Continue ", { 50,(int)h / 2 +150,
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

	windowCreditText = app->tex->Load(windowCreditPath);
	
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

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || toFadeButton || boolContinueButton)
	{
		if (boolContinueButton)
		{
			app->scene->continueFromGame = true;


		}
		toFadeButton = false;
		boolContinueButton = false;
		app->fade->Fade(this, app->scene, 60);
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
	// ---------- 

	app->guiManager->Draw();

	if (boolExitButton)
	{
		return false;
	}

	if (boolCreditButton)
	{
		app->render->DrawTexture(windowCreditText, 115, 75);
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
		LOG("Button 2 click");
		break;
	case 5:
		LOG("Button 2 click");
		boolExitButton = true;
		break;
	}

	return true;
}