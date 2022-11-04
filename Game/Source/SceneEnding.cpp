#include "SceneEnding.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"

SceneEnding::SceneEnding(bool isActive) : Module(isActive) {
	name.Create("sceneEnding");
}

// Destructor
SceneEnding::~SceneEnding() {

}

bool SceneEnding::Awake(pugi::xml_node& config) {


	textureSceneEndingPath = config.child("textureSceneEndingPath").attribute("path").as_string();

	return true;
}

bool SceneEnding::Start() {

	textureSceneEnding = app->tex->Load(textureSceneEndingPath.GetString());
	ending = false;
	return true;
}

bool SceneEnding::Update(float dt)
{
	return true;
}

bool SceneEnding::PostUpdate()
{
	SDL_Rect rect = { 0,0,1024,768 };
	if(ending)
	app->render->DrawTexture(textureSceneEnding, -247, -50, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);

	return true;
}

bool SceneEnding::CleanUp()
{
	//memoryleak
	app->tex->UnLoad(textureSceneEnding);
	return true;
}