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
		ret = item->data->Update();
	}


	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		this->active = false;
	}

	return ret;
}


bool EntityManager::LoadState(pugi::xml_node& data)
{
	auto posX = data.child("playerScene1").attribute("x").as_int();
	auto posY = data.child("playerScene1").attribute("y").as_int();

	app->scene->player->ChangePosition(posX, posY);

	return true;
}


bool EntityManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerScene1 = data.append_child("playerScene1");

	if (app->win->GetScale() == 1)
	{
		playerScene1.append_attribute("x") = app->scene->player->position.x+15;
		playerScene1.append_attribute("y") = app->scene->player->position.y+5;
	}
	else if(app->win->GetScale() == 2) //compensar el escalado para que la posición de guardar sea precisa
	{
		playerScene1.append_attribute("x") = app->scene->player->position.x * 2 + 28;
		playerScene1.append_attribute("y") = app->scene->player->position.y * 2 + 5;
	}



	return true;
}
