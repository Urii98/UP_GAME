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

private:

	void Movimiento();

public:

	Animation* currentAnimationFlyEnemy = nullptr;

	int limiteSup;
	int limiteInf;
	int scalarLimites;
	int map;

	int speedY;

	Animation flyAnimEnemy;
	Animation deathAnimEnemy;

	//bool destroy;

	enum estadoSEnemy1 {
		STOP = 0,
		MOVIMIENTO,
		DEATH,
		NONE,
	};
	int estadoSEF1;

private:

	SDL_Texture* texture;
	const char* texturePath;
	SDL_Timer deathAnimationTimer;
	//DONE 4: Add a physics
//	PhysBody* pbody;
};

#endif // !__SMALL_ENEMY_FLY__

