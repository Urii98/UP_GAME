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

	app->win->SetTitle(title.GetString());

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x.png");

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

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	iPoint mouseTile = app->map->WorldToMap((mouseX + (-(app->render->camera.x)-260))/32,
											(mouseY + -(app->render->camera.y)+320)/32 - app->map->mapData.tileHeight / 2);


	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//Test compute path function
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, mouseTile);
			originSelected = false;
		}
		else
		{
			origin = mouseTile;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
	}

	// L12: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		pruebas = true;
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

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

	
	//vect->clear();
	//delete vect;

	app->tex->UnLoad(img);

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