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
#include "Window.h"

#include "SceneEnding.h"
#include "SmallEnemy1.h"
#include "EntityManager.h" 


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	coinSFx = parameters.attribute("coinFxPath").as_string();
	deathSFx = parameters.attribute("kirbyDeathFxPath").as_string();

	pickCoinFxId = app->audio->LoadFx(coinSFx);
	kirbyDeathFx = app->audio->LoadFx(deathSFx);

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

	//downLAnim
	downLAnim.PushBack({ 181, 303 , 28 ,28 });
	downLAnim.PushBack({ 212, 303 , 28 ,28 });
	downLAnim.loop = true;
	downLAnim.speed = 0.125f;

	//downRAnim
	downRAnim.PushBack({ 845,304,28,28 });
	downRAnim.PushBack({ 876,304,28,28 });
	downRAnim.loop = true;
	downRAnim.speed = 0.125f;

	runRAnim.PushBack({ 704,231,30,24 });
	runRAnim.PushBack({ 736,231,30,24 });
	runRAnim.PushBack({ 763,231,30,24 });
	runRAnim.PushBack({ 789,231,30,24 });
	runRAnim.PushBack({ 816,231,30,24 });
	runRAnim.PushBack({ 844,231,30,24 });
	runRAnim.PushBack({ 870,231,30,24 });
	runRAnim.PushBack({ 897,231,30,24 });
	runRAnim.loop = true;
	runRAnim.speed = 0.2f;


	runLAnim.PushBack({ 352,231,30,24 });
	runLAnim.PushBack({ 320,231,30,24 });
	runLAnim.PushBack({ 293,231,30,24 });
	runLAnim.PushBack({ 267,231,30,24 });
	runLAnim.PushBack({ 240,231,30,24 });
	runLAnim.PushBack({ 212,231,30,24 });
	runLAnim.PushBack({ 186,231,30,24 });
	runLAnim.PushBack({ 159,231,30,24 });
	runLAnim.loop = true;
	runLAnim.speed = 0.2;


	//jumpRAnim
	jumpRAnim.PushBack({ 635,303,28,28 });
	jumpRAnim.PushBack({ 666,303,28,28 });
	jumpRAnim.PushBack({ 698,303,28,28 });
	jumpRAnim.PushBack({ 729,303,28,28 });
	jumpRAnim.PushBack({ 757,303,28,28 });
	jumpRAnim.PushBack({ 785,303,28,28 });
	jumpRAnim.PushBack({ 815,303,28,28 });
	jumpRAnim.PushBack({ 845,304,28,28 });
	jumpRAnim.PushBack({ 876,304,28,28 });
	jumpRAnim.loop = false;
	jumpRAnim.speed = 0.18f;

	//jumpLAnim
	jumpLAnim.PushBack({ 423, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 392, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 360, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 329, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 301, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 273, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 243, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 212, 303 , 28 ,28 });
	jumpLAnim.PushBack({ 181, 303 , 28 ,28 });
	jumpLAnim.loop = false;
	jumpLAnim.speed = 0.18f;

	//preflyRAnim
	preflyRAnim.PushBack({ 785, 388 , 30 ,30 });
	preflyRAnim.PushBack({ 814, 388 , 30 ,30 });
	preflyRAnim.PushBack({ 844, 388 , 30 ,30 });
	preflyRAnim.loop = false;
	preflyRAnim.speed = 0.125f;

	//flyRAnim
	flyRAnim.PushBack({ 709,426 ,33 ,32 });
	flyRAnim.PushBack({ 743,426 ,33 ,32 });
	flyRAnim.PushBack({ 778,426 ,33 ,32 });
	flyRAnim.PushBack({ 813,426 ,33 ,32 });
	flyRAnim.PushBack({ 847,426 ,33 ,32 });
	flyRAnim.PushBack({ 885,426 ,33 ,32 });
	flyRAnim.loop = true;
	flyRAnim.speed = 0.125f;


	//flyLAnim
	flyLAnim.PushBack({ 343,426 ,33 ,32 });
	flyLAnim.PushBack({ 309,426 ,33 ,32 });
	flyLAnim.PushBack({ 274,426 ,33 ,32 });
	flyLAnim.PushBack({ 239,426 ,33 ,32 });
	flyLAnim.PushBack({ 205,426 ,33 ,32 });
	flyLAnim.PushBack({ 167,426 ,33 ,32 });
	flyLAnim.loop = true;
	flyLAnim.speed = 0.125f;


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


	
	speedX = 6 * app->win->GetScale();
	speedY = 9 * app->win->GetScale();
	speedYDown = 6 * app->win->GetScale();
	estadoP = STOP;
	currentAnimation = &idleRAnim;
	direccionP = DERECHA;
	oneJump = false; //se pasa a true cuando ya se haya hecho un salto, se pasa a false cuando se collisiona
	flying = false;
	flapLimit = 0;

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 12, bodyType::DYNAMIC);


	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

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

	if (oneJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && flapLimit <3)
	{
		flyTimer.Start(0.15); // -- ANIMACIÓN -- volar
		std::cout << "flying" << std::endl;

		//Animacion de prefly, que no se reseteara hasta que haya collision o flying sea false
		if (direccionP == IZQUIERDA) {
			currentAnimation = &flyLAnim; //cambiar a izquierda cuando saque los sprites
		}
		else if (direccionP == DERECHA)
		{
			currentAnimation = &flyRAnim;
		}

		flying = true;
		flapLimit++;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { // -- ANIMACIÓN -- de caminar a la izquierda
		
		vel = b2Vec2(-speedX, -GRAVITY_Y);

		if (!oneJump)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				jumpTimer.Start(0.20);
				collisionP = NOCOLLISION;
				oneJump = true;
				if (currentAnimation != &jumpLAnim)
				{
					currentAnimation = &jumpLAnim;
				}
				flying = true;
			}
			
		}
		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && oneJump && flapLimit < 3)
		{
			flyTimer.Start(0.20);
			collisionP = NOCOLLISION;
			currentAnimation = &flyLAnim;
			flapLimit++;
		}

		if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO) // -- ANIMACIÓN -- saltar derecha
		{
			vel = b2Vec2(-speedX, -speedY);
		
		}

		if (collisionP == COLLISION) {

			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				currentAnimation = &runLAnim;
				vel = b2Vec2(-speedX - 2, -GRAVITY_Y);

			}
			else
			{
				currentAnimation = &walkLAnim;
			}
		}
		else
		{
			if (oneJump && !flying)
			{
				currentAnimation = &flyLAnim;
			}
		}
		direccionP = IZQUIERDA;
		app->render->camera.x += 2;

	} 
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { // -- ANIMACIÓN -- de caminar a la derecha
		vel = b2Vec2(speedX, -GRAVITY_Y);

		if (!oneJump)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) // -- ANIMACIÓN -- saltar izquierda
			{
				jumpTimer.Start(0.20);
				collisionP = NOCOLLISION;
				oneJump = true;
				if (currentAnimation != &jumpRAnim)
				{
					currentAnimation = &jumpRAnim;
				}
				flying = true;
			}
		}
		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && oneJump && flapLimit < 3)
		{
			flyTimer.Start(0.20);
			collisionP = NOCOLLISION;
			currentAnimation = &flyRAnim;
			flapLimit++;
		}
	
		
		if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)  // -- ANIMACIÓN -- saltar izquierda
		{
			vel = b2Vec2(speedX, -speedY);

		}

		if (collisionP == COLLISION) {

			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				currentAnimation = &runRAnim;
				vel = b2Vec2(speedX+2, -GRAVITY_Y);

			}
			else
			{
				currentAnimation = &walkRAnim;
			}
			

		}
		else
		{
			if (oneJump && !flying)
			{
				currentAnimation = &flyRAnim;
			}
		}

		direccionP = DERECHA;
		app->render->camera.x -= 2;
	}
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !flying && !oneJump) { //durante 0.05s moveremos al player verticalmente hasta llegar a los 30 pixeles que es la distancia max de un jump
	jumpTimer.Start(0.30);

	if (direccionP == IZQUIERDA) {
		currentAnimation = &jumpLAnim;

	}
	else if (direccionP == DERECHA)
	{
		currentAnimation = &jumpRAnim;

	}

	collisionP = NOCOLLISION;
	oneJump = true;


	}
	else if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)
	{
		vel = b2Vec2(0, -speedY);
	}
	else {  // -- ANIMACIÓN -- IDLE
		
		estadoP = STOP;

		if (collisionP == COLLISION)
		{
			if (direccionP == IZQUIERDA) {
				currentAnimation = &idleLAnim;
			}
			else if (direccionP == DERECHA)
			{
				currentAnimation = &idleRAnim;
			}
		}


		if (flapLimit ==3)
		{
			flapLimitTimer.Start(6);
		}
		else
		{
			if (flapLimitTimer.Test() == FIN)
			{
				flapLimit = 0;
			}
		}


		
		vel = b2Vec2(0, -GRAVITY_Y);
		
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && collisionP == NOCOLLISION) // -- ANIMACIÓN -- caer hacia abajo (cuando NO hay collision con nada)
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
	
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) // -- ANIMACIÓN MORIR
	{
		currentAnimation = &death;
		app->audio->PlayFx(kirbyDeathFx, 0);
		estadoP = DEATH;
	}



		

	pbody->body->SetLinearVelocity(vel);
}


bool Player::Update()
{
	app->render->playerPosition.x = position.x * app->win->GetScale(); //Le pasamos la posicion del player al render para que la cámara siga al player
	app->render->playerPosition.y = position.y * app->win->GetScale();

	this->active;

	switch (estadoP)
	{
	case(STOP):
		Movimiento();
		break;
	case(MOVIMIENTO):
		Movimiento();
		break;
	case(DEATH):
		app->audio->PlayFx(kirbyDeathFx,0);
		currentAnimation = &death;
		
		break;
	case(NONE):
		break;
	default: 
		break;
		
	}

	if (estadoP == DEATH)
	{
		deathTimer.Start(4);
		estadoP = NONE;
	}
	if (deathTimer.Test() == FIN)
	{
		app->sceneEnding->ending = true;
		CleanUp();
	}

	//Update player position in pixels: Posición del COLLIDER:
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale()) - 14;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 10;

	//cojo la posicion del player que me servira en el siguiente frame para chequear si se ha movido
	prevPosition = position.x;

	
		std::cout << "position iPoint.x = " << position.x << std::endl;
		std::cout << "position iPoint.y = " << position.y << std::endl;
		std::cout << "position pbody get Transform = " << METERS_TO_PIXELS(pbody->body->GetTransform().p.x) << std::endl;
		std::cout << "position pbody get Transform = " << METERS_TO_PIXELS(pbody->body->GetTransform().p.y) << std::endl;

		std::cout << "CAMERA POSITION.y" << app->render->camera.y << std::endl;
	

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		CleanUp();
	}

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
	app->tex->UnLoad(texture);

	//la memoria de player la libero directamente en scene
	//app->entityManager->DestroyEntity(this);

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
		collisionP = COLLISION;
		jumpRAnim.Reset();
		jumpLAnim.Reset();
		oneJump = false;
		flying = false;
		flapLimit = 0;
		std::cout << "PLATFORM COLLISION" << std::endl;
		break;
	case ColliderType::ENEMY:

		//estadoP = DEATH;

		break;
		
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}


}

