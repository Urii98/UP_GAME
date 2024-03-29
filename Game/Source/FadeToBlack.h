#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"
#include "SDL/include/SDL_rect.h"

class FadeToBlack : public Module
{
public:

	FadeToBlack(bool isActive);

	// Destructor
	virtual ~FadeToBlack();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();


	// Called from another module
	// Starts the fade process which has two steps, fade_out and fade_in
	// After the first step, the modules should be switched
	bool Fade(Module* toDisable, Module* toEnable, float frames = 60);

	float alpha;

private:

	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = Fade_Step::NONE;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	// The rectangle of the screen, used to render the black rectangle
	SDL_Rect screenRect;

	// The modules that should be switched after the first step
	
	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;

};

#endif // !__FADETOBLACK_H__


