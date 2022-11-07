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

private:

	void Movimiento();

public:

	Animation* currentAnimationEnemy = nullptr;

	int limiteIzqX;
	int limiteDerX;
	int scalarLimites;

	bool walkDir;
	int speedX;

	Animation walkRAnimEnemy;
	Animation walkLAnimEnemy;

	bool destroy;

	enum estadoSEnemy1 {
		STOP = 0,
		MOVIMIENTO,
		DEATH,
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
	PhysBody* pbody;
};

#endif // !__SMALL_ENEMY_2__
