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
		//LOG("Mouse x: %d Mouse y: %d", mouseX, mouseY);
		//LOG("bounds.x: %d bounds.h: %d", bounds.x, bounds.y);

		GuiControlState previousState = state;

		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			//state = GuiControlState::FOCUSED;

			if (previousState != state)
			{
				LOG("Change state from %d to %d", previousState, state);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				(state == GuiControlState::NORMAL) ? state = GuiControlState::SELECTED : state = GuiControlState::NORMAL;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}
		}
	}

	return false;
}

bool GuiCheckBox::Draw(Render* render)
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	
	// Draw the right CheckBox depending on state
	switch (state)
	{

	case GuiControlState::DISABLED:
	{

	} break;

	case GuiControlState::NORMAL:
	{
		rect = { 0, 0, 28, 27 };
		render->DrawTexture(checkBoxTex, bounds.x, bounds.y, &rect);
		
	} break;


	case GuiControlState::FOCUSED:
	{
	
	} break;
	case GuiControlState::PRESSED:
	{

	} break;

	case GuiControlState::SELECTED:
	{
		rect = { 28, 0, 28, 27 };
		render->DrawTexture(checkBoxTex, bounds.x, bounds.y, &rect);
	} break;

	default:
		break;
	}

	return false;
}