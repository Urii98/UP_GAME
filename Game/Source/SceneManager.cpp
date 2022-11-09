#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneManager.h"


SceneManager::SceneManager(bool isActive) : Module(isActive)
{
	name.Create("sceneManager");
}

SceneManager::~SceneManager()
{}

bool SceneManager::Start()
{
	return true;
}

void SceneManager::Menu()
{
	//esto es para printar un menu en la pantalla y que en funcion de lo que hagamos y de la escena en la queestemos pues que pase a un lado u otro



}

bool SceneManager::Update(float dt)
{
	
	switch (currentScene)
	{
	case(NONE):
		break;
	case(LOGO):

		break;
	case(TITLE):
		break;
	case(LEVEL1):
		Menu();
		break;
	case(LEVEL2):
		Menu();
		break;
	case(ENDING):
		Menu();
		break;
	default:
		break;
	}

	return true;
}

bool SceneManager::PostUpdate()
{
	return true;
}

bool SceneManager::CleanUp()
{
	return true;
}