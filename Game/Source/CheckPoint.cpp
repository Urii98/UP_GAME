#include "CheckPoint.h"
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

CheckPoint::CheckPoint() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
}

CheckPoint::~CheckPoint() {}

bool CheckPoint::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	numCheckPoint = parameters.attribute("num").as_int();

	checkPointTexturePath = parameters.attribute("texturepath").as_string();


	return true;
}

bool CheckPoint::Start() {

	checkPoint.PushBack({ 0,0,18,20 });
	checkPoint.loop = false;

	savedCheckPoint.PushBack({ 18,0,18,20 });
	savedCheckPoint.loop = false;



	//initilize textures
	checkPointTexture = app->tex->Load(checkPointTexturePath);

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 40, 40, bodyType::STATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::CHECKPOINT;

	pbody->listener = this;
	checkPointCoinAnimation = &checkPoint;


	destroy = false;

	return true;
}

bool CheckPoint::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	if (isCurrent)
	{
		checkPointCoinAnimation = &savedCheckPoint;
	}
	else
	{
		checkPointCoinAnimation = &checkPoint;
	}


	checkPointCoinAnimation->Update();
	SDL_Rect rect = checkPointCoinAnimation->GetCurrentFrame();
	app->render->DrawTexture(checkPointTexture, position.x / app->win->GetScale() -8, position.y / app->win->GetScale() - 8, &rect);







	return true;
}

bool CheckPoint::CleanUp()
{
	//memoryleak

	app->tex->UnLoad(checkPointTexture);
	active = false;
	//la memoria del item se destruye en scene

	return true;
}

void CheckPoint::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		auto dir = this;
		app->scene->checkPointIterator(*dir);
		if (!isDiscovered)
		{
			app->scene->player->checkPointsDiscovered++;
		}
		isDiscovered = true;

		break;
	}

}