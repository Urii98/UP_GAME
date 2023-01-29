#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "ModuleFonts.h"
#include "PlayerSensors.h"
#include "GuiManager.h"
#include "Window.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"


ModuleFonts::ModuleFonts(bool isActive) : Module(isActive)
{
	name.Create("fonts");
}

ModuleFonts::~ModuleFonts()
{

}

// Load assets
bool ModuleFonts::Start()
{
	LOG("Loading Fonts");

	char lookupTableChars[] = { " !'#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[/]^_ abcdefghijklmnopqrstuvwxyz{|}~ çüéâäàaçêëèïîìäaéÆæôöòûù" };
	textFont = app->fonts->Load("Assets/pixel_font.png", lookupTableChars, 8);

	for (int i = 0; i < 4; i++)
	{
		hourglassAnim.PushBack({ 45 * i,0,45,56 });
	}
	hourglassAnim.loop = true;
	hourglassAnim.speed = 0.78125f * (app->maxFrameDuration / 1000.0);
	

	currentAnimation = &hourglassAnim;

	starCoinUIPath = "Assets/Textures/starCoinUI3.png";
	starCoinUITexture = app->tex->Load(starCoinUIPath);

	hourglassPath = "Assets/Textures/hourglass.png";
	hourglassTexture = app->tex->Load(hourglassPath);

	podiumPath = "Assets/Textures/podium.png";
	podiumTexture = app->tex->Load(podiumPath);

	skillOffPath = "Assets/Textures/swordUIOff.png";
	skillOff = app->tex->Load(skillOffPath);
	skillOnPath = "Assets/Textures/swordUI.png";
	skillOn = app->tex->Load(skillOnPath);

	windowSettingsPath = "Assets/Textures/Window3.png";

	canTestSkill = false;
	sceneReady = false;
	boolSettingsButton = true;

	uint w, h;
	app->win->GetWindowSize(w, h);

	resumeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, " Resume ", { 50,(int)h / 2 + 150,
	app->win->buttonW,app->win->buttonH }, this);
	resumeButton->state = GuiControlState::NONE;

	titleButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, " TitleScreen ", { 50,(int)h / 2 + 200,
		app->win->buttonW,app->win->buttonH }, this);
	titleButton->state = GuiControlState::NONE;

	exitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, " Exit ", { 50,(int)h / 2 + 250,
		app->win->buttonW,app->win->buttonH }, this);
	exitButton->state = GuiControlState::NONE;

	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, " Settings ", { 50,(int)h / 2 + 300,
		app->win->buttonW,app->win->buttonH }, this);
	settingsButton->state = GuiControlState::NONE;

	checkboxFullScreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 6, "", { 248, 208, 30, 30 }, this);
	checkboxFullScreen->state = GuiControlState::DISABLED;

	checkboxVSync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, "", { 200, 245, 30, 30 }, this);
	checkboxVSync->state = GuiControlState::DISABLED;

	sliderbarMusic = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, 8, "", { 201, 187, 90, 15 }, this, { 201, 186, 20, 15 });
	sliderbarMusic->state = GuiControlState::DISABLED;

	sliderbarFx = (GuiSliderBar*)app->guiManager->CreateGuiControl(GuiControlType::SLIDERBAR, 9, "", { 172, 148, 90, 15 }, this, { 172, 147, 20, 15 });
	sliderbarFx->state = GuiControlState::DISABLED;


	windowSettingsText = app->tex->Load(windowSettingsPath);

	sliderbarMusic->sliderBounds.x = sliderbarMusic->bounds.x + sliderbarMusic->bounds.w;
	sliderbarFx->sliderBounds.x = sliderbarFx->bounds.x + sliderbarFx->bounds.w;

	return true;
}


// Load new texture from file path
int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}
	
	SDL_Texture* tex = app->tex->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// TODO 1: Finish storing font data

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// char_w --------	Width of each character
	// char_h --------	Height of each character

	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	font.totalLength = strlen(characters);
	font.columns = fonts[id].totalLength / rows;

	uint tex_w, tex_h;
	app->tex->GetSize(tex, tex_w, tex_h);
	font.char_w = tex_w / font.columns;
	font.char_h = tex_h / font.rows;

	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void ModuleFonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr)
	{
		app->tex->UnLoad(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void ModuleFonts::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		// TODO 2: Find the character in the table and its position in the texture, then Blit
		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 0.0f, 0.0f,2147483647, 2147483647,false);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}



bool ModuleFonts::Update(float dt) //para que aparezca durante el juego
{
	bool ret = true;
	if (sceneReady)
	{
		if (!app->scene->activateUI)
		{
			resumeButton->state = GuiControlState::NONE;
			titleButton->state = GuiControlState::NONE;
			exitButton->state = GuiControlState::NONE;
			settingsButton->state = GuiControlState::NONE;
		}

	}

	//Activar/desactivar fullscreen
	if (checkboxFullScreen->state == GuiControlState::SELECTED) {
		app->win->fullscreen = true;
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
	}
	if (checkboxFullScreen->state == GuiControlState::NORMAL) {
		app->win->fullscreen = false;
		SDL_SetWindowFullscreen(app->win->window, 0);
	}

	//Activar/desactivar VSync
	if (checkboxVSync->state == GuiControlState::SELECTED) {
		app->render->isVsync = true;
		SDL_CreateRenderer(app->win->window, -1, SDL_RENDERER_PRESENTVSYNC);
	}
	if (checkboxVSync->state == GuiControlState::NORMAL) {
		app->render->isVsync = false;
		SDL_CreateRenderer(app->win->window, -1, SDL_RENDERER_ACCELERATED);
	}

	if (sceneReady)
	{
		app->audio->volumenMusic = sliderbarMusic->sliderBounds.x - sliderbarMusic->bounds.x;
		if (app->audio->volumenMusic < 0) {
			app->audio->volumenMusic = 0;
		}

		app->audio->volumenFx = sliderbarFx->sliderBounds.x - sliderbarFx->bounds.x;
		if (app->audio->volumenFx < 0) {
			app->audio->volumenFx = 0;
		}

		app->audio->SetVolumeMusic(app->audio->volumenMusic);
	}



	return ret;
}


bool ModuleFonts::PostUpdate() //para que aparezca en screenlogo y screentitle
{
	bool ret = true;

	if (sceneReady)
	{
		//StarCoinUI
		std::string moneyToString = std::to_string(app->scene->player->moneyPoints);
		const char* moneyChar = moneyToString.c_str();
		BlitText(45, 9, textFont, moneyChar);
		app->render->DrawTexture(starCoinUITexture, 15, -5, 0, 0.0f, 0.0f, 2147483647, 2147483647, false);

		//podiumUI
		std::string podiumToString = std::to_string(app->scene->player->highestScore);
		const char* podiumChar = podiumToString.c_str();
		BlitText(450, 11, textFont, podiumChar);
		app->render->DrawTexture(podiumTexture, 425, 0, 0, 0.0f, 0.0f, 2147483647, 2147483647, false);


		//hourglassUI
		currentAnimation->Update();
		std::string timeToString = std::to_string(app->scene->timer.ReadSec());
		const char* timeChar = timeToString.c_str();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(hourglassTexture, 70, 0, &rect, 0.0f, 0.0f, 2147483647, 2147483647, false);
		BlitText(110, 9, textFont, timeChar);

		//skillUI
		if (canTestSkill)
		{
			app->render->DrawTexture(skillOn, 20, 30, 0, 0.0f, 0.0f, 2147483647, 2147483647, true);

		}
		else
		{
			app->render->DrawTexture(skillOff, 20, 30, 0, 0.0f, 0.0f, 2147483647, 2147483647, true);
		}

		if (!boolSettingsButton)
		{
			app->render->DrawTexture(windowSettingsText, 115, 75,0, 0.0f, 0.0f, 2147483647, 2147483647, true);

		}


		if (!app->scene->activateUI)
		{
			resumeButton->state = GuiControlState::NONE;
			titleButton->state = GuiControlState::NONE;
			exitButton->state = GuiControlState::NONE;
			settingsButton->state = GuiControlState::NONE;
		}
		else
		{
			resumeButton->state = GuiControlState::NORMAL;
			titleButton->state = GuiControlState::NORMAL;
			exitButton->state = GuiControlState::NORMAL;
			settingsButton->state = GuiControlState::NORMAL;
			app->guiManager->Draw();
		}

		if (boolExitButton)
		{
			return false;
		}
	}


	return ret;
}

bool ModuleFonts::CleanUp()
{
	bool ret = true;
	UnLoad(textFont);
	app->tex->UnLoad(starCoinUITexture);
	app->tex->UnLoad(hourglassTexture);
	app->tex->UnLoad(podiumTexture);
	app->tex->UnLoad(windowSettingsText);
	

	return ret;
}

bool ModuleFonts::OnGuiMouseClickEvent(GuiControl* control)
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
	case 3:
		LOG("Button 1 click");
	
		break;
	case 4:
		LOG("Button 1 click");
		if (boolSettingsButton)
		{
			if (app->win->fullscreen == true) checkboxFullScreen->state = GuiControlState::SELECTED;
			else checkboxFullScreen->state = GuiControlState::NORMAL;

			if (app->render->isVsync == true) checkboxVSync->state = GuiControlState::SELECTED;
			else checkboxVSync->state = GuiControlState::NORMAL;

			sliderbarMusic->state = GuiControlState::NORMAL;
			sliderbarFx->state = GuiControlState::NORMAL;
			boolSettingsButton = false;
		}
		else
		{
			boolSettingsButton = true;
			checkboxFullScreen->state = GuiControlState::DISABLED;
			checkboxVSync->state = GuiControlState::DISABLED;
			sliderbarMusic->state = GuiControlState::DISABLED;
			sliderbarFx->state = GuiControlState::DISABLED;
		}
		break;
	case 5:
		LOG("Button 2 click");
		boolExitButton = true;
		break;
	}

	return true;
}