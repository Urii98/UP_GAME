#include "SceneEnding.h"

#include "App.h"
#include "Textures.h"
#include "Player.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"

#include "SceneTitle.h"


#include "GuiButton.h"
#include "Window.h"
#include "GuiManager.h"

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

	uint w, h;

	app->win->GetWindowSize(w, h);
	button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 50,(int)h / 2 - 30,100,20 }, this);
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Button 2", { (int)w / 2 - 50,(int)h / 2,100,20 }, this);
	return true;
}

bool SceneEnding::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && ending)
	{
		if (app->sceneTitle->mapSelect == true){
			//posicion inicial del level 1
			app->scene->player->ChangePosition(125, 600);
			app->scene->player->deathFxbool = false;
			app->scene->player->winFxbool = false;
			app->scene->player->death.Reset();
		}
		else if (app->sceneTitle->mapSelect == false)
		{
			//posicion inicial del level 2
			app->scene->player->ChangePosition(125, 600);
			app->scene->player->deathFxbool = false;
			app->scene->player->winFxbool = false;
			app->scene->player->death.Reset();
		}

		app->scene->player->estadoP = app->scene->player->MOVIMIENTO;
		app->scene->player->currentAnimation = &app->scene->player->idleRAnim;
		app->scene->player->pbody->body->SetActive(true);
		app->scene->player->lifePoints = 3;
		ending = false;
		app->scene->timer.Start(); 
		
	}

	return true;
}

bool SceneEnding::PostUpdate()
{
	SDL_Rect rect = { 0,0,1024,768 };

	if(ending)
	app->render->DrawTexture(textureSceneEnding, 15, 13, &rect, 1.0f, 0.0, 2147483647, 2147483647, false);

	//app->guiManager->Draw();

	return true;
}

bool SceneEnding::CleanUp()
{
	//memoryleak
	//app->tex->UnLoad(textureSceneEnding);
	return true;
}

bool SceneEnding::OnGuiMouseClickEvent(GuiControl* control)
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