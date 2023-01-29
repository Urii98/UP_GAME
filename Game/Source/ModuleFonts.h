#pragma once

#include "Module.h"
#include <iostream>
#include<string>

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
	// Lookup table. All characters displayed in the same order as the texture
	char table[MAX_FONT_CHARS];

	// The font texture
	SDL_Texture* texture = nullptr;

	// Font setup data
	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;
};

class ModuleFonts : public Module
{
public:

	// Constructor
	ModuleFonts(bool isActive);

	// Destructor
	~ModuleFonts();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the texture file
	// Param characters		- The lookup table. All characters displayed in the same order as the texture
	// Param rows			- The amount of character rows in the texture
	int Load(const char* texturePath, const char* characters, uint rows = 1);

	// Removes a font by its index
	// Unloads the texture and removes it from the fonts array
	void UnLoad(int fontIndex);

	// Create a surface from text
	void BlitText(int x, int y, int fontIndex, const char* text) const;

	bool OnGuiMouseClickEvent(GuiControl* control);


private:
	// An array to keep track and store all loaded fonts
	Font fonts[MAX_FONTS];

	SDL_Texture* starCoinUITexture;
	const char* starCoinUIPath;

	SDL_Texture* podiumTexture;
	const char* podiumPath;

	SDL_Texture* hourglassTexture;
	const char* hourglassPath;

	Animation* currentAnimation = nullptr;
	Animation hourglassAnim;

	SDL_Texture* skillOff;
	SDL_Texture* skillOn;
	const char* skillOffPath;
	const char* skillOnPath;

	GuiButton* resumeButton;
	GuiButton* titleButton;
	GuiButton* exitButton;
	GuiButton* settingsButton;
	

public:
	
	int textFont = -1;
	bool canTestSkill;
	bool sceneReady;



};

