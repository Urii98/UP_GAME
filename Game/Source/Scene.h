#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "SmallEnemy1.h"
#include "SmallEnemyFly.h"
#include <vector>


struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool isActive);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node&);

	bool SaveState(pugi::xml_node&);

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	std::vector<Entity*>* vect = new std::vector<Entity*>;


private:
	SDL_Texture* img;

};

#endif // __SCENE_H__