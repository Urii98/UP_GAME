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
	jumpSensor = app->physics->CreateRectangleSensor(0, 0, 12, 10, bodyType::DYNAMIC);
	jumpSensor->ctype = ColliderType::SENSOR;

	jumpSensor->listener = this;

	frames = 0;
	resetjumps = false;
	
	return true;
}

bool PlayerSensors::Update()
{	
	b2Vec2 vecPlayerPos = b2Vec2(app->scene->player->pbody->body->GetTransform().p.x, app->scene->player->pbody->body->GetTransform().p.y+0.90);

	jumpSensor->body->SetTransform(vecPlayerPos, 0);
	
	//std::cout << "SENSOR  -  X :" << jumpSensor->body->GetTransform().p.x << std::endl;
	//std::cout << "SENSOR  -  Y :" << jumpSensor->body->GetTransform().p.y << std::endl;

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

	if (resetjumps)
		frames++;

	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		if (!resetjumps || frames >20)
		{
			app->scene->player->collisionP = app->scene->player->COLLISION;
			app->scene->player->oneJump = false;
			app->scene->player->flying = false;
			app->scene->player->flapLimit = 0;
			app->scene->player->jumpRAnim.Reset();
			app->scene->player->jumpLAnim.Reset();
			frames = 0;
			resetjumps = true;
		}

		break;
	}
}