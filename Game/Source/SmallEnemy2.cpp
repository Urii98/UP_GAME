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
	speedX = parameters.attribute("speedX").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	walkDir = parameters.attribute("walkDir").as_bool();
	destroy = parameters.attribute("destroy").as_bool();
	map = parameters.attribute("map").as_int();

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

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 4: Add a physics  - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 12, position.y + 12, 15, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->listener = this;


	startPath = true;
	nextFootStep = 0.0f;
	amountToMoveInX = 0.0f;
	initialPosition = { position.x / 64, position.y / 64 };
	range = 12;
	leftBorder = { position.x / 64, (position.y / 64)};
	rightBorder = { initialPosition.x + range, (position.y / 64)};
	firstPath = true;
	achievedRightBorder = false;
	achievedLeftBorder = true;
	destination = 0.0f;
	debug = false;
	currentAnimationEnemy = &walkLAnimEnemy;

	return true;
}

void SmallEnemy2::SentryMovement()
{

	if (startPath)
	{
		if (firstPath)
		{
			app->pathfinding->CreatePath(leftBorder, rightBorder);
			firstPath = false;
		}
		else
		{
			if (achievedLeftBorder)
			{
				iPoint myPos = { (int)std::round(nextFootStep / 64) , position.y / 64 };
				app->pathfinding->CreatePath(myPos, rightBorder);
				currentAnimationEnemy = &walkLAnimEnemy;
			}
			else if (achievedRightBorder)
			{
				iPoint myPos = { (int)std::round(nextFootStep / 64) , position.y / 64 };
				app->pathfinding->CreatePath(myPos, leftBorder);
				currentAnimationEnemy = &walkRAnimEnemy;
			}
		}
	}

	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

	if (debug)
	{
		app->pathfinding->DrawLastPath();
	}


	if (startPath)
	{

		if (path->At(1) != nullptr)
		{
			int aux = position.x;
			destination = path->At(1)->x * 64;
			nextFootStep = custom_lerp(position.x, destination, 0.02f);
			amountToMoveInX = nextFootStep - aux;
			startPath = false;
		}

	}

	b2Vec2 movePos = b2Vec2(PIXEL_TO_METERS(nextFootStep), PIXEL_TO_METERS(position.y));
	pbody->body->SetTransform(movePos, 0);

	nextFootStep += amountToMoveInX;

	if (nextFootStep > destination && achievedLeftBorder == true)
	{
		startPath = true;
	}
	else if (nextFootStep < destination && achievedRightBorder == true)
	{
		startPath = true;
	}

	if (achievedLeftBorder)
	{
		int limitToChange = std::abs(position.x/64 - rightBorder.x);
		if (limitToChange == 1)
		{
			achievedRightBorder = true;
			achievedLeftBorder = false;
		}
	}
	else if (achievedRightBorder)
	{
		int limitToChange = std::abs(position.x / 64 - leftBorder.x);
		if (limitToChange == 1)
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
		SentryMovement();

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
	//std::cout <<"position acabado el movement" << position.x << std::endl;

	currentAnimationEnemy->Update();
	SDL_Rect rect = currentAnimationEnemy->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x / app->win->GetScale() - 17, position.y / app->win->GetScale() - 15, &rect);

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