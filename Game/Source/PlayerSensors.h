#ifndef __PLAYER_SENSORS_H__
#define __PLAYER_SENSORS_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL_Timer.h"
#include "Animation.h"

class PlayerSensors : public Entity
{
public:
	PlayerSensors();
	virtual ~PlayerSensors();

	bool Awake();

	bool Start();

	bool Update();

	void PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	PhysBody* jumpSensor; 

	PhysBody* skill;
	SDL_Timer skillTimer;
	bool skillSwitch = false;

	bool resetjumps;
	int frames;

	Animation aux;

	
};

#endif // __PLAYER_SENSORS_H__