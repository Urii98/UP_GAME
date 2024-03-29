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

	bool Update(float dt);

	void PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Player::ChangePosition(int x, int y);
	
	void DrawLifePoints();

public:

	enum estadoPlayer
	{
		STOP = 0,
		MOVIMIENTO,
		DEATH,
		VICTORY,
		NONE,
		SKILL,
		HIT,
	};
	int estadoP;

	enum collisionPlayer {
		NOCOLLISION,
		COLLISION
	};
	int collisionP;

	enum direccion {
		IZQUIERDA,
		DERECHA,
	};
	int direccionP, direccionE;

	int speedX;
	int speedY;
	int speedYDown;

	int scalarSpeedX;
	int scalarSpeedY;
	int scalarSpeedYDown;

	bool oneJump;
	bool flying;
	bool victory;
	int flapLimit;

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
	
	Animation swordAttackRAnim;
	Animation swordAttackLAnim;
	
	Animation hitR, hitL;
	SDL_Timer hitTimer;
	SDL_Timer invulnerableTime;
	bool invulnerable;

	transformPosition teleport;

	int pickCoinFxId;
	int swordFxId;
	int playerDmgId;
	int hpId;

	int posXBeforeAttack;
	int posYBeforeAttack;

	bool skillSwitch;
	bool drawSwordUI;
	bool timerDead;
	bool sword;
	bool saveScoreFromVictory;
	int lifePoints;
	int moneyPoints;

	int playerCheckPoint;
	int checkPointsDiscovered;
	int posXFromCheckPoint;
	int highestScore;
	bool collidingWithCheckPoint;
private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	SDL_Texture* swordUITexture;
	SDL_Texture* swordUIOffTexture;
	SDL_Texture* lifePointsTexture;
	// L07 DONE 5: Add physics to the player - declare a Physics body
//	PhysBody* pbody;

	//PhysBody* sensor;

	const char* texturePath;
	const char* swordUIPath;
	const char* swordUIOffPath;
	const char* lifePath;

	
	int kirbyDeathFx;
	int kirbyVictoryFx;

	SDL_Timer jumpTimer;
	SDL_Timer flyTimer;
	SDL_Timer toDescent;
	SDL_Timer deathTimer;
	SDL_Timer flapLimitTimer;

	void Movimiento();
	void Camera();
	void PlayerDebug();
	void PlayerVictory();
	void TeleportToCheckPoint(int numCheckPoint);
	//ColliderType CheckCollision();
	
	const char* coinSFx;
	const char* deathSFx;
	const char* victorySFx;
	const char* hpSFx;

	const char* musicScenePath;
	const char* musicStopPath;
	const char* swordFxPath;
	const char* playerDmgPath;
	

};

#endif // __PLAYER_H__