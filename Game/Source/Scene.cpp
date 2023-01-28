#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "Defs.h"
#include "Log.h"

#include "SceneTitle.h"

#include "Optick/include/optick.h"

Scene::Scene(bool isActive) : Module(isActive)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{

		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;

		entities.Add(item);


	
		
	}

	for (pugi::xml_node itemNode = config.child("SmallEnemy1"); itemNode; itemNode = itemNode.next_sibling("SmallEnemy1"))
	{

			SmallEnemy1* item = (SmallEnemy1*)app->entityManager->CreateEntity(EntityType::SMALLENEMY1);
			item->parameters = itemNode;

			entities.Add(item);
		
	}

	for (pugi::xml_node itemNode = config.child("SmallEnemyFly"); itemNode; itemNode = itemNode.next_sibling("SmallEnemyFly"))
	{

		SmallEnemyFly* item = (SmallEnemyFly*)app->entityManager->CreateEntity(EntityType::SMALLENEMYFLY);
		item->parameters = itemNode;

		entities.Add(item);
		
		
	}

	for (pugi::xml_node itemNode = config.child("SmallEnemy2"); itemNode; itemNode = itemNode.next_sibling("SmallEnemy2"))
	{

		SmallEnemy2* item = (SmallEnemy2*)app->entityManager->CreateEntity(EntityType::SMALLENEMY2);
		item->parameters = itemNode;
		entities.Add(item);



	}

	for (pugi::xml_node itemNode = config.child("hpitem"); itemNode; itemNode = itemNode.next_sibling("hpitem"))
	{
	
		HpItem* item = (HpItem*)app->entityManager->CreateEntity(EntityType::HPITEM);
		item->parameters = itemNode;

		entities.Add(item);

	}

	for (pugi::xml_node itemNode = config.child("checkpoint"); itemNode; itemNode = itemNode.next_sibling("checkpoint"))
	{

		CheckPoint* item = (CheckPoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		item->parameters = itemNode;

		entities.Add(item);

	}

	playerSensor = (PlayerSensors*)app->entityManager->CreateEntity(EntityType::SENSORS);


	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	//vect->push_back(player);
	entities.Add(player);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{	

	// L03: DONE: Load map
	retLoad = app->map->Load();

	//// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		w += 1;
		h += 1;
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	//app->win->SetTitle(title.GetString());

	// Texture to highligh mouse position 
	point = app->tex->Load("Assets/Maps/point.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x.png");
	timer.Start();

	return true;
}


// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	OPTICK_EVENT();

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	// Draw map
	app->map->Draw();

	return true;
}



// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(img);
	app->tex->UnLoad(point);
	app->tex->UnLoad(originTex);


	ListItem<Entity*>* destroyEntities;
	destroyEntities = entities.start;

	while (destroyEntities != NULL)
	{
			
		app->entityManager->DestroyEntity(destroyEntities->data);
		RELEASE(destroyEntities->data);
		destroyEntities = destroyEntities->next;
	}

	


	//memoryleak
	return true;
}

bool Scene::LoadState(pugi::xml_node& data)
{
	
	return true;
}


bool Scene::SaveState(pugi::xml_node& data)
{
	
	return true;
}

void Scene::checkPointIterator(CheckPoint& dir)
{
	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL)
	{
		
		if (item->data->name == "checkpoint")
		{
			if (item->data->pbody->listener == &dir)
			{
				item->data->isCurrent = true;
				app->SaveGameRequest();
			}
			else
			{
				item->data->isCurrent = false;
			}
		}
		item = item->next;
	}

}