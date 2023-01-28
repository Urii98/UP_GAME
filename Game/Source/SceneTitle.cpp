#include "SceneTitle.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include <iostream>

SceneTitle::SceneTitle(bool isActive) : Module(isActive) {
	name.Create("sceneTitle");
}

// Destructor
SceneTitle::~SceneTitle() {

}

bool SceneTitle::Awake(pugi::xml_node& config) {

	textureTitlePath = config.child("textureTitlePath").attribute("path").as_string();
	lvlSelectorPath = config.child("lvlSelectorPath").attribute("path").as_string();
	lvlOnePath = config.child("lvlOnePath").attribute("path").as_string();
	lvlTwoPath = config.child("lvlTwoPath").attribute("path").as_string();


	musicTitlePath = config.child("musicTitlePath").attribute("path").as_string();
	musicStopPath = config.child("musicStopPath").attribute("path").as_string();


	mainMenuScreenPath = config.child("mainMenuScreenPath").attribute("path").as_string();
	playSelectedPath = config.child("playSelectedPath").attribute("path").as_string();
	newGameSelectedPath = config.child("newGameSelectedPath").attribute("path").as_string();
	settingsSelectedPath = config.child("settingsSelectedPath").attribute("path").as_string();
	exitSelectedPath = config.child("exitSelectedPath").attribute("path").as_string();

	//Default settings 
	defaultSettingsMenuPath = config.child("defaultSettingsMenuPath").attribute("path").as_string();
	xCirclePath = config.child("xCirclePath").attribute("path").as_string();
	xCircleSelectedPath = config.child("xCircleSelectedPath").attribute("path").as_string();

	//AudioSettingsTextures
	audioSelectedPath = config.child("audioSelectedPath").attribute("path").as_string();
	fxBarPath = config.child("fxBarPath").attribute("path").as_string();
	musicBarPath = config.child("musicBarPath").attribute("path").as_string();
	fxCirclePath = config.child("fxCirclePath").attribute("path").as_string();
	musicCirclePath = config.child("musicCirclePath").attribute("path").as_string();

	//Screen Menu Textures
	screenSelectedPath = config.child("screenSelectedPath").attribute("path").as_string();
	fullScreenTruePath = config.child("fullScreenTruePath").attribute("path").as_string();
	vSyncTruePath = config.child("vSyncTruPath").attribute("path").as_string();

	//Credits Menu Textures
	creditsSelectedPath = config.child("creditsSelectedPath").attribute("path").as_string();

	return true;
}

bool SceneTitle::Start() {

	LOG("Loading background assets- SceneTitle");

	bool ret = true;

	char x[120];

	opcion = 0;
	opcionSettings = 0;
	opcionVolumen = 0;
	opcionScreen = 0;
	menuSettings = false;

	for (int i = 0; i <= 146; i++)
	{
		sprintf_s(x, "Assets/Textures/Title/ezgif-frame-%d.jpg", i + 1);
		bgTexture[i] = app->tex->Load(x);
	}

	lvlSelectorTexture = app->tex->Load(lvlSelectorPath.GetString());
	lvlOneTexture = app->tex->Load(lvlOnePath.GetString());
	lvlTwoTexture = app->tex->Load(lvlTwoPath.GetString());

	mainMenuScreen = app->tex->Load(mainMenuScreenPath.GetString());
	playSelected = app->tex->Load(playSelectedPath.GetString());
	newGameSelected = app->tex->Load(newGameSelectedPath.GetString());
	settingsSelected = app->tex->Load(settingsSelectedPath.GetString());
	exitSelected = app->tex->Load(exitSelectedPath.GetString());

	defaultSettingsMenu = app->tex->Load(defaultSettingsMenuPath.GetString());
	xCircle = app->tex->Load(xCirclePath.GetString());
	xCircleSelected = app->tex->Load(xCircleSelectedPath.GetString());

	audioSelected = app->tex->Load(audioSelectedPath.GetString());
	fxBar = app->tex->Load(fxBarPath.GetString());
	musicBar = app->tex->Load(musicBarPath.GetString());
	fxCircle = app->tex->Load(fxCirclePath.GetString());
	musicCircle = app->tex->Load(musicCirclePath.GetString());

	screenSelected = app->tex->Load(screenSelectedPath.GetString());
	fullScreenTrue = app->tex->Load(fullScreenTruePath.GetString());
	vSyncTrue = app->tex->Load(vSyncTruePath.GetString());

	creditsSelected = app->tex->Load(creditsSelectedPath.GetString());

	frame = 0;
	alphatoFade = 0.0f;
	alphaFromFade = 1.0f;
	toFade = false;
	fromFade = true;
	mapSelect = true;
	levelSelected;

	return true;
}

bool SceneTitle::Update(float dt)
{
	int x, y, left = 1;
	app->input->GetMousePosition(x, y);
	LOG("Mouse x: %d, Mouse y: %d", x, y);
	SDL_Rect playbt = { 1053,170,231,70 };
	SDL_Rect newGamebt = { 1053,315,465,70 };
	SDL_Rect settingsbt = { 1050,462,410,70 };
	SDL_Rect exitbt = { 1050,610,215,70 };


	SDL_Rect audiobt = { 272,156,164,49 };
	SDL_Rect screenbt = { 272,267,197,49 };
	SDL_Rect creditsbt = { 272,375,218,49 };

	chrono.Start(0.18); //el tiempo que pasa entre cada frame de la animación

	if (frame < 145 && chrono.Test() == FIN)
		frame++;

	if (frame == 5)
	{
		app->audio->PlayMusic(musicTitlePath,0);
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->audio->PlayMusic(musicStopPath, 2.0);
		app->fade->Fade(this, app->scene, 60);
	}

	SDL_Rect rect = { 0,0,1024,768 };
	app->render->DrawTexture(bgTexture[frame], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);

	if (!menuSettings) {
		//seleccionar opció
		if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)) {
			if (opcion >= 0 && opcion < 3) {
				opcion++;
			}
		}
		if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)) {
			if (opcion > 0 && opcion <= 3) {
				opcion--;
			}
		}

		if (x > playbt.x && x<(playbt.x + playbt.w) && y>playbt.y && y < (playbt.y + playbt.h)) {
			opcion = 0;
		}
		else if (x > newGamebt.x && x<(newGamebt.x + newGamebt.w) && y>newGamebt.y && y < (newGamebt.y + newGamebt.h)) {
			opcion = 1;
		}
		else if (x > settingsbt.x && x<(settingsbt.x + settingsbt.w) && y>settingsbt.y && y < (settingsbt.y + settingsbt.h)) {
			opcion = 2;
		}
		else if (x > exitbt.x && x<(exitbt.x + exitbt.w) && y>exitbt.y && y < (exitbt.y + exitbt.h)) {
			opcion = 3;
		}
		switch (opcion)
		{
			//Play current saved game
		case 0:
			app->render->camera.y = 0;
			app->render->DrawTexture(playSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);


			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN)
			{
				active = false;
				app->scene->active = true;
			}
			break;

			//NewGame
		case 1:
			app->render->camera.y = 0;
			//Falta colocar el render
			app->render->DrawTexture(newGameSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN) {
				//app->scene->player.playedBefore = false;
				//app->scene->reloj = 0;
				active = false;
				app->scene->active = true;
			}

			break;
			//Settings
		case 2:
			app->render->DrawTexture(settingsSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			//Menu de ajustes
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN) {
				menuSettings = true;
			}


			/*
			app->win->fullscreen_window;
			app->win->fullscreen;*/
			break;
			//Exit
		case 3:


			app->render->camera.y = 0;

			app->render->DrawTexture(exitSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			if (app->input->GetKey(app->input->GetKey(SDL_SCANCODE_RETURN)) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN)
			{
				return false;
			}
			break;
		}
	}
	else if (menuSettings) {
		//seleccionar opció

		if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)) {
			if (opcion >= 0 && opcion < 2) {
				opcionSettings++;
			}
		}
		if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)) {
			if (opcion > 0 && opcion <= 2) {
				opcionSettings--;

			}
		}
		if (x > audiobt.x && x<(audiobt.x + audiobt.w) && y>audiobt.y && y < (audiobt.y + audiobt.h)) {
			opcionSettings = 0;

		}
		else if (x > screenbt.x && x<(screenbt.x + screenbt.w) && y>screenbt.y && y < (screenbt.y + screenbt.h)) {
			opcionSettings = 1;

		}
		else if (x > creditsbt.x && x<(creditsbt.x + creditsbt.w) && y>creditsbt.y && y < (creditsbt.y + creditsbt.h)) {
			opcionSettings = 2;

		}

		app->render->DrawTexture(defaultSettingsMenu, -app->render->camera.x, -app->render->camera.y, NULL, 1);
		app->render->DrawTexture(xCircle, -app->render->camera.x, -app->render->camera.y, NULL, 1);

		SDL_Rect xCircleRect = { 1293,138,71,67 };
		if (x > xCircleRect.x && x<(xCircleRect.x + xCircleRect.w) && y>xCircleRect.y && y < (xCircleRect.y + xCircleRect.h)) {
			app->render->DrawTexture(xCircleSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN)
			{
				menuSettings = false;
				app->SaveGameRequest();
				//loadPreConfig = true;
				app->LoadGameRequest();
			}

		}
		SDL_Rect musicZone = { 535,223,855,104 };
		SDL_Rect fxZone = { 535,376,855,104 };
		SDL_Rect fullScreenZone = { 1016,202,78,78 };
		SDL_Rect vSyncZone = { 1016,372,78,78 };
		SDL_Rect fullScreenMouseZone = { 529,195,861,97 };
		SDL_Rect vSyncMouseZone = { 529,361,861,97 };

		switch (opcionSettings)
		{
		case 0:
			app->render->DrawTexture(audioSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			app->render->DrawTexture(fxBar, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			app->render->DrawTexture(musicBar, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			app->render->DrawTexture(fxCircle, -app->render->camera.x + fxCircle_X, -app->render->camera.y + 393, NULL, 1);
			app->render->DrawTexture(fxCircle, -app->render->camera.x + musicCircle_X, -app->render->camera.y + 237, NULL, 1);


			if (x > fxZone.x && x<(fxZone.x + fxZone.w) && y>fxZone.y && y < (fxZone.y + fxZone.h)) {
				opcionVolumen = 1;

			}
			else if (x > musicZone.x && x<(musicZone.x + musicZone.w) && y>musicZone.y && y < (musicZone.y + musicZone.h)) {
				opcionVolumen = 0;

			}
			if ((app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)) {
				if (opcion >= 0 && opcion < 1) {
					opcionVolumen++;
				}
			}
			if ((app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)) {
				if (opcion > 0 && opcion <= 1) {
					opcionVolumen--;

				}
			}

			switch (opcionVolumen)
			{
			case 0:

				/*if (app->input->GetMouseButtonDown(left) == KEY_REPEAT && app->input->mouseX <= 1290 && app->input->mouseX >= 820 && app->input->mouseY >= 244 && app->input->mouseY <= 304)
				{
					musicCircle_X = app->input->mouseX - 40;
				}
				app->audio->volume_mix_max_music = ((musicCircle_X / 470) - 1.7446808851) * 128;

				app->audio->volume_mix_max_music = app->audio->volume_mix_max_music + 10;

				if (app->audio->volume_mix_max_music < 0)
				{
					app->audio->volume_mix_max_music = 0;
				}*/

				break;
			case 1:

				/*if (app->input->GetMouseButtonDown(left) == KEY_REPEAT && app->input->mouseX <= 1290 && app->input->mouseX >= 820 && app->input->mouseY >= 400 && app->input->mouseY <= 460)
				{
					fxCircle_X = app->input->mouseX - 40;
				}

				app->audio->volume_mix_max_wav = ((fxCircle_X / 470) - 1.7446808851) * 128;

				app->audio->volume_mix_max_wav = app->audio->volume_mix_max_wav + 10;

				if (app->audio->volume_mix_max_wav < 0)
				{
					app->audio->volume_mix_max_wav = 0;
				}*/

				//LOG("fxSound: %0.4f", app->audio->volume_mix_max_wav);

				break;
			}


			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN)
			{

				//audioMenu = true;;
			}


			break;
		case 1:
			app->render->DrawTexture(screenSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			/*if (app->win->fullscreen) {
				app->render->DrawTexture(fullScreenTrue, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			}
			if (app->render->vSync == true) {
				app->render->DrawTexture(vSyncTrue, -app->render->camera.x, -app->render->camera.y, NULL, 1);
			}
			if (x > fullScreenMouseZone.x && x<(fullScreenMouseZone.x + fullScreenMouseZone.w) && y>fullScreenMouseZone.y && y < (fullScreenMouseZone.y + fullScreenMouseZone.h)) {
				opcionScreen = 0;
			}
			else if (x > vSyncMouseZone.x && x<(vSyncMouseZone.x + vSyncMouseZone.w) && y>vSyncMouseZone.y && y < (vSyncMouseZone.y + vSyncMouseZone.h)) {
				opcionScreen = 1;
			}*/
			switch (opcionScreen)
			{
			case 0:
				//if (x > fullScreenZone.x && x<(fullScreenZone.x + fullScreenZone.w) && y>fullScreenZone.y && y < (fullScreenZone.y + fullScreenZone.h)) {
				//	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN)
				//	{
				//		app->win->fullscreen = !app->win->fullscreen;
				//		app->SaveGameRequest();
				//		//app->win->CleanUp();
				//		//app->win->Awake();
				//		app->win->active = false;
				//		app->win->active = true;
				//		if (app->win->fullscreen == true) {
				//			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
				//		}
				//		else {
				//			SDL_SetWindowFullscreen(app->win->window, 0);
				//		}
				//	}
				//}
				break;
			case 1:
				//if (x > vSyncZone.x && x<(vSyncZone.x + vSyncZone.w) && y>vSyncZone.y && y < (vSyncZone.y + vSyncZone.h)) {

				//	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN)
				//	{
				//		app->render->vSync = !app->render->vSync;
				//		app->SaveGameRequest();

				//		//vSyncMarc = !vSyncMarc;
				//	}
				//}
				break;
			}
			break;
		case 2:
			app->render->DrawTexture(creditsSelected, -app->render->camera.x, -app->render->camera.y, NULL, 1);

			/*if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetMouseButtonDown(left) == KEY_DOWN)
			{
				audioMenu = true;;
			}*/
			break;
		}

	}
	//if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)//GODMODE
	//{
	//	mapSelect = true;
	//	levelSelected = true;
	//}

	//if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)//GODMODE
	//{
	//	mapSelect = false;
	//	levelSelected = true;

	//}
	
	return true;
}

bool SceneTitle::PostUpdate()
{
	
	
	
	//
	if (!levelSelected)
	//{
	//	app->render->DrawTexture(lvlSelectorTexture,0,0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
	//}
	//else
	//{
	//	if (mapSelect)
	//	{
	//		app->render->DrawTexture(lvlOneTexture, 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
	//	}
	//	else
	//	{
	//		app->render->DrawTexture(lvlTwoTexture, 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
	//	}
	//}

	return true;
}

bool SceneTitle::CleanUp()
{
	for (int i = 0; i <= NUM_IMAGES; i++) {
		app->tex->UnLoad(bgTexture[i]);
	}

	app->tex->UnLoad(lvlSelectorTexture);
	app->tex->UnLoad(lvlOneTexture);
	app->tex->UnLoad(lvlTwoTexture);

	app->tex->UnLoad(mainMenuScreen);
	app->tex->UnLoad(playSelected);
	app->tex->UnLoad(newGameSelected);
	app->tex->UnLoad(settingsSelected);
	app->tex->UnLoad(exitSelected);

	app->tex->UnLoad(defaultSettingsMenu);
	app->tex->UnLoad(xCircle);
	app->tex->UnLoad(xCircleSelected);

	app->tex->UnLoad(audioSelected);
	app->tex->UnLoad(fxBar);
	app->tex->UnLoad(musicBar);
	app->tex->UnLoad(fxCircle);
	app->tex->UnLoad(musicCircle);

	app->tex->UnLoad(screenSelected);
	app->tex->UnLoad(fullScreenTrue);
	app->tex->UnLoad(vSyncTrue);

	app->tex->UnLoad(creditsSelected);
	
	return true;
}