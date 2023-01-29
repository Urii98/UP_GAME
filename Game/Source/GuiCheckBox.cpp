#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Log.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;

	checkBoxTex = app->tex->Load("Assets/Textures/checkbox.png");
	cbeckBoxPressed = app->audio->LoadFx("Assets/Audio/Fx/button_pressed.wav");
}

GuiCheckBox::~GuiCheckBox()
{
	delete checkBoxTex;
}

bool GuiCheckBox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			if (previousState != state)
			{
				LOG("Change state from %d to %d", previousState, state);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				(state == GuiControlState::NORMAL) ? state = GuiControlState::SELECTED : state = GuiControlState::NORMAL;
			}

			
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
				app->audio->PlayFx(cbeckBoxPressed);
			}
		}
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render)
{
	SDL_Rect rect;
	
	
	switch (state)
	{
	case GuiControlState::NORMAL:
	{
		rect = { 0, 0, 28, 27 };
		render->DrawTexture(checkBoxTex, bounds.x, bounds.y, &rect);
		
	} break;
	case GuiControlState::SELECTED:
	{
		rect = { 28, 0, 28, 27 };
		render->DrawTexture(checkBoxTex, bounds.x, bounds.y, &rect);
	} break;

	}

	return false;
}