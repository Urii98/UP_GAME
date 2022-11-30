#ifndef __PLAYER_SENSORS_H__
#define __PLAYER_SENSORS_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"

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

	bool resetjumps;
	int frames;

	PhysBody* sensArr[100];
};

#endif // __PLAYER_SENSORS_H__