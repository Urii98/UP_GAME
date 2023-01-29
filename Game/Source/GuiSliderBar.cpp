#include "GuiSliderBar.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Log.h"
#include "Window.h"

GuiSliderBar::GuiSliderBar(uint32 id, SDL_Rect bounds, SDL_Rect sliderBounds) : GuiControl(GuiControlType::SLIDERBAR, id)
{
	this->bounds = bounds;
	this->sliderBounds = sliderBounds;

	sliderBar = app->tex->Load("Assets/Textures/sliderBar.png");
	slider = app->tex->Load("Assets/Textures/slider.png");

	sliderPressed = app->audio->LoadFx("Assets/Audio/Fx/button_pressed.wav");
}

GuiSliderBar::~GuiSliderBar()
{
	delete sliderBar;
	delete slider;
}

bool GuiSliderBar::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

			if (previousState != state)
			{
				LOG("Change state from %d to %d", previousState, state);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				sliderBounds.x = mouseX;	
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				app->audio->PlayFx(sliderPressed);
				NotifyObserver();
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiSliderBar::Draw(Render* render)
{
	SDL_Rect rect;

	render->DrawTexture(sliderBar, bounds.x, bounds.y);

	// Draw the right SliderBar depending on state
	switch (state)
	{

	case GuiControlState::NORMAL:
	{

		render->DrawTexture(slider, sliderBounds.x, sliderBounds.y);

	} break;

	case GuiControlState::FOCUSED:
	{

		render->DrawTexture(slider, sliderBounds.x, sliderBounds.y);

	} break;

	case GuiControlState::PRESSED:
	{

		render->DrawTexture(slider, sliderBounds.x, sliderBounds.y);

	} break;

	}
	
	return false;
}