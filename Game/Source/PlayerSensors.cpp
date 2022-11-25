#include "PlayerSensors.h"
#include "App.h"
#include "Scene.h"
#include "Window.h"
#include <iostream>

PlayerSensors::PlayerSensors() : Entity(EntityType::SENSORS)
{
	name.Create("PlayerSensors");
}
PlayerSensors::~PlayerSensors()
{

}

bool PlayerSensors::Awake()
{
	return true;
}

bool PlayerSensors::Start()
{
	//jumpSensor = app->physics->CreateRectangleSensor(app->scene->player->position.x, app->scene->player->position.x, 15, 5, bodyType::DYNAMIC);
	//jumpSensor = app->physics->CreateRectangle(app->scene->player->position.x, app->scene->player->position.x, 15, 5, bodyType::DYNAMIC);
	jumpSensor = app->physics->CreateRectangle(2.8f, 6.4f, 7, 7, bodyType::DYNAMIC);
	jumpSensor->ctype = ColliderType::SENSOR;
	return true;
}

bool PlayerSensors::Update()
{
	iPoint  playerPos;
	playerPos.x = METERS_TO_PIXELS(app->scene->player->pbody->body->GetTransform().p.x / app->win->GetScale()) - 14;
	playerPos.y = METERS_TO_PIXELS(app->scene->player->pbody->body->GetTransform().p.y / app->win->GetScale()) - 10;
	
	b2Vec2 vecPlayerPos = b2Vec2(playerPos.x, playerPos.y);

	b2Vec2 hardCodePos = b2Vec2(2.8f, 6.4f);


	jumpSensor->body->SetTransform(hardCodePos, 0);

	std::cout << "SENSOR  -  X :" << playerPos.x << std::endl;
	std::cout << "SENSOR  -  Y :" << playerPos.y << std::endl;
	

	return true;

}

void PlayerSensors::PostUpdate()
{

}

bool PlayerSensors::CleanUp()
{
	return true;
}

void PlayerSensors::OnCollision(PhysBody* physA, PhysBody* physB)
{

}