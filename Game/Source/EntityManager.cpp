#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include "Scene.h"
#include "Window.h"
#include "SmallEnemy1.h"
#include "SmallEnemy2.h"
#include "SmallEnemyFly.h"
#include "PlayerSensors.h"
#include "HpItem.h"
#include "CheckPoint.h"

EntityManager::EntityManager(bool isActive) : Module(isActive)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
			pEntity = item->data;

			if (pEntity->active == false) continue;
			ret = item->data->Start();
	}

	numSmallEnemy1 = 0;
	numSmallEnemy2 = 0;
	numSmallEnemyFly = 0;


	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{

		ret = item->data->CleanUp();
		item = item->prev;

	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;

	case EntityType::ITEM:
		entity = new Item();
		break;

	case EntityType::SMALLENEMY1:
		entity = new SmallEnemy1();
		break;

	case EntityType::SMALLENEMYFLY:
		entity = new SmallEnemyFly();
		break;

	case EntityType::SMALLENEMY2:
		entity = new SmallEnemy2();
		break;

	case EntityType::SENSORS:
		entity = new PlayerSensors();
		break;

	case EntityType::HPITEM:
		entity = new HpItem();
		break;

	case EntityType::CHECKPOINT:
		entity = new CheckPoint();
		break;
	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}


	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		this->active = false;
	}

	return ret;
}


bool EntityManager::LoadState(pugi::xml_node& data)
{
	int posX = data.child("playerScene1").attribute("x").as_int();
	int posY = data.child("playerScene1").attribute("y").as_int();
	int money = data.child("playerScene1").attribute("money").as_int(); 
	int hp = data.child("playerScene1").attribute("hp").as_int();


	app->scene->player->moneyPoints = money;
	app->scene->player->lifePoints = hp;
	app->scene->player->ChangePosition(posX, posY);
	//if (posCheckPointX != 0)
	//{
	//	app->scene->player->ChangePosition(posCheckPointX, posCheckPointY);
	//}
	//else
	//{
	//	app->scene->player->ChangePosition(posX, posY);
	//}
	

	//Comprobar de alguna manera que no se han destruido los enemigos,
	//de no haberse destruido, entonces cogemos sus datos y los mandamos a cada uno para su posición con un teleport

	ListItem<Entity*>* dataEntities;
	dataEntities = entities.start;

	while (dataEntities != NULL)
	{
		if (dataEntities->data->destroy != true)
		{
			LoadEntities(data, dataEntities);
			
		}
		dataEntities = dataEntities->next;

	}

	numSmallEnemy1 = 0;
	numSmallEnemy2 = 0;
	numSmallEnemyFly = 0;

	return true;
}

void EntityManager::LoadEntities(pugi::xml_node& data, ListItem<Entity*>* entity)
{

	if (entity->data->name == "SmallEnemy1")
	{
		numSmallEnemy1++;

		std::string aux = "SmallEnemy1";
		std::string nameEntity = aux + std::to_string(numSmallEnemy1);
		const char* nameToPrint = nameEntity.c_str();

		int posX = data.child(nameToPrint).attribute("x").as_int();
		int posY = data.child(nameToPrint).attribute("y").as_int();

		iPoint entityPos = { posX, posY };
		int entityState = data.child(nameToPrint).attribute("state").as_int();

		entity->data->LoadInfo(entityPos, entityState);


	}
	else if (entity->data->name == "SmallEnemy2")
	{
		numSmallEnemy2++;

		std::string aux = "SmallEnemy2";
		std::string nameEntity = aux + std::to_string(numSmallEnemy2);
		const char* nameToPrint = nameEntity.c_str();

		int posX = data.child(nameToPrint).attribute("x").as_int();
		int posY = data.child(nameToPrint).attribute("y").as_int();

		iPoint entityPos = { posX, posY };

		entity->data->LoadInfo(entityPos,0);


	}
	else if (entity->data->name == "SmallEnemyFly")
	{
		numSmallEnemyFly++;

		std::string aux = "SmallEnemyFly";
		std::string nameEntity = aux + std::to_string(numSmallEnemyFly);
		const char* nameToPrint = nameEntity.c_str();

		int posX = data.child(nameToPrint).attribute("x").as_int();
		int posY = data.child(nameToPrint).attribute("y").as_int();

		iPoint entityPos = { posX, posY };
		int entityState = data.child(nameToPrint).attribute("state").as_int();

		entity->data->LoadInfo(entityPos, entityState);

	}
}


bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerScene1 = data.append_child("playerScene1");

	ListItem<Entity*>* item;
	item = entities.start;
	int posX = 600, posY = 700;
	while (item != NULL)
	{

		if (item->data->name == "checkpoint")
		{
			if (item->data->isCurrent == true)
			{
				posX = item->data->position.x;
				posY = item->data->position.y;
			}
	
		}
		item = item->next;
	}

	if (app->win->GetScale() == 1)
	{
		playerScene1.append_attribute("x") = posX;
		playerScene1.append_attribute("y") = posY;
		playerScene1.append_attribute("money") = app->scene->player->moneyPoints;
		playerScene1.append_attribute("hp") = app->scene->player->lifePoints;
	}
	else if(app->win->GetScale() == 2) //compensar el escalado para que la posición de guardar sea precisa
	{
		playerScene1.append_attribute("x") = posX;
		playerScene1.append_attribute("y") = posY;
		playerScene1.append_attribute("money") = app->scene->player->moneyPoints;
		playerScene1.append_attribute("hp") = app->scene->player->lifePoints;
	}

	ListItem<Entity*>* dataEntities;
	dataEntities = entities.start;
	
	while (dataEntities != NULL)
	{
		if (dataEntities->data->destroy != true)
		{
			SaveEntities(data, dataEntities, dataEntities->data->position);
		}
		
		dataEntities = dataEntities->next;
	}

	numSmallEnemy1 = 0;
	numSmallEnemy2 = 0;
	numSmallEnemyFly = 0;

	return true;
}

void EntityManager::SaveEntities(pugi::xml_node& data, ListItem<Entity*>* entity, iPoint pos)
{
	
	if (entity->data->name == "SmallEnemy1")
	{
		numSmallEnemy1++;
		std::string aux = "SmallEnemy1";
		std::string nameEntity = aux + std::to_string(numSmallEnemy1);
		const char* nameToPrint = nameEntity.c_str();

		pugi::xml_node smallEnemy1 = data.append_child(nameToPrint);
		smallEnemy1.append_attribute("x") = pos.x;
		smallEnemy1.append_attribute("y") = pos.y;
		smallEnemy1.append_attribute("state") = entity->data->GetState();


		entity->data->SaveInfo();
		

	}
	else if (entity->data->name == "SmallEnemy2")
	{

		numSmallEnemy2++;
		std::string aux = "SmallEnemy2";
		std::string nameEntity = aux + std::to_string(numSmallEnemy2);
		const char* nameToPrint = nameEntity.c_str();

		pugi::xml_node smallEnemy2 = data.append_child(nameToPrint);
		smallEnemy2.append_attribute("x") = pos.x;
		smallEnemy2.append_attribute("y") = pos.y;
	}
	else if (entity->data->name == "SmallEnemyFly")
	{
		numSmallEnemyFly++;
		std::string aux = "SmallEnemyFly";
		std::string nameEntity = aux + std::to_string(numSmallEnemyFly);
		const char* nameToPrint = nameEntity.c_str();

		pugi::xml_node smallEnemyFly = data.append_child(nameToPrint);
		smallEnemyFly.append_attribute("x") = pos.x;
		smallEnemyFly.append_attribute("y") = pos.y;
		smallEnemyFly.append_attribute("state") = entity->data->GetState();

		entity->data->SaveInfo();
	}
}