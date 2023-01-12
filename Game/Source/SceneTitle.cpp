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

	return true;
}

bool SceneTitle::Start() {

	LOG("Loading background assets- SceneTitle");

	bool ret = true;

	char x[120];

	for (int i = 0; i <= 146; i++)
	{
		sprintf_s(x, "Assets/Textures/Title/ezgif-frame-%d.jpg", i + 1);
		bgTexture[i] = app->tex->Load(x);
	}

	lvlSelectorTexture = app->tex->Load(lvlSelectorPath.GetString());
	lvlOneTexture = app->tex->Load(lvlOnePath.GetString());
	lvlTwoTexture = app->tex->Load(lvlTwoPath.GetString());

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
	OPTICK_EVENT();

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

	
	return true;
}

bool SceneTitle::PostUpdate()
{
	SDL_Rect rect = { 0,0,1024,768 };
	
	app->render->DrawTexture(bgTexture[frame], 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
	//
	//if (!levelSelected)
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
	
	return true;
}