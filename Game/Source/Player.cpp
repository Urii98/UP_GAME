#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	

	return true;
}

bool Player::Start() {

	//idleRAnim
	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.PushBack({ 803, 91, 26, 22 });
	idleRAnim.PushBack({ 832, 91, 26, 22 });
	idleRAnim.PushBack({ 803, 91, 26, 22 });
	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.075f;

	//idleLAnim
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.PushBack({ 257,91,26,22 });
	idleLAnim.PushBack({ 228,91,26,22 });
	idleLAnim.PushBack({ 257,91,26,22 });
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.loop = true;
	idleLAnim.speed = 0.075f;

	//walkRAnim
	for (int i = 0; i < 9; i++)
	{
		walkRAnim.PushBack({ 658 + (29) * i,198,28,23 });

	}
	walkRAnim.loop = true;
	walkRAnim.speed = 0.125f;

	//walkLAnim
	for (int i = 0; i < 9; i++)
	{
		//walkLAnim.PushBack({ 143 + (29) * i, 28, 23 });
		walkLAnim.PushBack({ 400 - (29) * i,198, 28, 23 });
	}
	walkLAnim.loop = true;
	walkLAnim.speed = 0.125f;

	downLAnim.PushBack({ 187,123,30,22 });
	downRAnim.PushBack({ 869,124,30,22 });

	runRAnim;
	runLAnim;

	jumpRAnim;
	jumpLAnim;

	descentRAnim;
	descentLAnim;

	flyRAnim;
	flyLAnim;

	death.PushBack({ 850, 3775, 28, 23 });
	death.PushBack({ 850, 3775, 28, 23 });
	death.PushBack({ 850, 3775, 28, 23 });
	death.PushBack({ 882, 2896, 28, 23 });
	death.PushBack({ 882, 2896, 28, 23 });
	death.PushBack({ 913, 2896, 28, 23 });
	death.PushBack({ 913, 2896, 28, 23 });
	death.PushBack({ 944, 2896, 28, 23 });
	death.PushBack({ 944, 2896, 28, 23 });
	death.loop = false;
	death.speed = 0.085f;


	opciones = 0;
	speedX = 6;
	speedY = 9; 
	speedYDown = 6;
	estadoP =STOP;

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 12, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");




	std::cout << "MASSA PLAYER - " << pbody->body->GetMass() << std::endl;

	std::cout << pbody->body->GetFixtureList()->GetDensity() << std::endl;

	pbody->body->GetFixtureList()->SetDensity(50.0);

	std::cout << "new density" << pbody->body->GetFixtureList()->GetDensity() << std::endl;

	pbody->body->ResetMassData();
	std::cout << "NUEVA MASA PLAYER - " << pbody->body->GetMass() << std::endl;

	
	return true;
}

void Player::Movimiento()
{


	if (flying) {
		toDescent.Start(0.10);
		if (toDescent.Test())// -- ANIMACIÓN -- de caida
		{
			flying = false;

		}
	}
	if (jumpTimer.Test() == EJECUTANDO && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		flyTimer.Start(0.10); // -- ANIMACIÓN -- volar
		std::cout << "flying" << std::endl;
		flying = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) { //durante 0.05s moveremos al player verticalmente hasta llegar a los 30 pixeles que es la distancia max de un jump
		jumpTimer.Start(0.20);

	}
	

	b2Vec2 vel = b2Vec2(0, 0);
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { // -- ANIMACIÓN -- de caminar a la izquierda
		
		vel = b2Vec2(-speedX, -GRAVITY_Y);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) // -- ANIMACIÓN -- saltar derecha
		{
			jumpTimer.Start(0.20);
		}

		if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO) // -- ANIMACIÓN -- saltar derecha
		{
			vel = b2Vec2(-speedX, -speedY);
		}

		app->render->camera.x += 2;
		if (direccionP /*NO SE COLISIONADA CON NADA y se esta en el aire*/) {

		currentAnimation = &walkLAnim;
		}
		direccionP = IZQUIERDA;
	} 
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { // -- ANIMACIÓN -- de caminar a la derecha
		vel = b2Vec2(speedX, -GRAVITY_Y);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) // -- ANIMACIÓN -- saltar izquierda
		{
			jumpTimer.Start(0.20);
		}

		if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)  // -- ANIMACIÓN -- saltar izquierda
		{
			vel = b2Vec2(speedX, -speedY);
		}

		app->render->camera.x -= 2;
		if (direccionP /*NO SE COLISIONADA CON NADA y se esta en el aire*/) {
			currentAnimation = &walkRAnim;
		}
		direccionP = DERECHA;
	
	}
	else if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)
	{
		vel = b2Vec2(0, -speedY);
	}
	else {  // -- ANIMACIÓN -- IDLE
		estadoP = STOP;
		if (direccionP == IZQUIERDA) {
			currentAnimation = &idleLAnim;
		} 
		else if (direccionP == DERECHA)
		{
			currentAnimation = &idleRAnim;
		}
		
		vel = b2Vec2(0, -GRAVITY_Y);
		
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) // -- ANIMACIÓN -- caer hacia abajo
	{
		vel = b2Vec2(0, -GRAVITY_Y + speedYDown);

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel = b2Vec2(-speedX, -GRAVITY_Y + speedYDown);
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel = b2Vec2(speedX, -GRAVITY_Y + speedYDown);
		}

		if (direccionP == IZQUIERDA) {
			currentAnimation = &downLAnim;
		}
		else if (direccionP == DERECHA)
		{
			currentAnimation = &downRAnim;
		}

	}
	
	/*if (está en contacto con algún collider)
	{
		current animacion si estaba en down, tiene que pasar a idle
	}*/

	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) // -- ANIMACIÓN MORIR
	{
		currentAnimation = &death;
		estadoP = DEATH;
	}
		

	pbody->body->SetLinearVelocity(vel);
}


bool Player::Update()
{
	app->render->playerPosition.x = position.x; //Le pasamos la posicion del player al render para que la cámara siga al player
	app->render->playerPosition.y = position.y;
	

	switch (estadoP)
	{
	case(STOP):
		Movimiento();
		break;
	case(MOVIMIENTO):
		Movimiento();
		break;
	case(DEATH):
		break;

	}
		

	//Update player position in pixels: Posición del COLLIDER:
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 14;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10;


	currentAnimation->Update();
	PostUpdate();
	return true;
}

void Player::PostUpdate()
{

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	
}


bool Player::CleanUp()
{

	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		std::cout << "PLATFORM COLLISION" << std::endl;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}


}