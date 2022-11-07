#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"


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
//		vect->push_back(item);
		
	}

	for (pugi::xml_node itemNode = config.child("SmallEnemy1"); itemNode; itemNode = itemNode.next_sibling("SmallEnemy1"))
	{
		SmallEnemy1* item = (SmallEnemy1*)app->entityManager->CreateEntity(EntityType::SMALLENEMY1);
		item->parameters = itemNode;
		//vect->push_back(item);
	}

	for (pugi::xml_node itemNode = config.child("SmallEnemyFly"); itemNode; itemNode = itemNode.next_sibling("SmallEnemyFly"))
	{
		SmallEnemyFly* item = (SmallEnemyFly*)app->entityManager->CreateEntity(EntityType::SMALLENEMYFLY);
		item->parameters = itemNode;
		//vect->push_back(item);
	}


	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	vect->push_back(player);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	//if (app->map->active == false)
	//{
	//	app->map->active = true;
	//	app->map->Start();
	//}

	//if (app->entityManager->active == false)
	//{
	//	app->entityManager->active = true;
	//	app->entityManager->Start();
	//}


	// L03: DONE: Load map
	app->map->Load();

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

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

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	

	// Draw map
	app->map->Draw();

	//if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	//{
	//	this->active = false;
	//	CleanUp();
	//}

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

	vect->clear();

	delete vect;

	app->tex->UnLoad(img);

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