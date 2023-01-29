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

#include "Optick/include/optick.h"

#include "GuiButton.h"
#include "Window.h"
#include "GuiManager.h"


SceneLogo::SceneLogo(bool isActive) : Module(isActive) {
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo() {

}

bool SceneLogo::Awake(pugi::xml_node& config) {

	bool ret = true;

	textureLogoPath = config.child("textureLogoPath").attribute("path").as_string();
	btnTextureAtlas = app->tex->Load("Assets/Textures/atlasbutton.png");
	alpha = 0.0f;
	toFade = false;
	prueba = false;

	return ret;
}

bool SceneLogo::Start() {
	
	LOG("Loading background assets - SceneLogo");

	bool ret = true;

	textureLogo = app->tex->Load(textureLogoPath.GetString());

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	uint w, h;
	
	app->win->GetWindowSize(w, h);
	button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 50,(int)h / 2 - 80,app->win->buttonW,app->win->buttonH }, this);
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Button 2", { (int)w / 2 - 50,(int)h / 2,app->win->buttonW,app->win->buttonH }, this);

	return true;
}

bool SceneLogo::Update(float dt)
{
	OPTICK_EVENT();

	if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		prueba = true;

	}


	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->Fade(this, app->sceneTitle, 100);

	}

	

	return true;
}


bool SceneLogo::PostUpdate()
{
	if (!prueba)
	{
		SDL_Rect rect = { 0,0,1024,768 };

		app->render->DrawTexture(textureLogo, 0, 0, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);

		app->render->DrawRectangle(rect, 0, 0, 0, (unsigned char)(255.0f * alpha));
	}

	app->guiManager->Draw();

	return true;
}

bool SceneLogo::CleanUp()
{
	//memoryleak
	app->tex->UnLoad(textureLogo);

	return true;
}


bool SceneLogo::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		break;
	case 2:
		LOG("Button 2 click");
		break;
	}

	return true;
}