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

	musicTitlePath = config.child("musicTitlePath").attribute("path").as_string();
	musicStopPath = config.child("musicStopPath").attribute("path").as_string();

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

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->audio->PlayMusic(musicStopPath, 2.0);
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
	
	return true;
}