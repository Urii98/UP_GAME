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
#include <cmath>


SmallEnemy1::SmallEnemy1() : Entity(EntityType::SMALLENEMY1)
{
	name.Create("SmallEnemy1");
}

SmallEnemy1::~SmallEnemy1() {}

bool SmallEnemy1::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	scalarLimites = parameters.attribute("scalarLimites").as_int();
	speedinX = parameters.attribute("speedX").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
	
	walkDir = parameters.attribute("walkDir").as_bool();
	attackE = parameters.attribute("attackE").as_bool();
	animAtk = parameters.attribute("animAtk").as_bool();
	destroy = parameters.attribute("destroy").as_bool();
	map = parameters.attribute("map").as_int();

	deathPath = parameters.attribute("deathpath").as_string();
	deathFxPath = parameters.attribute("deathFxPath").as_string();
	deathFxId = app->audio->LoadFx(deathFxPath);

	return true;
}

bool SmallEnemy1::Start() {

	

	estadoSE1 = SENTRY;
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

	deathL.PushBack({ 817,1365,24,21 });
	deathL.PushBack({ 778,1365,24,21 });
	deathL.loop = true;
	deathL.speed = 0.15f;

	deathR.PushBack({ 903,1365,24,21 });
	deathR.PushBack({ 863,1365,24,21 });
	deathR.loop = true;
	deathR.speed = 0.15f;

	for (int i = 0; i < 6; i++)
	{
		deathEffect.PushBack({ 0 + i * 40, 0, 40, 41 });
	}
	deathEffect.loop = false;
	deathEffect.speed = 0.25f;

	//initilize textures
	texture = app->tex->Load(texturePath);
	deathTexture = app->tex->Load(deathPath);

	// L07 DONE 4: Add a physics  - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 10, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->listener = this;

	pbody->body->SetGravityScale(2.0f);
	pbody->body->ResetMassData();
	
	startPath = true;
	nextFootStep = 0.0f;
	amountToMoveInX = 0.0f;
	initialPosition = { position.x / 64, position.y / 64 };
	range = 5;
	leftBorder = { position.x / 64, (position.y / 64) };
	rightBorder = { initialPosition.x + range, (position.y / 64)};
	firstPath = true;
	achievedRightBorder = false;
	achievedLeftBorder = true;
	destination = 0.0f;
	debug = false;
	playerTileX = 0;
	playerTileY = 0;
	currentAnimationEnemy = &walkRAnimEnemy;
	limitToChaseX = 0;
	limitToChaseY = 0;
	attackAnimation = false;

	changedDataFromSave = false;
	newData.startPath = startPath;
	newData.nextFootStep = nextFootStep;
	newData.amountToMoveInX = amountToMoveInX;
	newData.destination = destination;
	newData.firstPath = firstPath;
	newData.achievedRightBorder = achievedRightBorder;
	newData.achievedLeftBorder = achievedLeftBorder;
	newData.attackAnimation = attackAnimation;
	newData.animation = currentAnimationEnemy;

	framesStopped = 0;
	lastPosinX = 0;
	lastPosinY = 0;

	speedX = 3.0f;
	speedLimit = 8.0f;

	auxBug = false;
	intBug = 0;
	enemyIsDead = false;

	return true;
}

void SmallEnemy1::ReturnMovement2()
{
	int ret = 0;
	b2Vec2 vel = b2Vec2(0, 0);

	if (position.y / 64 != leftBorder.y)
	{
		leftBorder = { position.x / 64, (position.y / 64) };
		rightBorder = { leftBorder.x + range, (position.y / 64) };
	}

	attackAnimation = false;
	iPoint myPos = { position.x / 64, position.y / 64 };
	

	if (changedDataFromSave)
	{
		myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
		changedDataFromSave = false;
	}

	ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");

	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

	if (debug)
	{
		app->pathfinding->DrawLastPath();
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
			if (path->At(1)->x * 64 > position.x)
			{
				vel = b2Vec2(speedX, 2.0f);
			}
			else if (path->At(1)->x * 64 < position.x)
			{
				vel = b2Vec2(-speedX, 2.0f);
			}
		}

		pbody->body->SetLinearVelocity(vel);
		if (path->At(1) != NULL)
			destination = path->At(1)->x * 64;

	}

	if (position.x > destination)
	{
		currentAnimationEnemy = &walkLAnimEnemy;
	}
	else if (position.x < destination)
	{
		currentAnimationEnemy = &walkRAnimEnemy;
	}

}

void SmallEnemy1::ChaseMovement2()
{

	int ret = 0;
	iPoint playerPos = { app->scene->player->position.x / 32, app->scene->player->position.y / 32 };
	iPoint myPos = { (int)std::round(position.x / 64) , (int)std::round(position.y / 64) };

	b2Vec2 vel = b2Vec2(0, 0);

	if (!attackAnimation)
	{
		attackAnimTimer.Start(0.30);
		attackAnimation = true;
	}
		
	if (attackAnimTimer.Test() == EJECUTANDO)
	{
		currentAnimationEnemy = &angryRAnimEnemy;
		return;
	}


	if (changedDataFromSave)
	{
		myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
		changedDataFromSave = false;
	}
	
	ret = app->pathfinding->CreatePath(myPos, playerPos, "terrestre");

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
			}
			else if (path->At(1)->x * 32 < app->scene->player->position.x)
			{
				vel = b2Vec2(speedX, 0);
			}

			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
			//std::cout << "linear velocity - " << pbody->body->GetLinearVelocity().x << std::endl;

			if (pbody->body->GetLinearVelocity().x > speedLimit)
			{
				b2Vec2 vel = pbody->body->GetLinearVelocity();
				vel.x = speedLimit;
				pbody->body->SetLinearVelocity(vel);
			}
			else if (pbody->body->GetLinearVelocity().x < -speedLimit)
			{
				b2Vec2 vel = pbody->body->GetLinearVelocity();
				vel.x = -speedLimit;
				pbody->body->SetLinearVelocity(vel);
			}

			//pbody->body->SetLinearVelocity(vel);

			destination = path->At(1)->x * 64;
			startPath = false;
		}


		if (position.x > destination)
		{
			currentAnimationEnemy = &attackLAnimEnemy;
		}
		else if (position.x < destination)
		{
			currentAnimationEnemy = &attackRAnimEnemy;
		}

	}

	if (lastPosinX == position.x && lastPosinY == position.y)
	{
		framesStopped++;
		
	}

	lastPosinX = position.x;
	lastPosinY = position.y;

	if (framesStopped > 60)
	{
		b2Vec2 vel = b2Vec2(0, -2);
		//pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		pbody->body->ApplyLinearImpulse(vel, pbody->body->GetLocalCenter(), true);
		std::cout << "impulso linear vertical aplicado " << std::endl;
		framesStopped = 0;
	}




}

void SmallEnemy1::SentryMovement2()
{
	int ret = 0;
	b2Vec2 vel = b2Vec2(0, 0);
	iPoint myPos = { 0,0 };

	if (position.y / 64 != leftBorder.y)
	{
		leftBorder = { position.x / 64, (position.y / 64) };
		rightBorder = { leftBorder.x + range, (position.y / 64) };
	}

	if (startPath)
	{
		if (firstPath)
		{
			ret = app->pathfinding->CreatePath(leftBorder, rightBorder, "terrestre");
			firstPath = false;
			if (ret == -1)
			{
				while (ret == -1)
				{
					leftBorder.x = leftBorder.x - 1;
					rightBorder.x = rightBorder.x - 1;
					ret = app->pathfinding->CreatePath(leftBorder, rightBorder, "terrestre");
				}
			}
		}
		else
		{
			if (achievedLeftBorder)
			{
				myPos = { position.x / 64 , position.y / 64 };
				ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
				currentAnimationEnemy = &walkLAnimEnemy;

				if (changedDataFromSave)
				{
					myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
					ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
					changedDataFromSave = false;
				}

				if (ret == -1)
				{
					while (ret == -1)
					{
						rightBorder.x = rightBorder.x - 1;
						ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
					}
				}

			}
			else if (achievedRightBorder)
			{
				myPos = { position.x / 64 , position.y / 64 };
				ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
				currentAnimationEnemy = &walkRAnimEnemy;

				if (changedDataFromSave)
				{
					myPos = { (int)std::round(newData.posX / 64) , newData.posY / 64 };
					ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
					changedDataFromSave = false;
				}

				if (ret == -1)
				{
					while (ret == -1)
					{
						leftBorder.x = leftBorder.x - 1;
						ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
					}
				}

			}
		}
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
			if (path->At(1)->x * 64 > position.x)
			{
				vel = b2Vec2(speedX, 5.0f);
			}
			else if (path->At(1)->x * 64 < position.x)
			{
				vel = b2Vec2(-speedX, 5.0f);
			}
		}

		pbody->body->SetLinearVelocity(vel);
		if (path->At(1) != NULL)
			destination = path->At(1)->x * 64;

	}

	if (pbody->body->GetLinearVelocity().x > speedLimit)
	{
		b2Vec2 vel = pbody->body->GetLinearVelocity();
		vel.x = speedLimit;
		pbody->body->SetLinearVelocity(vel);
	}
	else if (pbody->body->GetLinearVelocity().x < -speedLimit)
	{
		b2Vec2 vel = pbody->body->GetLinearVelocity();
		vel.x = -speedLimit;
		pbody->body->SetLinearVelocity(vel);
	}

	if (position.x > destination)
	{
		currentAnimationEnemy = &walkLAnimEnemy;
	}
	else if (position.x < destination)
	{
		currentAnimationEnemy = &walkRAnimEnemy;
	}

	if (achievedLeftBorder)
	{
		int limitToChange = std::abs(position.x / 64 - rightBorder.x);

		if (limitToChange == 0)
		{
			achievedRightBorder = true;
			achievedLeftBorder = false;
		}
	}
	else if (achievedRightBorder)
	{
		int limitToChange = std::abs(position.x / 64 - leftBorder.x);
		if (limitToChange == 0)
		{
			achievedLeftBorder = true;
			achievedRightBorder = false;
		}
	}

	if (lastPosinX == position.x && lastPosinY == position.y)
	{
		framesStopped++;

	}

	lastPosinX = position.x;
	lastPosinY = position.y;

	if (framesStopped > 60)
	{
		b2Vec2 vel = b2Vec2(0, -2);
		//pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		pbody->body->ApplyLinearImpulse(vel, pbody->body->GetLocalCenter(), true);
		std::cout << "impulso linear vertical aplicado " << std::endl;
		framesStopped = 0;
	}

}

bool SmallEnemy1::Update()
{

	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	switch (estadoSE1) {
	case STOP:
		break;
	case SENTRY:
		SentryMovement2();

		playerTileX = app->scene->player->position.x / 32;
		playerTileY = app->scene->player->position.y / 32;
		limitToChaseX = std::abs(playerTileX - (position.x / 64));
		limitToChaseY = std::abs(playerTileY - (position.y / 64));

		if (limitToChaseX < 5 && limitToChaseY < 4)
		{
			estadoSE1 = CHASE;
		}
		
		
		break;
	case CHASE:
		ChaseMovement2();

		playerTileX = app->scene->player->position.x / 32;
		limitToChaseX = std::abs(playerTileX - (position.x / 64));


		if (limitToChaseX > 4)
		{
			estadoSE1 = RETURN;
			startPath = true;
			firstPath = true;
			achievedRightBorder = false;
			achievedLeftBorder = true;

		}

		break;

	case DEATH:

		if (app->scene->player->position.x * 2 > position.x)
		{
			b2Vec2 vel = b2Vec2(-0.5, -3);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}
		else if (app->scene->player->position.x * 2 < position.x)
		{
			b2Vec2 vel = b2Vec2(0.5, -3);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}

		deathEffectTimer.Start(0.5);

		break;


	case RETURN:
		ReturnMovement2();

		if (position.x / 64 == leftBorder.x)
		{
			estadoSE1 = SENTRY;
			startPath = true;
			firstPath = true;
			achievedRightBorder = false;
			achievedLeftBorder = true;
		}

		playerTileX = app->scene->player->position.x / 32;
		limitToChaseX = std::abs(playerTileX - (position.x / 64));
		limitToChaseY = std::abs(playerTileY - (position.y / 64));

		if (limitToChaseX < 5 && limitToChaseY < 4)
		{
			estadoSE1 = CHASE;
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

	currentAnimationEnemy->Update();
	SDL_Rect rect = currentAnimationEnemy->GetCurrentFrame();

	if (deathEffectTimer.Test() == FIN)
	{
		enemyIsDead = true;
		currentAnimationEnemy = &deathEffect;
		app->audio->PlayFx(deathFxId);

	}

	if (deathTimer.Test() == FIN)
	{
		destroy = true;
	}

	if (enemyIsDead)
	{
		app->render->DrawTexture(deathTexture, position.x / app->win->GetScale() - 10, position.y / app->win->GetScale() - 7, &rect);
	}
	else
	{
		app->render->DrawTexture(texture, position.x / app->win->GetScale() - 10, position.y / app->win->GetScale() - 7, &rect);
	}

	

	if (position.y > 1856 ||
		position.x < 0 ||
		position.x > 15936 ||
		position.y < 0)
	{
		destroy = true;
	}

	if (destroy)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		CleanUp();
		destroy = false;
	}

	return true;
}


bool SmallEnemy1::CleanUp()
{

	app->tex->UnLoad(texture);
	app->tex->UnLoad(deathTexture);
	active = false;

	return true;
}

void SmallEnemy1::OnCollision(PhysBody* physA, PhysBody* physB)
{
	
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		destroy = true;
	
		break;

	case ColliderType::SKILL:
		if (currentAnimationEnemy == &walkLAnimEnemy || currentAnimationEnemy == &attackLAnimEnemy || currentAnimationEnemy == &angryLAnimEnemy)
		{
			currentAnimationEnemy = &deathL;
		}
		else if (currentAnimationEnemy == &walkRAnimEnemy || currentAnimationEnemy == &attackRAnimEnemy || currentAnimationEnemy == &angryRAnimEnemy)
		{
			currentAnimationEnemy = &deathR;
		}
		
		estadoSE1 = DEATH;
		deathTimer.Start(1.0f);
		b2Vec2 vel = b2Vec2(0, 0);
		pbody->body->SetLinearVelocity(vel);
		break;
	}
}

void SmallEnemy1::LoadInfo(iPoint pos, int state)
{

	newData.posX = pos.x;
	newData.posY = pos.y;
	newData.estado = state;
	estadoSE1 = state;
	changedDataFromSave = true;


	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(newData.posX), PIXEL_TO_METERS(newData.posY));
	pbody->body->SetTransform(movePos, 0);

	startPath = newData.startPath;
	nextFootStep = newData.nextFootStep;
	amountToMoveInX = newData.amountToMoveInX;
	destination = newData.destination;
	firstPath = newData.firstPath;
	achievedRightBorder = newData.achievedRightBorder;
	achievedLeftBorder = newData.achievedLeftBorder;
	attackAnimation = newData.attackAnimation;
	
	if (newData.animation != NULL)
	currentAnimationEnemy = newData.animation;

}

void SmallEnemy1::SaveInfo()
{

	newData.startPath = startPath;
	newData.nextFootStep = nextFootStep;
	newData.amountToMoveInX = amountToMoveInX;
	newData.destination = destination;
	newData.firstPath = firstPath;
	newData.achievedRightBorder = achievedRightBorder;
	newData.achievedLeftBorder = achievedLeftBorder;
	newData.attackAnimation = attackAnimation;
	newData.animation = currentAnimationEnemy;
}

int SmallEnemy1::GetState()
{
	return estadoSE1;
}