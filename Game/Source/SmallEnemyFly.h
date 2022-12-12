#ifndef __SMALL_ENEMY_FLY__
#define __SMALL_ENEMY_FLY__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class SmallEnemyFly : public Entity
{
public:

	SmallEnemyFly();
	virtual ~SmallEnemyFly();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void LoadInfo(iPoint pos, int state);

	void SaveInfo();

	int GetState();

private:

	void ChaseMovement();
	void ChaseMovement2();

	void SentryMovement();

	void ReturnMovement();

public:

	Animation* currentAnimationFlyEnemy = nullptr;

	int limiteSup;
	int limiteInf;
	int scalarLimites;
	int map;

	int speedY;

	Animation idleRFlyAnim;
	Animation idleLFlyAnim;

	Animation chaseRFlyAnim;
	Animation chaseLFlyAnim;

	Animation deathRAnimEnemy;
	Animation deathLAnimEnemy;

	//bool destroy;

	enum estadoSEnemy1 {
		STOP = 0,
		SENTRY,
		CHASE,
		DEATH,
		RETURN,
		NONE,
	};
	int estadoSEF1;

private:

	SDL_Texture* texture;
	SDL_Texture* angryTexture;
	const char* texturePath;
	const char* textureAngryPath;
	SDL_Timer deathAnimationTimer;
	//DONE 4: Add a physics
//	PhysBody* pbody;
	bool firstSentryMovement;


	iPoint initialPosition, leftBorder, rightBorder;
	int range;
	bool startPath;
	float nextFootStepInX;
	float nextFootStepInY;
	float amountToMoveInX;
	float amountToMoveInY;
	float destinationInX;
	float destinationInY;
	bool firstPath;
	bool achievedRightBorder;
	bool achievedLeftBorder;
	bool debug;
	int playerTileX;
	int playerTileY;
	int limitToChase;
	
	struct dataFromSave
	{
		int posX;
		int posY;
		bool startPath;
		float nextFootStepInX;
		float nextFootStepInY;
		float amountToMoveInX;
		float amountToMoveInY;
		float destinationInX;
		float destinationInY;
		bool firstPath;
		bool achievedRightBorder;
		bool achievedLeftBorder;
		bool attackAnimation;
		int estado;
		Animation* animation = nullptr;

	} newData;

	bool changedDataFromSave;

	float speedX;
};

#endif // !__SMALL_ENEMY_FLY__

