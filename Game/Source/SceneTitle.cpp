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
	musicTitlePath = config.child("musicTitlePath").attribute("path").as_string();
	musicStopPath = config.child("musicStopPath").attribute("path").as_string();

	return true;
}

bool SceneTitle::Start() {

	LOG("Loading background assets- SceneTitle");

	bool ret = true;

	char x[120];

	for (int i = 1; i <= 149; i++)
	{
		sprintf_s(x, "Assets/Textures/Title/ezgif-frame-%d.jpg", i + 1);
		bgTexture[i] = app->tex->Load(x);
	}
	frame = 0;

	return true;
}

bool SceneTitle::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (chrono.Test() == EJECUTANDO || chrono.Test() == FIN))
	{
		app->fade->Fade(this, (Module*)app->scene, 30);

		
		app->audio->PlayMusic(musicStopPath, 2.0);
		app->sceneTitle->active = false; //esto fuerza que se desactive esta clase
		//lo que se supone que haría la funcion de fade de arriba, pero que no va

		//Aqui tendría que ir esto: Pero si cuando se cargan los modulos scene no esta en active=true, despues cuando la activo no se ve el mapa
		app->scene->active = true;

		//#chapuza1

	}

	
	chrono.Start(0.18);

	if (frame < 147 && chrono.Test() == FIN)
		frame++;

	if (frame == 5)
	{
		app->audio->PlayMusic(musicTitlePath, 2.0);
	}
	else if (frame == 147)
	{
		app->audio->PlayMusic(musicStopPath, 2.0);
	}
	
	return true;
}

bool SceneTitle::PostUpdate()
{

	app->render->DrawTexture(bgTexture[frame], -360, 0, NULL);

	
	return true;
}

bool SceneTitle::CleanUp()
{
	for (int i = 0; i < NUM_IMAGES; i++) {
		app->tex->UnLoad(bgTexture[i]);
	}
	return true;
}