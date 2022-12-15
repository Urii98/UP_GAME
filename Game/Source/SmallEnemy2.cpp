#include "SmallEnemy2.h"
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


SmallEnemy2::SmallEnemy2() : Entity(EntityType::SMALLENEMY2)
{
	name.Create("SmallEnemy2");
}

SmallEnemy2::~SmallEnemy2() {}

bool SmallEnemy2::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	scalarLimites = parameters.attribute("scalarLimites").as_int();
	speedinX = parameters.attribute("speedX").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
	deathPath = parameters.attribute("deathpath").as_string();

	walkDir = parameters.attribute("walkDir").as_bool();
	destroy = parameters.attribute("destroy").as_bool();
	map = parameters.attribute("map").as_int();

	deathFxPath = parameters.attribute("deathFxPath").as_string();
	deathFxId = app->audio->LoadFx(deathFxPath);

	return true;
}

bool SmallEnemy2::Start() {

	//if (app->sceneTitle->mapSelect == false) {
	//	if (map == 1) {
	//		position.x = 6586;
	//		position.y = 1182;
	//	}
	//	if (map == 2) {
	//		position.x = 7232;
	//		position.y = 1182;
	//	}
	//	if (map == 3) {
	//		position.x = 6920;
	//		position.y = 926;
	//	}
	//	if (map == 4) {
	//		position.x = 6586;
	//		position.y = 670;
	//	}
	//	if (map == 5) {
	//		position.x = 7232;
	//		position.y = 670;
	//	}
	//	if (map == 6) {
	//		position.x = 8890;
	//		position.y = 1694;
	//	}
	//	if (map == 7) {
	//		position.x = 8844;
	//		position.y = 1246;
	//	}
	//	if (map == 8) {
	//		position.x = 8352;
	//		position.y = 1246;
	//	}
	//	if (map == 9) {
	//		position.x = 8544;
	//		position.y = 798;
	//	}
	//	if (map == 10) {
	//		position.x = 8948;
	//		position.y = 798;
	//	}
	//	if (map == 11) {
	//		position.x = 9856;
	//		position.y = 734;
	//	}
	//	if (map == 12) {
	//		position.x = 10368;
	//		position.y = 926;
	//	}
	//	if (map == 13) {
	//		position.x = 10886;
	//		position.y = 1118;
	//	}
	//	if (map == 14) {
	//		position.x = 11382;
	//		position.y = 1118;
	//	}
	//	if (map == 15) {
	//		position.x = 12168;
	//		position.y = 1118;
	//	}
	//	if (map == 16) {
	//		position.x = 12942;
	//		position.y = 1118;
	//	}
	//}

	estadoSE2 = SENTRY;
	direccionSE2 = DERECHA;

	limiteIzqX = position.x - scalarLimites;
	limiteDerX = position.x + scalarLimites;

	walkRAnimEnemy.PushBack({ 764,926,35,31 });
	walkRAnimEnemy.PushBack({ 809,926,35,31 });
	walkRAnimEnemy.PushBack({ 853,926,35,31 });
	walkRAnimEnemy.PushBack({ 896,926,35,31 });
	walkRAnimEnemy.PushBack({ 941,926,35,31 });
	walkRAnimEnemy.PushBack({ 896,926,35,31 });
	walkRAnimEnemy.PushBack({ 853,926,35,31 });
	walkRAnimEnemy.PushBack({ 809,926,35,31 });
	walkRAnimEnemy.loop = true;
	//walkRAnimEnemy.pingpong = true;
	walkRAnimEnemy.speed = 0.125f;
	

	walkLAnimEnemy.PushBack({ 720,926,35,31 });
	walkLAnimEnemy.PushBack({ 675,926,35,31 });
	walkLAnimEnemy.PushBack({ 631,926,35,31 });
	walkLAnimEnemy.PushBack({ 588,926,35,31 });
	walkLAnimEnemy.PushBack({ 543,926,35,31 });
	walkLAnimEnemy.PushBack({ 588,926,35,31 });
	walkLAnimEnemy.PushBack({ 631,926,35,31 });
	walkLAnimEnemy.PushBack({ 675,926,35,31 });
	walkLAnimEnemy.loop = true;
	//walkLAnimEnemy.pingpong = true;
	walkLAnimEnemy.speed = 0.125f;

	deathR.PushBack({ 902,974,35,34 });
	deathR.PushBack({ 853,974,35,34 });
	deathR.loop = true;
	deathR.speed = 0.15f;

	deathL.PushBack({ 581,974,35,34 });
	deathL.PushBack({ 630,974,35,34 });
	deathL.loop = true;
	deathL.speed = 0.15f;

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
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 15, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->listener = this;

	pbody->body->SetGravityScale(2.0f);


	startPath = true;
	nextFootStep = 0.0f;
	amountToMoveInX = 0.0f;
	initialPosition = { position.x / 64, position.y / 64 };
	range = 5;
	leftBorder = { position.x / 64, (position.y / 64)};
	rightBorder = { initialPosition.x + range, (position.y / 64)};
	firstPath = true;
	achievedRightBorder = false;
	achievedLeftBorder = true;
	destination = 0.0f;
	debug = false;
	currentAnimationEnemy = &walkLAnimEnemy;

	changeDataFromSave = false;

	lastPosinX = 0;
	lastPosinY = 0;
	framesStopped = 0;

	speedX = 3.8;
	speedLimit = 7;
	enemyIsDead = false;

	return true;
}

void SmallEnemy2::SentryMovement2()
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

				if (changeDataFromSave)
				{
					myPos = { (int)std::round(posXFromSave / 64) , posYFromSave / 64 };
					ret = app->pathfinding->CreatePath(myPos, rightBorder, "terrestre");
					changeDataFromSave = false;
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

				if (changeDataFromSave)
				{
					myPos = { (int)std::round(posXFromSave / 64) , posYFromSave / 64 };
					ret = app->pathfinding->CreatePath(myPos, leftBorder, "terrestre");
					changeDataFromSave = false;
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
				vel = b2Vec2(speedX, 5.0);
			}
			else if (path->At(1)->x * 64 < position.x)
			{
				vel = b2Vec2(-speedX, 5.0);
			}
		}

		//pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		pbody->body->SetLinearVelocity(vel);	
		if(path->At(1) != NULL)
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
		currentAnimationEnemy = &walkRAnimEnemy;
	}
	else if (position.x < destination)
	{
		currentAnimationEnemy = &walkLAnimEnemy;
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

}

bool SmallEnemy2::Update()
{

	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	switch (estadoSE2) {
	case STOP:
		break;
	case SENTRY:
		SentryMovement2();

		break;
	case DEATH:

		if (app->scene->player->position.x*2 > position.x)
		{
			b2Vec2 vel = b2Vec2(-1, -5);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}
		else if (app->scene->player->position.x*2 < position.x)
		{
			b2Vec2 vel = b2Vec2(1, -5);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}

		deathEffectTimer.Start(0.5);

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
		app->render->DrawTexture(deathTexture, position.x / app->win->GetScale() - 17, position.y / app->win->GetScale() - 15, &rect);
	}
	else
	{
		app->render->DrawTexture(texture, position.x / app->win->GetScale() - 17, position.y / app->win->GetScale() - 15, &rect);
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
	}

	return true;
}

bool SmallEnemy2::CleanUp()
{
	//memoryleak

	app->tex->UnLoad(texture);
	app->tex->UnLoad(deathTexture);
	active = false;

	return true;
}

void SmallEnemy2::OnCollision(PhysBody* physA, PhysBody* physB)
{
	
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		destroy = true;

		break;
	
	case ColliderType::SKILL:
		
		if (currentAnimationEnemy == &walkLAnimEnemy)
		{
			currentAnimationEnemy = &deathL;
		}
		else if (currentAnimationEnemy == &walkRAnimEnemy)
		{
			currentAnimationEnemy = &deathR;
		}

		estadoSE2 = DEATH;
		deathTimer.Start(1.0f);
		b2Vec2 vel = b2Vec2(0, 0);
		pbody->body->SetLinearVelocity(vel);

		//destroy = true;
		break;
		

	}
	
}

void SmallEnemy2::LoadInfo(iPoint pos, int state)
{
	posXFromSave = pos.x;
	posYFromSave = pos.y;
	startPath = true;
	changeDataFromSave = true;

	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(posXFromSave), PIXEL_TO_METERS(posYFromSave));
	pbody->body->SetTransform(movePos, 0);
}