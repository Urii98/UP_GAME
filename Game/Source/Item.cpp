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

#include "Window.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::STATIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	pbody->listener = this;

	destroy = false;

	return true;
}

bool Item::Update()
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale()) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

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

	app->tex->UnLoad(texture);
	//la memoria del item se destruye en scene

	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		destroy = true;
		break;
	}

}