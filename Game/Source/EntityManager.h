#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager(bool isActive);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	bool LoadState(pugi::xml_node&);

	bool SaveState(pugi::xml_node&);

	void SaveEntities(pugi::xml_node&, ListItem<Entity*>* entity, iPoint pos);
	void LoadEntities(pugi::xml_node&, ListItem<Entity*>* entity);

public:
	List<Entity*> entities;
	int numSmallEnemy1;
	int numSmallEnemy2;
	int numSmallEnemyFly;
	int posCheckPointX, posCheckPointY;

	bool checkLoadFirstTime;
	

};

#endif // __ENTITYMANAGER_H__
