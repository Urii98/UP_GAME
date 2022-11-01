#include "SceneLogo.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include <iostream>


SceneLogo::SceneLogo(bool isActive) : Module(isActive) {
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo() {

}

bool SceneLogo::Awake() {

	return true;
}

bool SceneLogo::Start() {
	onOffSwitch = false;
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = app->tex->Load("Assets/Textures/Negro.png");


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

bool SceneLogo::Update(float dt)
{
	std::cout << "SCENELOGO" << std::endl;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 30);
		
		app->sceneLogo->active = false; //esto fuerza que se desactive esta clase
		//lo que se supone que haría la funcion de fade de arriba, pero que no va
		

		//#chapuza1

	}

	return true;
}






bool SceneLogo::PostUpdate()
{
	app->render->DrawTexture(bgTexture, 0-360, 0, NULL);



	return true;
}

bool SceneLogo::CleanUp()
{
	app->tex->UnLoad(bgTexture);
	return true;
}
