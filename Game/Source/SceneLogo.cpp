#include "SceneLogo.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"


SceneLogo::SceneLogo() : Module() {
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
	chrono.Start(10);

	if (chrono.Test())
	{
		onOffSwitch = true;
	}

	app->fade->Fade(this, (Module*)app->scene, 30);

	if (onOffSwitch)
	{
		app->fade->Fade(this, (Module*)app->scene, 30);
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->scene, 30);
	}

	return true;
}

bool SceneLogo::PostUpdate()
{
	//app->render->DrawTexture(bgTexture, 0/*-360*/, 0, NULL);
	return true;
}

bool SceneLogo::CleanUp()
{
	app->tex->UnLoad(bgTexture);
	return true;
}
