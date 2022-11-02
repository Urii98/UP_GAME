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
	
	LOG("Loading background assets - SceneLogo");

	bool ret = true;

	//char x[120];

	//for (int i = 1; i <= 149 ; i++)
	//{
	//	sprintf_s(x, "Assets/Textures/Title/ezgif-frame-%d.jpg", i + 1);
	//	bgTexture[i] = app->tex->Load(x);
	//}
	//frame = 0;

	textureLogo = app->tex->Load("Assets/Textures/Logo.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

bool SceneLogo::Update(float dt)
{
	std::cout << "SCENELOGO" << std::endl;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->Fade(this, (Module*)app->sceneTitle, 30);
		
		app->sceneLogo->active = false; //esto fuerza que se desactive esta clase
		//lo que se supone que haría la funcion de fade de arriba, pero que no va

		app->sceneTitle->active = true;
		

		//#chapuza1

	}

	/*chrono.Start(0.04);

	if(frame <147 && chrono.Test() == FIN)
		frame++;*/


	return true;
}






bool SceneLogo::PostUpdate()
{

	//app->render->DrawTexture(bgTexture[frame], -360, 0, NULL);

	app->render->DrawTexture(textureLogo, 0-360, 0, NULL);

	

	return true;
}

bool SceneLogo::CleanUp()
{
	//memoryleak
	app->tex->UnLoad(textureLogo);
	return true;
}
