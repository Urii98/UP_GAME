#ifndef __GUI_SLIDERBAR_H__
#define __GUI_SLIDERBAR_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiSliderBar : public GuiControl
{
public:

	GuiSliderBar(uint32 id, SDL_Rect bounds, SDL_Rect sliderBounds);
	virtual ~GuiSliderBar();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	SDL_Rect sliderBounds;

	SDL_Texture* sliderBar;
	SDL_Texture* slider;

	uint sliderPressed;
};

#endif // __GUI_SLIDERBAR_H__