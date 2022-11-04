#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SDL_Timer.h"
#include "Animation.h"
#include <iostream>


struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	void PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	enum estadoPlayer
	{
		STOP = 0,
		MOVIMIENTO,
		DEATH,
		NONE,
	};
	int estadoP;

	enum collisionPlayer {
		NOCOLLISION,
		COLLISION
	};
	int collisionP;

	enum direccionPlayer {
		IZQUIERDA,
		DERECHA,
	};
	int direccionP;
	
	int speedX;
	int speedY;
	int speedYDown;
	bool oneJump;

	int prevPosition;

	Animation* currentAnimation = nullptr; 

	Animation idleRAnim;
	Animation idleLAnim;
	Animation walkRAnim;
	Animation walkLAnim;
	Animation downRAnim;
	Animation downLAnim;
	Animation runRAnim;
	Animation runLAnim;
	Animation jumpRAnim;
	Animation jumpLAnim;
	Animation preflyRAnim;
	Animation flyRAnim;
	Animation preflyLAnim;
	Animation flyLAnim;
	Animation death;


private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;

	



	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	int pickCoinFxId;
	int kirbyDeathFx;

	SDL_Timer jumpTimer;
	SDL_Timer flyTimer;
	SDL_Timer toDescent;
	SDL_Timer deathTimer;
	SDL_Timer flapLimitTimer;
	bool flying;
	int flapLimit;

	void Movimiento();
	//ColliderType CheckCollision();
	
	const char* coinSFx;
	const char* deathSFx;

};

#endif // __PLAYER_H__