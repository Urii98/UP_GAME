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

bool SceneEnding::Awake() {
	return true;
}

bool SceneEnding::Start() {
	return true;
}

bool SceneEnding::Update()
{
	return true;
}

bool SceneEnding::PostUpdate()
{
	return true;
}

bool SceneEnding::CleanUp()
{
	return true;
}