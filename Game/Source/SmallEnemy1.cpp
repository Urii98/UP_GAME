#include "SmallEnemy1.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


SmallEnemy1::SmallEnemy1() : Entity(EntityType::SMALLENEMY1)
{
	name.Create("SmallEnemy1");
}

SmallEnemy1::~SmallEnemy1() {}

bool SmallEnemy1::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	limiteIzqX = position.x - 30;
	limiteDerX = position.x + 30;

	return true;
}

bool SmallEnemy1::Start() {

	estadoSE1 = MOVIMIENTO;
	direccionE = DERECHA;
	walkDir = false;
	attackE = false;
	animAtk = false;
	speedX = 2;


	walkRAnimEnemy.PushBack({ 718,1297,24,21 });
	walkRAnimEnemy.PushBack({ 676,1297,24,21 });
	walkRAnimEnemy.PushBack({ 630,1297,24,21 });
	walkRAnimEnemy.loop = true;
	walkRAnimEnemy.speed = 0.125f;

	
	walkLAnimEnemy.PushBack({ 776,1297,24,21 });
	walkLAnimEnemy.PushBack({ 818,1297,24,21 });
	walkLAnimEnemy.PushBack({ 864,1297,24,21 });
	walkRAnimEnemy.loop = true;
	walkLAnimEnemy.speed = 0.125f;

	angryRAnimEnemy.PushBack({ 777,1332,24,21 });
	angryRAnimEnemy.PushBack({ 817,1332,24,21 });
	angryRAnimEnemy.loop = false;
	angryRAnimEnemy.speed = 0.05f;

	angryLAnimEnemy.PushBack({ 717,1332,24,21 });
	angryLAnimEnemy.PushBack({ 676,1332,24,21 });
	angryLAnimEnemy.loop = false;
	angryLAnimEnemy.speed = 0.05f;

	attackRAnimEnemy.PushBack({ 863,1327,24,26 });
	attackRAnimEnemy.PushBack({ 905,1327,24,26 });
	attackRAnimEnemy.loop = true;
	attackRAnimEnemy.speed = 0.2f;

	attackLAnimEnemy.PushBack({630,1327,24,26});
	attackLAnimEnemy.PushBack({588,1327,24,26});
	attackLAnimEnemy.loop = true;
	attackLAnimEnemy.speed = 0.2f;


	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics  - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 10, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

void SmallEnemy1::Movimiento()
{
	if (position.x <= limiteDerX && !walkDir)
	{
		b2Vec2 vel = b2Vec2(speedX, -GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
		direccionE = DERECHA;
		currentAnimationEnemy = &walkRAnimEnemy;
	}
	else
	{
		walkDir = true;
	}

	if (position.x >= limiteIzqX && walkDir)
	{
		b2Vec2 vel = b2Vec2(-speedX, -GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
		direccionE = IZQUIERDA;
		currentAnimationEnemy = &walkLAnimEnemy;
	}
	else
	{
		walkDir = false;
	}
}

void SmallEnemy1::Ataque() {

	//animacion angry
	if (!attackE)
	{
		if (direccionE == DERECHA)
		{
			currentAnimationEnemy = &angryRAnimEnemy;
		}
		else if (direccionE == IZQUIERDA)
		{
			currentAnimationEnemy = &angryLAnimEnemy;
			
		}
		attackE = true;
		attackAnimTimer.Start(1.5); //timer para cambiar a la animacion de atacar
		b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
	}
	

	if (attackAnimTimer.Test() == FIN && !animAtk) //termina la pausa para la animacion de angry y se procede a atacar
	{
		animAtk = true;
	}


	if (attackE && animAtk) //perseguir y animacion atk en loop
	{
		if (app->scene->player->position.x < position.x)
		{
			b2Vec2 vel = b2Vec2(-speedX, -GRAVITY_Y);
			pbody->body->SetLinearVelocity(vel);
			currentAnimationEnemy = &attackLAnimEnemy;
		}
		else if (app->scene->player->position.x > position.x)
		{
			b2Vec2 vel = b2Vec2(speedX, -GRAVITY_Y);
			pbody->body->SetLinearVelocity(vel);
			currentAnimationEnemy = &attackRAnimEnemy;
		}
	}

	//animacion ataque
}

bool SmallEnemy1::Update()
{
	switch (estadoSE1) {
	case STOP:
		break;
	case MOVIMIENTO:
		Movimiento();
		if (app->scene->player->position.x <limiteDerX && app->scene->player->position.x > limiteIzqX-50 &&
			app->scene->player->position.y > position.y -20 && app->scene->player->position.y < position.y+20)
		{
			estadoSE1 = ATAQUE;
		}
		
		break;
	case ATAQUE:
		Ataque();
		break;
	default:
		break;
	}

	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 10;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 7;


	currentAnimationEnemy->Update();
	SDL_Rect rect = currentAnimationEnemy->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);
	

	return true;
}

bool SmallEnemy1::CleanUp()
{
	return true;
}