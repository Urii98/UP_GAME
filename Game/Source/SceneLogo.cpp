#include "SceneLogo.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include <iostream>
#include <stdio.h>

#include "SceneEnding.h"


SceneLogo::SceneLogo(bool isActive) : Module(isActive) {
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo() {

}

bool SceneLogo::Awake(pugi::xml_node& config) {

	bool ret = true;

	textureLogoPath = config.child("textureLogoPath").attribute("path").as_string();
	alpha = 0.0f;
	toFade = false;


	return ret;
}

bool SceneLogo::Start() {
	
	LOG("Loading background assets - SceneLogo");

	bool ret = true;

	textureLogo = app->tex->Load(textureLogoPath.GetString());

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

bool SceneLogo::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		toFade = true;

	}

	if (toFade)
	{
		alpha += 0.01f;
	}

	if (alpha > 1.00f)
	{
		toFade = false;

		app->fade->Fade(this, (Module*)app->sceneTitle, 30);

		app->sceneLogo->active = false; //esto fuerza que se desactive esta clase
		//lo que se supone que haría la funcion de fade de arriba, pero que no va

		app->sceneTitle->active = true;

		//#chapuza1

	}




	return true;
}






bool SceneLogo::PostUpdate()
{
	SDL_Rect rect = { 0,0,1024,768 };

	app->render->DrawTexture(textureLogo, 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);

	app->render->DrawRectangle(rect, 0, 0, 0, (unsigned char)(255.0f * alpha));

	return true;
}

bool SceneLogo::CleanUp()
{
	//memoryleak
	app->tex->UnLoad(textureLogo);
	return true;
}
