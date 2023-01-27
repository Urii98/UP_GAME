#ifndef __CHECKPOINT_H__
#define __CHECKPOINT_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class CheckPoint : public Entity
{
public:

	CheckPoint();
	virtual ~CheckPoint();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isDiscovered = false;
	
	Animation* checkPointCoinAnimation = nullptr;

	//bool destroy;

private:

	SDL_Texture* checkPointTexture;
	const char* checkPointTexturePath;

	Animation checkPoint;
	Animation savedCheckPoint;

	//DONE 4: Add a physics to an item
//	PhysBody* pbody;
};

#endif // __CHECKPOINT_H__