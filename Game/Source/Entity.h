#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

struct Collider;

enum class EntityType
{
	PLAYER,
	ITEM,
	SMALLENEMY1,
	SMALLENEMY2,
	SMALLENEMYFLY,
	SENSORS,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	//virtual bool PostUpdate()
	//{
	//	return true;
	//}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {
	
	};

	virtual void LoadInfo(iPoint pos, int state)
	{

	}

	virtual void SaveInfo()
	{

	}

	virtual int GetState()
	{
		return 0;
	}

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;

	PhysBody* pbody;
	bool destroy;
};

#endif // __ENTITY_H__