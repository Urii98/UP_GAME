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

#include "Optick/include/optick.h"


SmallEnemyFly::SmallEnemyFly() : Entity(EntityType::SMALLENEMYFLY)
{
	name.Create("SmallEnemyFly");
}

SmallEnemyFly::~SmallEnemyFly() {}

bool SmallEnemyFly::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	scalarLimites = parameters.attribute("scalarLimites").as_int();
	speedY = parameters.attribute("speedY").as_float();
	speedX = parameters.attribute("speedX").as_float();

	texturePath = parameters.attribute("texturepath").as_string();
	textureAngryPath = parameters.attribute("textureangrypath").as_string();

	destroy = parameters.attribute("destroy").as_bool();
	map = parameters.attribute("map").as_int();

	deathPath = parameters.attribute("deathpath").as_string();
	deathFxPath = parameters.attribute("deathFxPath").as_string();
	deathFxId = app->audio->LoadFx(deathFxPath);

	return true;
}

bool SmallEnemyFly::Start() {

	estadoSEF1 = SENTRY;

	limiteSup = position.y - scalarLimites;
	limiteInf = position.y + scalarLimites;


	idleLFlyAnim.PushBack({ 766,513,25,23 });
	idleLFlyAnim.PushBack({ 845,513,25,23 });
	idleLFlyAnim.loop = true;
	idleLFlyAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);

	chaseLFlyAnim.PushBack({ 766,513,25,23 });
	chaseLFlyAnim.PushBack({ 806,513,25,23 });
	chaseLFlyAnim.PushBack({ 845,513,25,23 });
	chaseLFlyAnim.PushBack({ 877,513,25,23 });
	chaseLFlyAnim.loop = true;
	chaseLFlyAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);

	deathLAnimEnemy.PushBack({ 808,550,25,23 });
	deathLAnimEnemy.PushBack({ 770,550,25,23 });
	deathLAnimEnemy.loop = true;
	deathLAnimEnemy.speed = 9.375f * (app->maxFrameDuration / 1000.0);

	
	idleRFlyAnim.PushBack({ 726,513,25,23 });
	idleRFlyAnim.PushBack({ 649,513,25,23 });
	idleRFlyAnim.loop = true;
	idleRFlyAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);
	
	chaseRFlyAnim.PushBack({ 726,513,25,23 });
	chaseRFlyAnim.PushBack({ 686,513,25,23 });
	chaseRFlyAnim.PushBack({ 649,513,25,23 });
	chaseRFlyAnim.PushBack({ 615,513,25,23 });
	chaseRFlyAnim.loop = true;
	chaseRFlyAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);
		
	deathRAnimEnemy.PushBack({ 685,550,25,23 });
	deathRAnimEnemy.PushBack({ 723,550,25,23 });
	deathRAnimEnemy.loop = true;
	deathRAnimEnemy.speed = 9.375f * (app->maxFrameDuration / 1000.0);

	for (int i = 0; i < 6; i++)
	{
		deathEffect.PushBack({ 0 + i * 40, 0, 40, 41 });
	}
	deathEffect.loop = false;
	deathEffect.speed = 15.625 * (app->maxFrameDuration / 1000.0);

	//initilize textures
	texture = app->tex->Load(texturePath);
	angryTexture = app->tex->Load(textureAngryPath);
	deathTexture = app->tex->Load(deathPath);

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
	
	
	changedDataFromSave = false;
	newData.startPath = startPath;
	newData.nextFootStepInX = nextFootStepInX;
	newData.nextFootStepInY = nextFootStepInY;
	newData.amountToMoveInX = amountToMoveInX;
	newData.amountToMoveInY = amountToMoveInY;
	newData.destinationInX = destinationInX;
	newData.destinationInY = destinationInY;
	newData.firstPath = firstPath;
	newData.achievedRightBorder = achievedRightBorder;
	newData.achievedLeftBorder = achievedLeftBorder;
	newData.animation = currentAnimationFlyEnemy;

	enemyIsDead = false;

	return true;
}

void SmallEnemyFly::SentryMovement(float dt)
{
	if (position.y >= limiteInf || !firstSentryMovement)
	{
		b2Vec2 vel = b2Vec2(0, -speedY);
		//vel.y *= dt;
		pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		
	}

	if (position.y <= limiteSup )
	{
		b2Vec2 vel = b2Vec2(0, speedY);
		//vel.y *= dt;
		pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		firstSentryMovement = true;

	}
}

void SmallEnemyFly::ChaseMovement2(float dt)
{
	int ret = 0;
	iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
	iPoint myPos = { (int)std::round(position.x / 64) , (int)std::round(position.y / 64) };

	b2Vec2 vel = b2Vec2(0, 0);

	if (firstPath)
	{
		iPoint myFirstPos = { position.x / 64 , position.y / 64 };
		ret = app->pathfinding->CreatePath(myFirstPos, playerPos, "aereo");
		firstPath = false;
	}
	else
	{
		ret = app->pathfinding->CreatePath(myPos, playerPos, "aereo");
	}

	if (ret != -1)
	{
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();



		if (debug)
		{
			app->pathfinding->DrawLastPath();
		}

		if (path->At(1) != nullptr)
		{

			if (path->At(1)->x * 32 > app->scene->player->position.x)
			{
				vel = b2Vec2(-speedX, 0);
				if (path->At(1)->y * 32 > app->scene->player->position.y)
				{
					vel = b2Vec2(-speedX, -speedX);
				}
				else if (path->At(1)->y * 32 < app->scene->player->position.y)
				{
					vel = b2Vec2(-speedX, speedX);
				}

			}
			else if (path->At(1)->x * 32 < app->scene->player->position.x)
			{
				vel = b2Vec2(speedX, 0);
				if (path->At(1)->y * 32 > app->scene->player->position.y)
				{
					vel = b2Vec2(speedX, -speedX);
				}
				else if (path->At(1)->y * 32 < app->scene->player->position.y)
				{
					vel = b2Vec2(speedX, speedX);
				}
			}

			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
			 
			if (pbody->body->GetLinearVelocity().x > 13)
			{
				b2Vec2 vel = pbody->body->GetLinearVelocity();
				vel.x = 13;
				pbody->body->SetLinearVelocity(vel);
			}
			else if (pbody->body->GetLinearVelocity().x < -13)
			{
				b2Vec2 vel = pbody->body->GetLinearVelocity();
				vel.x = -13;
				pbody->body->SetLinearVelocity(vel);
			}
		
			//pbody->body->SetLinearVelocity(vel);

			destinationInX = path->At(1)->x * 64;
			destinationInY = path->At(1)->y * 64;
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

	}

}

void SmallEnemyFly::ReturnMovement()
{
	iPoint myPos = { (int)std::round(nextFootStepInX / 64) , (int)std::round(nextFootStepInY / 64) };

	if (startPath)
	{
		if (firstPath)
		{
			iPoint myPos = { position.x / 64, position.y / 64 };
			app->pathfinding->CreatePath(myPos, leftBorder, "aereo");
			firstPath = false;
		}
		else
		{
			if (changedDataFromSave)
			{
				myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
				changedDataFromSave = false;
			}
			app->pathfinding->CreatePath(myPos, leftBorder, "aereo");
		}
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
		if (position.x == destinationInX)
		{
			estadoSEF1 = SENTRY;
			startPath = true;
			firstPath = true;
			achievedRightBorder = false;
			achievedLeftBorder = true;
			firstSentryMovement = false;
			return;
		}

		nextFootStepInX = custom_lerp(position.x, destinationInX, 0.07f);
		nextFootStepInY = custom_lerp(position.y, destinationInY, 0.07f);

		amountToMoveInX = nextFootStepInX - auxX;
		amountToMoveInY = nextFootStepInY - auxY;
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

bool SmallEnemyFly::Update(float dt)
{
	OPTICK_EVENT();
	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);


	switch (estadoSEF1) {
	case STOP:
		break;
	case SENTRY:
		SentryMovement(dt); 

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
		ChaseMovement2(dt);

		playerTileX = app->scene->player->position.x / 32;
		limitToChase = std::abs(playerTileX - (position.x / 64));

		break;

	case DEATH:

		if (app->scene->player->position.x * 2 > position.x)
		{
			b2Vec2 vel = b2Vec2(-0.5, -1.5);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}
		else if (app->scene->player->position.x * 2 < position.x)
		{
			b2Vec2 vel = b2Vec2(0.5, -1.5);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}

		deathEffectTimer.Start(0.5);

		break;

	case RETURN:

		ReturnMovement();

		if (position.x / 64 == leftBorder.x)
		{

			b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y));
			pbody->body->SetTransform(movePos, 0);

			estadoSEF1 = SENTRY;
			startPath = true;
			firstPath = true;
			achievedRightBorder = false;
			achievedLeftBorder = true;
			firstSentryMovement = false;

			currentAnimationFlyEnemy = &idleRFlyAnim;

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

	if (deathEffectTimer.Test() == FIN)
	{
		enemyIsDead = true;
		currentAnimationFlyEnemy = &deathEffect;
		app->audio->PlayFx(deathFxId);

	}

	if (deathTimer.Test() == FIN)
	{
		destroy = true;
	}

	if (estadoSEF1 == CHASE)
	{
		app->render->DrawTexture(angryTexture, position.x / app->win->GetScale() - 10, position.y / app->win->GetScale() - 15, &rect);
	} 
	else if (enemyIsDead)
	{
		app->render->DrawTexture(deathTexture, position.x / app->win->GetScale() - 18, position.y / app->win->GetScale() - 22, &rect);
	}
	else
	{
		app->render->DrawTexture(texture, position.x / app->win->GetScale() - 10, position.y / app->win->GetScale() - 15, &rect);
	}
	

	if (position.y > 1856 ||
		position.x < 0 ||
		position.x > 15936 ||
		position.y < 0)
	{
		destroy = true;
	}

	if (deathAnimationTimer.Test() == FIN || destroy)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
//		app->tex->UnLoad(texture);
		active = false;
		CleanUp();
		destroy = true;
	}

	return true;
}

bool SmallEnemyFly::CleanUp()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(angryTexture);
	app->tex->UnLoad(deathTexture);

	return true;
}

void SmallEnemyFly::OnCollision(PhysBody* physA, PhysBody* physB)
{
	b2Vec2 vel = b2Vec2(0, 0);
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		destroy = true;

		break;

	case ColliderType::SKILL:
		if (currentAnimationFlyEnemy == &idleLFlyAnim || currentAnimationFlyEnemy == &chaseLFlyAnim)
		{
			currentAnimationFlyEnemy = &deathLAnimEnemy;
		}
		else if (currentAnimationFlyEnemy == &idleRFlyAnim || currentAnimationFlyEnemy == &chaseRFlyAnim)
		{
			currentAnimationFlyEnemy = &deathRAnimEnemy;
		}

		estadoSEF1 = DEATH;
		deathTimer.Start(1.0f);
		pbody->body->SetLinearVelocity(vel);
		break;

	case ColliderType::DEATH:
		if (currentAnimationFlyEnemy == &idleLFlyAnim || currentAnimationFlyEnemy == &chaseLFlyAnim)
		{
			currentAnimationFlyEnemy = &deathLAnimEnemy;
		}
		else if (currentAnimationFlyEnemy == &idleRFlyAnim || currentAnimationFlyEnemy == &chaseRFlyAnim)
		{
			currentAnimationFlyEnemy = &deathRAnimEnemy;
		}

		estadoSEF1 = DEATH;
		deathTimer.Start(1.0f);
		pbody->body->SetLinearVelocity(vel);
		break;
	}
}

void SmallEnemyFly::LoadInfo(iPoint pos, int state)
{

	newData.posX = pos.x;
	newData.posY = pos.y;
	newData.estado = state;
	estadoSEF1 = state;
	changedDataFromSave = true;

	if (state == 1)
	{
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		pbody->body->SetAngularVelocity(0);
	}
	
	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(newData.posX), PIXEL_TO_METERS(newData.posY));
	pbody->body->SetTransform(movePos, 0);
	

	startPath = newData.startPath;
	nextFootStepInX = newData.nextFootStepInX;
	nextFootStepInY = newData.nextFootStepInY;
	amountToMoveInX = newData.amountToMoveInX;
	amountToMoveInY = newData.amountToMoveInY;
	destinationInX = newData.destinationInX;
	destinationInY = newData.destinationInY;
	firstPath = newData.firstPath;
	achievedRightBorder = newData.achievedRightBorder;
	achievedLeftBorder = newData.achievedLeftBorder;
	if(newData.animation != NULL)
	currentAnimationFlyEnemy = newData.animation;

}

void SmallEnemyFly::SaveInfo()
{

	newData.startPath = startPath;
	newData.nextFootStepInX = nextFootStepInX;
	newData.nextFootStepInY = nextFootStepInY;
	newData.amountToMoveInX = amountToMoveInX;
	newData.amountToMoveInY = amountToMoveInY;
	newData.destinationInX = destinationInX;
	newData.destinationInY = destinationInY;
	newData.firstPath = firstPath;
	newData.achievedRightBorder = achievedRightBorder;
	newData.achievedLeftBorder = achievedLeftBorder;
	newData.animation = currentAnimationFlyEnemy;
}

int SmallEnemyFly::GetState()
{
	return estadoSEF1;
}