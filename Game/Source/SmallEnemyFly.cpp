#include "SmallEnemyFly.h"
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
#include <cmath>


SmallEnemyFly::SmallEnemyFly() : Entity(EntityType::SMALLENEMYFLY)
{
	name.Create("SmallEnemyFly");
}

SmallEnemyFly::~SmallEnemyFly() {}

bool SmallEnemyFly::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	scalarLimites = parameters.attribute("scalarLimites").as_int();
	speedY = parameters.attribute("speedY").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	destroy = parameters.attribute("destroy").as_bool();
	map = parameters.attribute("map").as_int();

	return true;
}

bool SmallEnemyFly::Start() {

	estadoSEF1 = SENTRY;

	limiteSup = position.y - scalarLimites;
	limiteInf = position.y + scalarLimites;


	idleLFlyAnim.PushBack({ 766,513,25,23 });
	idleLFlyAnim.PushBack({ 845,513,25,23 });
	idleLFlyAnim.loop = true;
	idleLFlyAnim.speed = 0.125f;

	chaseLFlyAnim.PushBack({ 766,513,25,23 });
	chaseLFlyAnim.PushBack({ 806,513,25,23 });
	chaseLFlyAnim.PushBack({ 845,513,25,23 });
	chaseLFlyAnim.PushBack({ 877,513,25,23 });
	chaseLFlyAnim.loop = true;
	chaseLFlyAnim.speed = 0.125f;

	deathLAnimEnemy.PushBack({ 808,550,25,23 });
	deathLAnimEnemy.PushBack({ 770,550,25,23 });
	deathLAnimEnemy.loop = false;
	deathLAnimEnemy.speed = 0.095f;

	
	idleRFlyAnim.PushBack({ 726,513,25,23 });
	idleRFlyAnim.PushBack({ 649,513,25,23 });
	idleRFlyAnim.loop = true;
	idleRFlyAnim.speed = 0.125f;
	
	chaseRFlyAnim.PushBack({ 726,513,25,23 });
	chaseRFlyAnim.PushBack({ 686,513,25,23 });
	chaseRFlyAnim.PushBack({ 649,513,25,23 });
	chaseRFlyAnim.PushBack({ 615,513,25,23 });
	chaseRFlyAnim.loop = true;
	chaseRFlyAnim.speed = 0.125f;
		
	deathRAnimEnemy.PushBack({ 685,550,25,23 });
	deathRAnimEnemy.PushBack({ 723,550,25,23 });
	deathRAnimEnemy.loop = false;
	deathRAnimEnemy.speed = 0.095f;

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics  - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 9, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMYFLY;

	pbody->listener = this;

	pbody->body->SetGravityScale(0.0);

	firstSentryMovement = false;

	startPath = true;
	nextFootStepInX = 0.0f;
	nextFootStepInY = 0.0f;
	amountToMoveInX = 0.0f;
	amountToMoveInY = 0.0f;
	initialPosition = { position.x / 64, position.y / 64 };
	range = 5;
	leftBorder = { position.x / 64, (position.y / 64) + 1 };
	rightBorder = { initialPosition.x + range, (position.y / 64) + 1 };
	firstPath = true;
	achievedRightBorder = false;
	achievedLeftBorder = true;
	destinationInX = 0.0f;
	destinationInY = 0.0f;
	debug = false;
	playerTileX = 0;
	playerTileY = 0;
	currentAnimationFlyEnemy = &idleLFlyAnim;
	limitToChase = 0;

	return true;
}

void SmallEnemyFly::SentryMovement()
{
	if (position.y >= limiteInf || !firstSentryMovement)
	{
		b2Vec2 vel = b2Vec2(0, -speedY);
		pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		
	}

	if (position.y <= limiteSup )
	{
		b2Vec2 vel = b2Vec2(0, speedY);
		pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		firstSentryMovement = true;

	}
}

void SmallEnemyFly::chaseprueba() 
{

	iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
	iPoint myPos = { (int)std::round(nextFootStepInX / 64) , (int)std::round(nextFootStepInY / 64) };

	if (firstPath)
	{
		iPoint myFirstPos = { position.x/64 , position.y / 64 };
		app->pathfinding->CreatePath(myFirstPos, playerPos);
		firstPath = false;
	}
	else
	{
		app->pathfinding->CreatePath(myPos, playerPos);
	}

	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

	if (debug)
	{
		app->pathfinding->DrawLastPath();
	}

	if (path->At(1) != nullptr)
	{
		int auxX = position.x;
		int auxY = position.y;
		destinationInX = path->At(1)->x * 64;
		destinationInY = path->At(1)->y * 64;

		nextFootStepInX = custom_lerp(position.x, destinationInX, 0.055f);
		nextFootStepInY = custom_lerp(position.y, destinationInY, 0.055f);

		amountToMoveInX = nextFootStepInX - auxX;
		amountToMoveInY = nextFootStepInY - auxY;
		startPath = false;
	}

	if (position.x > destinationInX)
	{
		currentAnimationFlyEnemy = &chaseLFlyAnim;
	}
	else if (position.x < destinationInX)
	{
		currentAnimationFlyEnemy = &chaseRFlyAnim;
	}

	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(nextFootStepInX), PIXEL_TO_METERS(nextFootStepInY));
	pbody->body->SetTransform(movePos, 0);

	nextFootStepInX += amountToMoveInX;
	nextFootStepInY += amountToMoveInY;

}

void SmallEnemyFly::ReturnMovement()
{

}

bool SmallEnemyFly::Update()
{
	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);


	switch (estadoSEF1) {
	case STOP:
		break;
	case SENTRY:
		SentryMovement();

		playerTileX = app->scene->player->position.x / 32;
		playerTileY = app->scene->player->position.y / 32;
		if (playerTileX > leftBorder.x-range/2 && playerTileX < rightBorder.x) //como el leftborder empieza en la posicion inicial, para que el rango en X sea igual tanto de izq como de derecha le resto el rango/2 a la primera parte
		{
			if (playerTileY > (position.y / 64) - 2 && playerTileY < (position.y / 64) + 2)
			{
				estadoSEF1 = CHASE;
			}
		}
			

		break;

	case CHASE:

		chaseprueba();

		/*playerTileX = app->scene->player->position.x / 32;
		limitToChase = std::abs(playerTileX - (position.x / 64));

		if (limitToChase > 5)
		{
			estadoSEF1 = RETURN;
			startPath = true;
			firstPath = true;
			achievedRightBorder = false;
			achievedLeftBorder = true;

		}*/

		break;

	case RETURN:

		ReturnMovement();

		if (position.x / 64 == leftBorder.x)
		{
			estadoSEF1 = SENTRY;
			startPath = true;
			firstPath = true;
			achievedRightBorder = false;
			achievedLeftBorder = true;
		}

		break;

	default:
		break;
	}

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		if (!debug)
		{
			debug = true;
		}
		else
		{
			debug = false;
		}
	}

	currentAnimationFlyEnemy->Update();
	SDL_Rect rect = currentAnimationFlyEnemy->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x / app->win->GetScale() - 10, position.y / app->win->GetScale() - 15, &rect);

	if (deathAnimationTimer.Test() == FIN)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		app->tex->UnLoad(texture);
		active = false;
//		CleanUp();
		destroy = true;
	}

	return true;
}

bool SmallEnemyFly::CleanUp()
{
	//memoryleak

//	app->tex->UnLoad(texture);
	
	
	//pbody->body->GetWorld()->DestroyBody(pbody->body);


	//if (!destroy)
	//{
	//	app->physics->world->DestroyBody(pbody->body);
	//	//pbody->body->GetWorld()->DestroyBody(pbody->body);
	//}

	

	return true;
}

void SmallEnemyFly::OnCollision(PhysBody* physA, PhysBody* physB)
{

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		//destroy = true;

		if (currentAnimationFlyEnemy == &idleLFlyAnim || currentAnimationFlyEnemy == &chaseLFlyAnim)
		{
			deathAnimationTimer.Start(0.20f);
			currentAnimationFlyEnemy = &deathLAnimEnemy;
		}

		if (currentAnimationFlyEnemy == &idleRFlyAnim || currentAnimationFlyEnemy == &chaseRFlyAnim)
		{
			deathAnimationTimer.Start(0.20f);
			currentAnimationFlyEnemy = &deathRAnimEnemy;
		}
		
		estadoSEF1 = STOP;

		break;
	}
}