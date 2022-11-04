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

	for (int i = 0; i <= 149; i++)
	{
		sprintf_s(x, "Assets/Textures/Title/ezgif-frame-%d.jpg", i + 1);
		bgTexture[i] = app->tex->Load(x);
	}
	frame = 0;
	alphatoFade = 0.0f;
	alphaFromFade = 1.0f;
	toFade = false;
	fromFade = true;

	return true;
}

bool SceneTitle::Update(float dt)
{

	if (!fromFade)
	{
	/*	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && (chrono.Test() == EJECUTANDO || chrono.Test() == FIN))
		{
			toFade = true;
		}*/
		
		chrono.Start(0.18);

		if (frame < 147 && chrono.Test() == FIN)
			frame++;

		if (frame == 5)
		{
			app->audio->PlayMusic(musicTitlePath, 2.0);
		}
		else if (frame >= 147)
		{
			app->audio->PlayMusic(musicStopPath, 2.0);
		}

		if (toFade && alphatoFade < 1.0f && frame == 147)
		{
			alphatoFade += 0.05f;
		}

		if ((alphatoFade > 1.00f && frame == 147) || app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			toFade = false;

			app->fade->Fade(this, (Module*)app->scene, 30);

			app->audio->PlayMusic(musicStopPath, 2.0);
			app->sceneTitle->active = false; //esto fuerza que se desactive esta clase
			//lo que se supone que haría la funcion de fade de arriba, pero que no va por ahora

			//Aqui tendría que ir esto: Pero si cuando se cargan los modulos scene no esta en active=true, despues cuando la activo no se ve el mapa

			app->scene->active = true;

			//#chapuza1
		}


	}
	else if(fromFade == true)
	{
		alphaFromFade -= 0.008f;

		if (alphaFromFade < 0.03f)
		{
			fromFade = false;
			toFade = true;
		}

	}
	



	
	return true;
}

bool SceneTitle::PostUpdate()
{
	SDL_Rect rect = { 0,0,1024,768 };
	SDL_Rect rect2 = { -247,0,1024,768 };
	
	if (!fromFade)
	{
		app->render->DrawTexture(bgTexture[frame], -247, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);

		app->render->DrawRectangle(rect2, 0, 0, 0, (unsigned char)(255.0f * alphatoFade));
	}
	else
	{
		app->render->DrawTexture(bgTexture[1], -247, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);
		app->render->DrawRectangle(rect2, 0, 0, 0, (unsigned char)(255.0f * alphaFromFade));
	}

	
	return true;
}

bool SceneTitle::CleanUp()
{
	for (int i = 0; i <= NUM_IMAGES; i++) {
		app->tex->UnLoad(bgTexture[i]);
	}

	
	
	return true;
}