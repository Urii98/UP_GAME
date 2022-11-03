#ifndef __SCENE_ENDING_H__
#define __SCENE_ENDING_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class SceneEnding : public Module
{
public:

	SceneEnding(bool isActive);

	// Destructor
	virtual ~SceneEnding();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();




public:
	SDL_Texture* bgTexture = nullptr;

	bool ending;

	SDL_Texture* textureSceneEnding;
	SString textureSceneEndingPath;

};



#endif // !__SCENE_ENDING_H__



