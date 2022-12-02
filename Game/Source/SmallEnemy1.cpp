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
#include "EntityManager.h" 
#include "SceneTitle.h"

#include "Window.h"

#include "Map.h"
#include "Pathfinding.h"
#include "Defs.h"


SmallEnemy1::SmallEnemy1() : Entity(EntityType::SMALLENEMY1)
{
	name.Create("SmallEnemy1");
}

SmallEnemy1::~SmallEnemy1() {}

bool SmallEnemy1::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	scalarLimites = parameters.attribute("scalarLimites").as_int();
	speedX = parameters.attribute("speedX").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
	
	walkDir = parameters.attribute("walkDir").as_bool();
	attackE = parameters.attribute("attackE").as_bool();
	animAtk = parameters.attribute("animAtk").as_bool();
	destroy = parameters.attribute("destroy").as_bool();
	map = parameters.attribute("map").as_int();

	return true;
}

bool SmallEnemy1::Start() {

	

	estadoSE1 = MOVIMIENTO;
	direccionE = DERECHA;

	limiteIzqX = position.x - scalarLimites;
	limiteDerX = position.x + scalarLimites;

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

	angryRAnimEnemy.PushBack({ 940,1331,24,21 });
	angryRAnimEnemy.PushBack({ 777,1332,24,21 });
	angryRAnimEnemy.PushBack({ 817,1332,24,21 });
	angryRAnimEnemy.loop = false;
	angryRAnimEnemy.speed = 0.05f;

	angryLAnimEnemy.PushBack({ 940,1331,24,21 });
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

	pbody->listener = this;

	return true;
}

void SmallEnemy1::Movimiento()
{
	/*if (position.x <= limiteDerX && !walkDir)
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
	}*/
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

void SmallEnemy1::desesperacion()
{

	std::cout << position.x << " - " << position.y << std::endl;

	currentAnimationEnemy = &walkRAnimEnemy;

	iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
	iPoint myPos = { position.x / 32 , position.y / 32 };
	iPoint aux = { myPos.x + 3, myPos.y };

	app->pathfinding->CreatePath(myPos, aux);
//	app->pathfinding->CreatePath(myPos, playerPos);

	b2Vec2 vel = b2Vec2(speedX, -GRAVITY_Y);
	pbody->body->SetLinearVelocity(vel);


	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{

		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y);
	}
}

bool SmallEnemy1::Update()
{

	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	/*position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale()) - 10;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 7;*/

	//desesperacion();
	currentAnimationEnemy = &walkRAnimEnemy;

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		position.x++;

		b2Vec2 resetPos = b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));
		pbody->body->SetTransform(resetPos, 0);

	}

	//while(position == path[int i])

	switch (estadoSE1) {
	case STOP:
		break;
	case MOVIMIENTO:
		Movimiento();
	/*	if (app->scene->player->position.x <limiteDerX && 
			app->scene->player->position.x > limiteIzqX-50 &&
			app->scene->player->position.y > position.y -20 && 
			app->scene->player->position.y < position.y+20)
		{
			estadoSE1 = ATAQUE;
		}*/
		
		break;
	case ATAQUE:
		Ataque();
		break;
	default:
		break;
	}



	currentAnimationEnemy->Update();
	SDL_Rect rect = currentAnimationEnemy->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x / app->win->GetScale() - 10, position.y / app->win->GetScale() - 7, &rect);
	

	if (destroy)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		CleanUp();
		destroy = false;
	}

	//ChasePathFinding();

	return true;
}

void SmallEnemy1::ChasePathFinding()
{

	iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
	iPoint myPos = { position.x / 32 , position.y / 32 };

	app->pathfinding->CreatePath(myPos, playerPos);

	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y);
	}

}

bool SmallEnemy1::CleanUp()
{
	//memoryleak

	app->tex->UnLoad(texture);
	active = false;

	//if(!destroy)
	//{
	//	app->physics->world->DestroyBody(pbody->body);
	//	//pbody->body->GetWorld()->DestroyBody(pbody->body);
	//}

	return true;
}

void SmallEnemy1::OnCollision(PhysBody* physA, PhysBody* physB)
{
	
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		destroy = true;
	
		break;
	}
}