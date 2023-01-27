#ifndef __HPITEM_H__
#define __HPITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class HpItem : public Entity
{
public:

	HpItem();
	virtual ~HpItem();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	int map;
	bool isPicked = false;
	Animation* currentHpAnimation = nullptr;

	SDL_Timer timeToDestroy;

	//bool destroy;

private:

	SDL_Texture* hpTexture;
	const char* hpTexturePath;

	SDL_Texture* effectTexture;
	const char* effectTexturePath;

	Animation hpAnimation;
	Animation effectAnimation;

	bool soundFX;

	//DONE 4: Add a physics to an item
//	PhysBody* pbody;
};

#endif // __HPITEM_H__