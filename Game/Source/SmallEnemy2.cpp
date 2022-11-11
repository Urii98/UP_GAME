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

	return true;
}

bool SmallEnemy2::Start() {
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
	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale())-17;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 15;


	switch (estadoSE2) {
	case STOP:
		break;
	case MOVIMIENTO:
		Movimiento();
		break;
	default:
		break;
	}


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