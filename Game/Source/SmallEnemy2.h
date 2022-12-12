#ifndef __SMALL_ENEMY_2__
#define __SMALL_ENEMY_2__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class SmallEnemy2 : public Entity
{
public:

	SmallEnemy2();
	virtual ~SmallEnemy2();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void LoadInfo(iPoint pos, int state);

private:

	void SentryMovement2();
	void SentryMovement();

public:

	Animation* currentAnimationEnemy = nullptr;

	int limiteIzqX;
	int limiteDerX;
	int scalarLimites;
	int map;

	bool walkDir;
	int speedinX;

	Animation walkRAnimEnemy;
	Animation walkLAnimEnemy;

	//bool destroy;

	enum estadoSEnemy1 {
		STOP = 0,
		SENTRY,
		CHASE,
		DEATH,
		RETURN,
		NONE,
	};
	int estadoSE2;

	enum direccionEnemy {
		IZQUIERDA,
		DERECHA,
	};
	int direccionSE2;

private:

	SDL_Texture* texture;
	const char* texturePath;
	SDL_Timer deathAnimationTimer;
	//DONE 4: Add a physics
//	PhysBody* pbody;

	iPoint initialPosition, leftBorder, rightBorder;
	int range;
	bool startPath;
	float nextFootStep;
	float amountToMoveInX;
	float destination;
	bool firstPath;
	bool achievedRightBorder;
	bool achievedLeftBorder;
	bool debug;
	
	bool changeDataFromSave;
	int posXFromSave;
	int posYFromSave;

	int lastPosinX;
	int lastPosinY;
	int framesStopped;

	float speedX, speedLimit;
};

#endif // !__SMALL_ENEMY_2__

