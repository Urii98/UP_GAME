#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SDL_Timer.h"
#include "Animation.h"
#include "Map.h"
#include <iostream>


struct SDL_Texture;

struct transformPosition {
	float posX;
	float posY;
	bool turn;

};

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

	void Player::ChangePosition(int x, int y);

public:

	enum estadoPlayer
	{
		STOP = 0,
		MOVIMIENTO,
		DEATH,
		VICTORY,
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

	int scalarSpeedX;
	int scalarSpeedY;
	int scalarSpeedYDown;

	bool oneJump;
	bool victory;

	int prevPosition;

	bool godMode;

	bool deathFxbool;
	bool winFxbool;

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
	Animation win;

	transformPosition teleport;

	
		

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;

	
	// L07 DONE 5: Add physics to the player - declare a Physics body
//	PhysBody* pbody;

	//PhysBody* sensor;


	const char* texturePath;

	int pickCoinFxId;
	int kirbyDeathFx;
	int kirbyVictoryFx;

	SDL_Timer jumpTimer;
	SDL_Timer flyTimer;
	SDL_Timer toDescent;
	SDL_Timer deathTimer;
	SDL_Timer flapLimitTimer;
	bool flying;
	int flapLimit;

	void Movimiento();
	void Camera();
	void PlayerDebug();
	void PlayerVictory();
	//ColliderType CheckCollision();
	
	const char* coinSFx;
	const char* deathSFx;
	const char* victorySFx;

	const char* musicScenePath;
	const char* musicStopPath;

};

#endif // __PLAYER_H__