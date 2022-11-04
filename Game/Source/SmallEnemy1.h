#ifndef __SMALL_ENEMY_1__
#define __SMALL_ENEMY_1__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SDL_Timer.h"
#include "Animation.h"

struct SDL_Texture;

class SmallEnemy1 : public Entity
{
public:

	SmallEnemy1();
	virtual ~SmallEnemy1();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

private:

	void Movimiento();
	void Ataque();

public:

	bool isPicked = false;
	Animation* currentAnimationEnemy = nullptr;

	int limiteIzqX;
	int limiteDerX;
	int scalarLimites;

	bool walkDir;
	bool attackE;
	bool animAtk;
	int speedX;

	Animation walkRAnimEnemy;
	Animation walkLAnimEnemy;
	Animation angryRAnimEnemy;
	Animation angryLAnimEnemy;
	Animation attackRAnimEnemy;
	Animation attackLAnimEnemy;

	bool destroy;

	enum estadoSEnemy1 {
		STOP = 0,
		MOVIMIENTO,
		ATAQUE,
		DEATH,
		NONE,
	};
	int estadoSE1;

	enum direccionEnemy {
		IZQUIERDA,
		DERECHA,
	};
	int direccionE;

private:

	SDL_Texture* texture;
	const char* texturePath;
	SDL_Timer attackAnimTimer;
	//DONE 4: Add a physics
	PhysBody* pbody;
};

#endif // !__SMALL_ENEMY_1__

