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


SmallEnemy2::SmallEnemy2() : Entity(EntityType::SMALLENEMY2)
{
	name.Create("SmallEnemy2");
}

SmallEnemy2::~SmallEnemy2() {}

bool SmallEnemy2::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	scalarLimites = parameters.attribute("scalarLimites").as_int();
	speedX = parameters.attribute("speedX").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	walkDir = parameters.attribute("walkDir").as_bool();
	destroy = parameters.attribute("destroy").as_bool();
	map = parameters.attribute("map").as_int();

	return true;
}

bool SmallEnemy2::Start() {

	if (app->sceneTitle->mapSelect == false) {
		if (map == 1) {
			position.x = 6586;
			position.y = 1182;
		}
		if (map == 2) {
			position.x = 7232;
			position.y = 1182;
		}
		if (map == 3) {
			position.x = 6920;
			position.y = 926;
		}
		if (map == 4) {
			position.x = 6586;
			position.y = 670;
		}
		if (map == 5) {
			position.x = 7232;
			position.y = 670;
		}
		if (map == 6) {
			position.x = 8890;
			position.y = 1694;
		}
		if (map == 7) {
			position.x = 8844;
			position.y = 1246;
		}
		if (map == 8) {
			position.x = 8352;
			position.y = 1246;
		}
		if (map == 9) {
			position.x = 8544;
			position.y = 798;
		}
		if (map == 10) {
			position.x = 8948;
			position.y = 798;
		}
		if (map == 11) {
			position.x = 9856;
			position.y = 734;
		}
		if (map == 12) {
			position.x = 10368;
			position.y = 926;
		}
		if (map == 13) {
			position.x = 10886;
			position.y = 1118;
		}
		if (map == 14) {
			position.x = 11382;
			position.y = 1118;
		}
		if (map == 15) {
			position.x = 12168;
			position.y = 1118;
		}
		if (map == 16) {
			position.x = 12942;
			position.y = 1118;
		}
	}

	estadoSE2 = MOVIMIENTO;
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

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics  - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 15, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->listener = this;

	return true;
}

void SmallEnemy2::Movimiento()
{
	if (position.x <= limiteDerX && !walkDir)
	{
		b2Vec2 vel = b2Vec2(speedX, -GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
		direccionSE2 = DERECHA;
		currentAnimationEnemy = &walkLAnimEnemy;
	}
	else
	{
		walkDir = true;
	}

	if (position.x >= limiteIzqX && walkDir)
	{
		b2Vec2 vel = b2Vec2(-speedX, -GRAVITY_Y);
		pbody->body->SetLinearVelocity(vel);
		direccionSE2 = IZQUIERDA;
		currentAnimationEnemy = &walkRAnimEnemy;
	}
	else
	{
		walkDir = false;
	}
}
bool SmallEnemy2::Update()
{
	switch (estadoSE2) {
	case STOP:
		break;
	case MOVIMIENTO:
		Movimiento();
		break;
	default:
		break;
	}

	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale()) - 17;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 15;

	currentAnimationEnemy->Update();
	SDL_Rect rect = currentAnimationEnemy->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	if (destroy)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		CleanUp();
		destroy = false;
	}

	return true;
}

bool SmallEnemy2::CleanUp()
{
	//memoryleak

	app->tex->UnLoad(texture);
	active = false;

	//if (!destroy)
	//{
	//	app->physics->world->DestroyBody(pbody->body);
	//	//pbody->body->GetWorld()->DestroyBody(pbody->body);
	//}

	return true;
}

void SmallEnemy2::OnCollision(PhysBody* physA, PhysBody* physB)
{

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		destroy = true;

		break;
	}
}