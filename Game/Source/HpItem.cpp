#include "HpItem.h"
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

HpItem::HpItem() : Entity(EntityType::HPITEM)
{
	name.Create("hpitem");
}

HpItem::~HpItem() {}

bool HpItem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	hpTexturePath = parameters.attribute("texturepath").as_string();
	effectTexturePath = parameters.attribute("effecttexturepath").as_string();


	return true;
}

bool HpItem::Start() {

	for (int i = 0; i < 5; i++)
	{
		hpAnimation.PushBack({ 0 + 21 * i ,0,21,21 });
	}
	hpAnimation.loop = true;
	hpAnimation.speed = 7.5f * (app->maxFrameDuration / 1000.0);

	for (int i = 0; i < 4; i++)
	{
		effectAnimation.PushBack({ 0 + 32 * i ,0,32,32 });
	}
	effectAnimation.loop = false;
	effectAnimation.speed = 12.5f * (app->maxFrameDuration / 1000.0);




	//initilize textures
	hpTexture = app->tex->Load(hpTexturePath);
	effectTexture = app->tex->Load(effectTexturePath);

	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::STATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::HPITEM;

	pbody->listener = this;
	currentHpAnimation = &hpAnimation;


	destroy = false;
	soundFX = false;

	return true;
}

bool HpItem::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	currentHpAnimation->Update();
	SDL_Rect rect = currentHpAnimation->GetCurrentFrame();
	app->render->DrawTexture(hpTexture, position.x / app->win->GetScale() - 13, position.y / app->win->GetScale() - 13, &rect);


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

bool HpItem::CleanUp()
{
	//memoryleak

	app->tex->UnLoad(hpTexture);
	app->tex->UnLoad(effectTexture);
	active = false;
	//la memoria del item se destruye en scene

	return true;
}

void HpItem::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		//destroy = true;
		currentHpAnimation = &effectAnimation;
		hpTexture = effectTexture;

		if (!soundFX)
		{
			app->audio->PlayFx(app->scene->player->hpId);
			soundFX = true;
		}

		if (app->scene->player->lifePoints < 3)
		{
			app->scene->player->lifePoints++;
		}

		timeToDestroy.Start(0.2f);


		break;
	}

}