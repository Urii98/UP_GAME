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


#include "Window.h"


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

	return true;
}

bool SmallEnemyFly::Start() {
	estadoSEF1 = MOVIMIENTO;
	
	limiteSup = position.y - scalarLimites;
	limiteInf = position.y + scalarLimites;

	flyAnimEnemy.PushBack({ 766,513,25,23 });
	flyAnimEnemy.PushBack({ 806,513,25,23 });
	flyAnimEnemy.PushBack({ 845,513,25,23 });
	flyAnimEnemy.PushBack({ 877,513,25,23 });
	flyAnimEnemy.loop = true;
	flyAnimEnemy.speed = 0.125f;

	deathAnimEnemy.PushBack({808,550,25,23});
	deathAnimEnemy.PushBack({ 770,550,25,23 });
	deathAnimEnemy.loop = false;
	deathAnimEnemy.speed = 0.095f;
	
	
	currentAnimationFlyEnemy = &flyAnimEnemy;

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics  - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 9, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMYFLY;

	pbody->listener = this;

	return true;
}

void SmallEnemyFly::Movimiento()
{
	if (position.y == limiteInf)
	{
		b2Vec2 vel = b2Vec2(0, speedY);
		pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		
	}
	
	if(position.y>= limiteSup)
	{
		b2Vec2 vel = b2Vec2(0, -speedY);
		pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		
	}
}
bool SmallEnemyFly::Update()
{
	// L07 DONE 4: Add a physics  - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale()) - 10;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 15;

	
	switch (estadoSEF1) {
	case STOP:
		break;
	case MOVIMIENTO:
		Movimiento();
		break;
	default:
		break;
	}

	currentAnimationFlyEnemy->Update();
	SDL_Rect rect = currentAnimationFlyEnemy->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	if (deathAnimationTimer.Test() == FIN)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		app->tex->UnLoad(texture);
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
		deathAnimationTimer.Start(0.20f);
		currentAnimationFlyEnemy = &deathAnimEnemy;
		break;
	}
}