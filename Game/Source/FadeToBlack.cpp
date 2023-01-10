#include "FadeToBlack.h"

#include "App.h"
#include"Render.h"
#include "Log.h"
#include <iostream>

#include "SDL/include/SDL_render.h"

#include"EntityManager.h"
#include "Map.h"
FadeToBlack::FadeToBlack(bool isActive) : Module(isActive)
{
	screenRect = { 0, 0, 1024, 768 };
	name.Create("fade");
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
//	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		//alpha += 0.01f;
		alpha += 0.625f *dt;
		std::cout << "Alpha: " << alpha << std::endl;
		if (alpha > 1.00f)
		{
			moduleToDisable->Disable();

			/*if (moduleToEnable->name == "scene")
			{
				app->entityManager->Enable();
				app->map->Enable();
			}*/
			moduleToEnable->Enable();
			if (moduleToEnable->name == "scene")
			{
				app->entityManager->Enable();
				app->map->Enable();
			}
			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		//alpha -= 0.01f;
		std::cout << "Alpha: " << alpha << std::endl;
		alpha -= 0.625f * dt;
		if (alpha <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return true;


}

bool FadeToBlack::PostUpdate()
{
	

  	if (currentStep == Fade_Step::NONE) return true;


	if (currentStep == Fade_Step::TO_BLACK)
	{
		// Render the black square with alpha on the screen
		SDL_Rect rect = { 0,0,1024,768 };
		app->render->DrawRectangle(rect, 0, 0, 0, (unsigned char)(255.0f * alpha));
	}


	return true;
}

bool FadeToBlack::Fade(Module* moduleToDisablee, Module* moduleToEnablee, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisablee;
		this->moduleToEnable = moduleToEnablee;

		ret = true;
	}

	return ret;
}
