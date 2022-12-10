#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	int map;
	bool isPicked = false;
	Animation* currentCoinAnimation = nullptr;

	SDL_Timer timeToDestroy;

	//bool destroy;

private:

	SDL_Texture* coinTexture;
	const char* coinTexturePath;

	SDL_Texture* effectTexture;
	const char* effectTexturePath;

	Animation coinAnimation;
	Animation effectAnimation;

	bool soundFX;

	//DONE 4: Add a physics to an item
//	PhysBody* pbody;
};

#endif // __ITEM_H__