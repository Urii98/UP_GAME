#include "PlayerSensors.h"
#include "App.h"
#include "Scene.h"
#include "Window.h"
#include "Audio.h"
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
	enemyCollisioned = false;
	
	playerSkill.x = 0;
	playerSkill.y = 0;
	skillReset.Start(0.2f);
	skillDraw.Start(0.2f);

	return true;
}

bool PlayerSensors::Update()
{	
	
	b2Vec2 vecPlayerPos = b2Vec2(app->scene->player->pbody->body->GetTransform().p.x, app->scene->player->pbody->body->GetTransform().p.y+0.90);

	jumpSensor->body->SetTransform(vecPlayerPos, 0);

	frames++;

	app->scene->player->drawSwordUI = true;

	if (skillDraw.Test() == EJECUTANDO)
	{
		app->scene->player->drawSwordUI = false;
	}

	if (app->scene->player->sword == true)
	{
		skillReset.Start(1.0f);
		app->scene->player->sword = false;
	}


	if (app->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN && skillReset.Test() == FIN && (app->scene->player->estadoP == app->scene->player->MOVIMIENTO || app->scene->player->estadoP == app->scene->player->STOP))
	{
		skillTimer.Start(1.0f);
		skillReset.Start(3.0f);
		skillDraw.Start(3.0f);
		
		

		if (app->scene->player->skillSwitch == false)
		{
			skill = app->physics->CreateRectangleSensor(0, 0, 38, 70, bodyType::KINEMATIC);
			skill->ctype = ColliderType::SKILL;

			jumpSensor->listener = this;

			if (app->scene->player->direccionP == app->scene->player->DERECHA)
			{
				playerSkill = b2Vec2(app->scene->player->pbody->body->GetTransform().p.x + 1.50, app->scene->player->pbody->body->GetTransform().p.y - 0.56);
			}
			else if (app->scene->player->direccionP == app->scene->player->IZQUIERDA)
			{
				playerSkill = b2Vec2(app->scene->player->pbody->body->GetTransform().p.x - 1.50, app->scene->player->pbody->body->GetTransform().p.y - 0.56);
			}
			
			

			skill->body->SetTransform(playerSkill, 0);
			app->scene->player->skillSwitch = true;
			
			enemyCollisioned = false;
			
			aux = *app->scene->player->currentAnimation;

			if (app->scene->player->direccionP == app->scene->player->DERECHA)
			{
				app->scene->player->currentAnimation = &app->scene->player->swordAttackRAnim;
			}
			else
			{
				app->scene->player->currentAnimation = &app->scene->player->swordAttackLAnim;
			}
			
			app->scene->player->estadoP = app->scene->player->SKILL;
			app->scene->player->posXBeforeAttack = (app->scene->player->position.x * app->win->GetScale()) +28;
			app->scene->player->posYBeforeAttack = (app->scene->player->position.y * app->win->GetScale()) +20;


			app->audio->PlayFx(app->scene->player->swordFxId);
			
		}
		
		
	}
	if (skillTimer.Test() == FIN)
	{
		app->scene->player->skillSwitch = false;

		if (enemyCollisioned == false)
		{
			skill->body->GetWorld()->DestroyBody(skill->body);
		}
		
		app->scene->player->currentAnimation = &aux;
		app->scene->player->estadoP = app->scene->player->MOVIMIENTO;
	}

	

	/*if (aux)
	{
		if (skill->body->IsActive() == true)
		b2Vec2 vecPlayerPos = b2Vec2(app->scene->player->pbody->body->GetTransform().p.x + 1.50, app->scene->player->pbody->body->GetTransform().p.y);

		skill->body->SetTransform(vecPlayerPos, 0);
	}*/
	

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

	if (physA->ctype == ColliderType::SENSOR)
	{


		switch (physB->ctype)
		{
		case ColliderType::PLATFORM:
			if (!resetjumps || frames > 20)
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
	if (physA->ctype == ColliderType::SKILL)
	{
		switch (physB->ctype)
		{
		case ColliderType::ENEMY:
		
			enemyCollisioned = true;
			skill->body->GetWorld()->DestroyBody(skill->body);

			break;

		case ColliderType::ENEMYFLY:
			enemyCollisioned = true;
			skill->body->GetWorld()->DestroyBody(skill->body);

			break;
		}
	}
}