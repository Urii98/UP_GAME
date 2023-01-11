#include "Item.h"
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

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	
	coinTexturePath = parameters.attribute("texturepath").as_string();
	effectTexturePath = parameters.attribute("effecttexturepath").as_string();
	map = parameters.attribute("map").as_int();

	return true;
}

bool Item::Start() {

	for (int i = 0; i < 5; i++)
	{
		coinAnimation.PushBack({ 0 + 24*i ,0,24,24 });
	}
	coinAnimation.loop = true;
	coinAnimation.speed = 0.12f;

	for (int i = 0; i < 4; i++)
	{
		effectAnimation.PushBack({ 0 + 32 * i ,0,32,32 });
	}
	effectAnimation.loop = false;
	effectAnimation.speed = 0.2f;
	

	if (app->sceneTitle->mapSelect == false) {
		if (map == 1) {
			position.x = 4328;
			position.y = 430;
		}
		if (map == 2) {
			position.x = 6976;
			position.y = 82;
		}
		if (map == 3) {
			position.x = 12352;
			position.y = 768;
		}
		if (map == 4) {
			position.x = 14696;
			position.y = 244;
		}

	}


	//initilize textures
	coinTexture = app->tex->Load(coinTexturePath);
	effectTexture = app->tex->Load(effectTexturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x, position.y, 12, bodyType::STATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	pbody->listener = this;
	currentCoinAnimation = &coinAnimation;
	

	destroy = false;
	soundFX = false;

	return true;
}

bool Item::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	currentCoinAnimation->Update();
	SDL_Rect rect = currentCoinAnimation->GetCurrentFrame();
	app->render->DrawTexture(coinTexture, position.x / app->win->GetScale() - 13, position.y / app->win->GetScale() - 13, &rect);
	

	if (timeToDestroy.Test() == FIN)
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

bool Item::CleanUp()
{
	//memoryleak

	app->tex->UnLoad(coinTexture);
	app->tex->UnLoad(effectTexture);
	active = false;
	//la memoria del item se destruye en scene

	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		//destroy = true;
		currentCoinAnimation = &effectAnimation;
		coinTexture = effectTexture;

		if (!soundFX)
		{
			app->audio->PlayFx(app->scene->player->pickCoinFxId);
			soundFX = true;
		}

		timeToDestroy.Start(0.2f);
		

		break;
	}

}