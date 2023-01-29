#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"
#include "Textures.h"
#include "Physics.h"
#include <iostream>

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	buttonSelected = app->audio->LoadFx("Assets/Audio/Fx/button_selected.wav");
	buttonPressed = app->audio->LoadFx("Assets/Audio/Fx/button_pressed.wav");
	buttonDisabled = app->audio->LoadFx("Assets/Audio/Fx/button_disable.wav");

	buttonTexture = app->tex->Load("Assets/Textures/Button.png");

}

GuiButton::~GuiButton()
{
	delete buttonTexture;
}

bool GuiButton::Update(float dt)
{
	bool ret = true;

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	mouseX *= app->win->GetScale();
	mouseY *= app->win->GetScale();

	// Check collision between mouse and button bounds
	if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
		(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
	{
		if (state != GuiControlState::DISABLED)
		{
			state = GuiControlState::FOCUSED;
			if (!mouseIn) {
				app->audio->PlayFx(buttonSelected); mouseIn = true;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;

			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				app->audio->PlayFx(buttonPressed);
				NotifyObserver();
			}
		}
		else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
		{
//			app->audio->PlayFx(buttonDisabled);
		}
			
	}
	else if (state != GuiControlState::DISABLED)
	{
		state = GuiControlState::NORMAL, mouseIn = false;
	}
	

	return false;
}




bool GuiButton::Draw(Render* render)
{
	
	SDL_Rect rect;

	switch (state)
	{
	case GuiControlState::DISABLED:
		if (app->physics->debug)
		{
		render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		}
		else
		{
			rect = { 192,55,55,23 };
			render->DrawTexture(buttonTexture, bounds.x / 2, bounds.y / 2, &rect);
		}
		break;
	case GuiControlState::NORMAL:
		if (app->physics->debug)
		{
			render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		}
		else
		{
			rect = { 21,55,55,23 };
			render->DrawTexture(buttonTexture, bounds.x/2, bounds.y/2, &rect);
		}
		
		break;
	case GuiControlState::FOCUSED:
		if (app->physics->debug)
		{
			render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		}
		else
		{
			rect = { 78,55,55,23 };
			render->DrawTexture(buttonTexture, bounds.x / 2, bounds.y / 2, &rect);
		}
		
		break;
	case GuiControlState::PRESSED:
		if (app->physics->debug)
		{
			render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		}
		else
		{
			rect = { 135,55,55,23 };
			render->DrawTexture(buttonTexture, bounds.x / 2, bounds.y / 2, &rect);
		}
		
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, {255,255,255});

	return false;
}