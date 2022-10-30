#include "SceneTitle.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"

SceneTitle::SceneTitle() : Module() {
	name.Create("sceneTitle");
}

// Destructor
SceneTitle::~SceneTitle() {

}

bool SceneTitle::Awake() {
	return true;
}

bool SceneTitle::Start() {
	return true;
}

bool SceneTitle::Update()
{
	return true;
}

bool SceneTitle::PostUpdate()
{
	return true;
}

bool SceneTitle::CleanUp()
{
	return true;
}