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

	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.PushBack({ 803, 91, 26, 22 });
	idleRAnim.PushBack({ 832, 91, 26, 22 });
	idleRAnim.PushBack({ 803, 91, 26, 22 });
	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.PushBack({ 774, 91, 26, 22 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.075f;

	idleLAnim;

	//5 pixeles de separacion 598 198 26 23
	for (int i = 0; i < 9; i++)
	{
		walkRAnim.PushBack({ 658 + (29)*i,198,28,23}); 

	}
	walkRAnim.loop = true;
	walkRAnim.speed = 0.125f;

	walkRAnim;
	walkLAnim;
	runRAnim;
	runLAnim;
	jumpRAnim;
	jumpLAnim;
	descentRAnim;
	descentLAnim;
	flyRAnim;
	flyLAnim;
	death;


	opciones = 0;
	speedX = 6;
	speedY = 9; // 30/30; //pixeles entre 30 frames (medio segundo)
	estadoP =STOP;

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 12, bodyType::DYNAMIC);

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

	b2Vec2 vel = b2Vec2(0, 0);
		//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { // -- ANIMACIÓN -- de caminar a la izquierda
		
		vel = b2Vec2(-speedX, -GRAVITY_Y);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			jumpTimer.Start(0.20);
		}

		if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)
		{
			vel = b2Vec2(-speedX, -speedY);
		}

		
		app->render->camera.x += 2;
		
		
	} 
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { // -- ANIMACIÓN -- de caminar a la derecha
		vel = b2Vec2(speedX, -GRAVITY_Y);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			jumpTimer.Start(0.20);
		}

		if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)
		{
			vel = b2Vec2(speedX, -speedY);
		}

		currentAnimation = &walkRAnim;
		app->render->camera.x -= 2;
	
	}
	else if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)
	{
		vel = b2Vec2(0, -speedY);
	}
	else {
		estadoP = STOP;
		currentAnimation = &idleRAnim;
		vel = b2Vec2(0, -GRAVITY_Y);
		
	}
	

	pbody->body->SetLinearVelocity(vel);
}


bool Player::Update()
{
	app->render->playerPosition.x = position.x; //Le pasamos la posicion del player al render para que la cámara siga al player
	app->render->playerPosition.y = position.y;
	
	

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


	switch (estadoP)
	{
	case(STOP): // -- ANIMACIÓN -- IDLE
		Movimiento();
		break;
	case(MOVIMIENTO):
		Movimiento();
		break;
	}


	//if (jumpTimer.Test() == FIN)
//{
//	std::cout << "FIN" << std::endl;

//} 
//else if(jumpTimer.Test() == APAGADO)
//{
//	std::cout << "APAGADO" << std::endl;
//}
//else if(jumpTimer.Test() == EJECUTANDO)
//{
//	std::cout << "EJECUTANDO" << std::endl;
//}

	//if (opciones == 2) { //MOVIMIENTOS VERTICALES

	//	b2Vec2 vel = pbody->body->GetLinearVelocity();
	//	float desiredVel = 0;

	//	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	//		desiredVel = -80;
	//		
	//	}

	//	float velChange = desiredVel - vel.y;
	//	float impulse = pbody->body->GetMass() * velChange; //disregard time factor
	//	pbody->body->ApplyLinearImpulse(b2Vec2(0, impulse), pbody->body->GetWorldCenter(), true);

	//}

	

	//Update player position in pixels: Posición del COLLIDER:
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 13;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 10;

	//SDL_Rect* a{ 774,91,26,22 };

	
	


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