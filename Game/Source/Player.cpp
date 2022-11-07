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
	flapLimit = parameters.attribute("flapLimite").as_int();
	scalarSpeedX = parameters.attribute("scalarSpeedX").as_int();
	scalarSpeedY = parameters.attribute("scalarSpeedY").as_int();
	scalarSpeedYDown = parameters.attribute("scalarSpeedYDown").as_int();


	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	coinSFx = parameters.attribute("coinFxPath").as_string();
	deathSFx = parameters.attribute("kirbyDeathFxPath").as_string();

	oneJump = parameters.attribute("oneJump").as_bool();
	flying = parameters.attribute("flying").as_bool();
	godMode = parameters.attribute("godMode").as_bool();
	
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

	speedX = scalarSpeedX * app->win->GetScale();
	speedY = scalarSpeedY * app->win->GetScale();
	speedYDown = scalarSpeedYDown * app->win->GetScale(); //para cuando estamos en el aire y apretamos "S" para bajar más rápido

	estadoP = STOP;
	currentAnimation = &idleRAnim;
	direccionP = DERECHA;

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 12, bodyType::DYNAMIC);


	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	teleport.turn = false;

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

	if (!godMode)
	{


		if (oneJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && flapLimit < 3)
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

			if (godMode)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				{
					vel = b2Vec2(-speedX, -speedYDown);
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
					vel = b2Vec2(speedX + 2, -GRAVITY_Y);

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


			if (godMode)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				{
					vel = b2Vec2(speedX, -speedYDown);
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


			if (flapLimit == 3)
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


		if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN && !godMode) // -- ANIMACIÓN MORIR
		{
			currentAnimation = &death;
			app->audio->PlayFx(kirbyDeathFx, 0);
			estadoP = DEATH;
		}

	}
	else
	{

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			vel = b2Vec2(0, -speedYDown);
		}
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			vel = b2Vec2(-speedX, 0);
		}
		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			vel = b2Vec2(0, speedYDown);
		}
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			vel = b2Vec2(speedX, 0);
		}

	}
		

	pbody->body->SetLinearVelocity(vel);
}

void Player::ChangePosition(int x, int y)
{

	teleport.posX = x;
	teleport.posY = y;
	teleport.turn = true;

}


bool Player::Update()
{
	//Update player position in pixels: Posición del COLLIDER:
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale()) - 14;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 10;



	//CAMARAAAAAAAAAAAA TENDRA Q SER UNA FUNCION XD
	if (position.x<130 * app->win->GetScale() && position.y<200 * app->win->GetScale()) {//Kirby esquina superior izquierda
		app->render->playerPosition.x = 260 * app->win->GetScale();
		app->render->playerPosition.y = 370 * app->win->GetScale();
	}
	else if (position.x < 130 * app->win->GetScale() && position.y > 385 * app->win->GetScale()) {//Kirby esquina inferior izquierda
		app->render->playerPosition.x = 260 * app->win->GetScale();
		app->render->playerPosition.y = 400 * app->win->GetScale();
	}
	//else if (position.x > 130 * app->win->GetScale() && position.y < 200 * app->win->GetScale()) {//Kirby esquina superior derecha
	//	app->render->playerPosition.x = 260 * app->win->GetScale();
	//	app->render->playerPosition.y = 370 * app->win->GetScale();
	//}
	//else if (position.x > 130 * app->win->GetScale() && position.y > 200 * app->win->GetScale()) {//Kirby esquina inferior derecha
	//	app->render->playerPosition.x = 260 * app->win->GetScale();
	//	app->render->playerPosition.y = 370 * app->win->GetScale();
	//}
	//else if (position.x < 200 * app->win->GetScale()) {//Kirby se va para izquierda
	//	app->render->playerPosition.x = 370 * app->win->GetScale();
	//	app->render->playerPosition.y = position.y * app->win->GetScale();
	//}
	//else if (position.x > 200 * app->win->GetScale()) {//Kirby se va para derecha
	//	app->render->playerPosition.x = 370 * app->win->GetScale();
	//	app->render->playerPosition.y = position.y * app->win->GetScale();
	//}
	//else if (position.y > 767 * app->win->GetScale()) {//Kirby se va para abajo
	//	app->render->playerPosition.x = position.x * app->win->GetScale();
	//	app->render->playerPosition.y = 767 * app->win->GetScale();
	//}
	//else if (position.y < 200 * app->win->GetScale()) { //Kirby se va para arriba
	//	app->render->playerPosition.x = position.x * app->win->GetScale();
	//	app->render->playerPosition.y = 370 * app->win->GetScale();
	//}
	else {//Kirby normal
		app->render->playerPosition.x = position.x * app->win->GetScale(); //Le pasamos la posicion del player al render para que la cámara siga al player
		app->render->playerPosition.y = position.y * app->win->GetScale();
	}

	std::cout << (position.x / app->win->GetScale()) << "    " << (position.y / app->win->GetScale())  << std::endl;





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


	/*std::cout << "position iPoint.x = " << position.x << std::endl;
	std::cout << "position iPoint.y = " << position.y << std::endl;
	std::cout << "position pbody get Transform = " << METERS_TO_PIXELS(pbody->body->GetTransform().p.x) << std::endl;
	std::cout << "position pbody get Transform = " << METERS_TO_PIXELS(pbody->body->GetTransform().p.y) << std::endl;
	std::cout << "CAMERA POSITION.y" << app->render->camera.y << std::endl;*/
	

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (!godMode)
		{
			godMode = true;
		}
		else
		{
			godMode = false;
		}
		
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) //comenzar desde el inicio del level
	{
		ChangePosition(125, 600);
	}
	 
	if (godMode)
	{
		std::cout << "GODMODE ACTIVADO" << std::endl;
		flapLimit = 0;
		estadoP = MOVIMIENTO;
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		CleanUp();
	}

	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		b2Vec2 resetPos = b2Vec2(PIXEL_TO_METERS(30), PIXEL_TO_METERS(30));
		pbody->body->SetTransform(resetPos, 0);
	}

	if (teleport.turn == true)
	{
		b2Vec2 resetPos = b2Vec2(PIXEL_TO_METERS(teleport.posX), PIXEL_TO_METERS(teleport.posY));
		pbody->body->SetTransform(resetPos, 0);

		teleport.turn = false;
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
	std::cout << "entrando en Cleanup - Player" << std::endl;
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
		//ChangePosition(30, 30);
		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		app->audio->PlayFx(pickCoinFxId);
		ChangePosition(125, 600);
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
