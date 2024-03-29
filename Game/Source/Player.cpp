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
#include "Map.h"

#include "SceneEnding.h"
#include "SmallEnemy1.h"
#include "EntityManager.h" 
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "PlayerSensors.h"

#include "Optick/include/optick.h"


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
	lifePoints = parameters.attribute("lifePoints").as_int();
	moneyPoints = parameters.attribute("moneyPoints").as_int();


	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	coinSFx = parameters.attribute("coinFxPath").as_string();
	deathSFx = parameters.attribute("kirbyDeathFxPath").as_string();
	victorySFx = parameters.attribute("kirbyVictoryFxPath").as_string();
	swordFxPath = parameters.attribute("swordFxPath").as_string();
	playerDmgPath = parameters.attribute("playerDmgPath").as_string();
	hpSFx = "Assets/Audio/Fx/hp.ogg";

	musicScenePath = parameters.attribute("musicScenePath").as_string();
	musicStopPath = parameters.attribute("musicStopPath").as_string();

	oneJump = parameters.attribute("oneJump").as_bool();
	flying = parameters.attribute("flying").as_bool();
	godMode = parameters.attribute("godMode").as_bool();
	victory = parameters.attribute("victory").as_bool();
	deathFxbool = parameters.attribute("deathFxbool").as_bool();
	winFxbool = parameters.attribute("win").as_bool();

	
	pickCoinFxId = app->audio->LoadFx(coinSFx);
	kirbyDeathFx = app->audio->LoadFx(deathSFx);
	kirbyVictoryFx = app->audio->LoadFx(victorySFx);
	swordFxId = app->audio->LoadFx(swordFxPath);
	playerDmgId = app->audio->LoadFx(playerDmgPath);
	hpId = app->audio->LoadFx(hpSFx);

	texturePath = parameters.attribute("texturepath").as_string();
	swordUIPath = parameters.attribute("swordUIPath").as_string();
	swordUIOffPath = parameters.attribute("swordUIOffPath").as_string();
	lifePath = parameters.attribute("lifePath").as_string();
	

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
	idleRAnim.speed = 4.6875f * (app->maxFrameDuration / 1000.0);

	//idleLAnim
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.PushBack({ 257,91,26,22 });
	idleLAnim.PushBack({ 228,91,26,22 });
	idleLAnim.PushBack({ 257,91,26,22 });
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.PushBack({ 286,91,26,22 });
	idleLAnim.loop = true;
	idleLAnim.speed = 4.6875f * (app->maxFrameDuration / 1000.0);

	//walkRAnim
	for (int i = 0; i < 9; i++)
	{
		walkRAnim.PushBack({ 658 + (29) * i,198,28,23 });

	}
	walkRAnim.loop = true;
	walkRAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);

	//walkLAnim
	for (int i = 0; i < 9; i++)
	{
		//walkLAnim.PushBack({ 143 + (29) * i, 28, 23 });
		walkLAnim.PushBack({ 400 - (29) * i,198, 28, 23 });
	}
	walkLAnim.loop = true;
	walkLAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);

	//downLAnim
	downLAnim.PushBack({ 181, 303 , 28 ,28 });
	downLAnim.PushBack({ 212, 303 , 28 ,28 });
	downLAnim.loop = true;
	downLAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);

	//downRAnim
	downRAnim.PushBack({ 845,304,28,28 });
	downRAnim.PushBack({ 876,304,28,28 });
	downRAnim.loop = true;
	downRAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);

	runRAnim.PushBack({ 704,231,30,24 });
	runRAnim.PushBack({ 736,231,30,24 });
	runRAnim.PushBack({ 763,231,30,24 });
	runRAnim.PushBack({ 789,231,30,24 });
	runRAnim.PushBack({ 816,231,30,24 });
	runRAnim.PushBack({ 844,231,30,24 });
	runRAnim.PushBack({ 870,231,30,24 });
	runRAnim.PushBack({ 897,231,30,24 });
	runRAnim.loop = true;
	runRAnim.speed = 12.5f * (app->maxFrameDuration / 1000.0);


	runLAnim.PushBack({ 352,231,30,24 });
	runLAnim.PushBack({ 320,231,30,24 });
	runLAnim.PushBack({ 293,231,30,24 });
	runLAnim.PushBack({ 267,231,30,24 });
	runLAnim.PushBack({ 240,231,30,24 });
	runLAnim.PushBack({ 212,231,30,24 });
	runLAnim.PushBack({ 186,231,30,24 });
	runLAnim.PushBack({ 159,231,30,24 });
	runLAnim.loop = true;
	runLAnim.speed = 12.5f * (app->maxFrameDuration / 1000.0);


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
	jumpRAnim.speed = 11.25f * (app->maxFrameDuration / 1000.0);

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
	jumpLAnim.speed = 11.25f * (app->maxFrameDuration / 1000.0);

	//preflyRAnim
	preflyRAnim.PushBack({ 785, 388 , 30 ,30 });
	preflyRAnim.PushBack({ 814, 388 , 30 ,30 });
	preflyRAnim.PushBack({ 844, 388 , 30 ,30 });
	preflyRAnim.loop = false;
	preflyRAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);

	//flyRAnim
	flyRAnim.PushBack({ 709,426 ,33 ,32 });
	flyRAnim.PushBack({ 743,426 ,33 ,32 });
	flyRAnim.PushBack({ 778,426 ,33 ,32 });
	flyRAnim.PushBack({ 813,426 ,33 ,32 });
	flyRAnim.PushBack({ 847,426 ,33 ,32 });
	flyRAnim.PushBack({ 885,426 ,33 ,32 });
	flyRAnim.loop = true;
	flyRAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);


	//flyLAnim
	flyLAnim.PushBack({ 343,426 ,33 ,32 });
	flyLAnim.PushBack({ 309,426 ,33 ,32 });
	flyLAnim.PushBack({ 274,426 ,33 ,32 });
	flyLAnim.PushBack({ 239,426 ,33 ,32 });
	flyLAnim.PushBack({ 205,426 ,33 ,32 });
	flyLAnim.PushBack({ 167,426 ,33 ,32 });
	flyLAnim.loop = true;
	flyLAnim.speed = 7.8125f * (app->maxFrameDuration / 1000.0);


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
	death.speed = 5.3125f * (app->maxFrameDuration / 1000.0);

	win.PushBack({ 721, 58, 25, 25 });
	win.PushBack({ 747, 58, 25, 25 });
	win.PushBack({ 773, 58, 25, 25 });
	win.PushBack({ 799, 58, 25, 25 });
	win.PushBack({ 825, 58, 25, 25 });
	win.PushBack({ 853, 58, 25, 25 });
	win.PushBack({ 881, 58, 25, 25 });
	win.loop = false;
	win.speed = 5.3125f * (app->maxFrameDuration / 1000.0);

	swordAttackRAnim.PushBack({ 168,1439,42,48 });
	swordAttackRAnim.PushBack({ 217,1439,42,48 });
	swordAttackRAnim.PushBack({ 269,1439,42,48 });
	swordAttackRAnim.PushBack({ 324,1439,47,48 });
	swordAttackRAnim.PushBack({ 387,1439,47,48 });
	swordAttackRAnim.PushBack({ 451,1439,47,48 });
	swordAttackRAnim.PushBack({ 39,1519,50,48 });
	swordAttackRAnim.PushBack({ 110,1519,58,48 });
	swordAttackRAnim.PushBack({ 188,1519,63,48 });
	swordAttackRAnim.PushBack({ 271,1519,74,48 });
	swordAttackRAnim.PushBack({ 367,1519,52,48 });
	swordAttackRAnim.PushBack({ 439,1519,67,48 });
	swordAttackRAnim.PushBack({ 524,1519,69,48 });
	swordAttackRAnim.PushBack({ 609,1519,35,48 });
	swordAttackRAnim.loop = true;
	swordAttackRAnim.speed = 25.0f * (app->maxFrameDuration / 1000.0);

	
	swordAttackLAnim.PushBack({ 461,1885,42,48 });
	swordAttackLAnim.PushBack({ 412,1885,42,48 });
	swordAttackLAnim.PushBack({ 360,1885,42,48 });
	swordAttackLAnim.PushBack({ 301,1885,47,48 });
	swordAttackLAnim.PushBack({ 238,1885,47,48 });
	swordAttackLAnim.PushBack({ 174,1885,47,48 });
	swordAttackLAnim.PushBack({ 582,1965,50,48 });
	swordAttackLAnim.PushBack({ 502,1965,58,48 });
	swordAttackLAnim.PushBack({ 420,1965,63,48 });
	swordAttackLAnim.PushBack({ 326,1965,74,48 });
	swordAttackLAnim.PushBack({ 252,1965,52,48 });
	swordAttackLAnim.PushBack({ 164,1965,68,48 });
	swordAttackLAnim.PushBack({ 78,1965,69,48 });
	swordAttackLAnim.PushBack({ 27,1965,35,48 });
	swordAttackLAnim.loop = true;
	swordAttackLAnim.speed = 25.0f * (app->maxFrameDuration / 1000.0);

	hitR.PushBack({ 706,1034,27,27 });
	hitR.PushBack({ 651,990,33,33 });
	hitR.PushBack({ 723,990,33,33 });
	hitR.PushBack({ 761,990,34,33 });
	hitR.loop = true;
	hitR.speed = 15.625f * (app->maxFrameDuration / 1000.0);

	hitL.PushBack({ 354,1034,27,27 });
	hitL.PushBack({ 402,990,33,33 });
	hitL.PushBack({ 330,990,33,33 });
	hitL.PushBack({ 292,990,33,33 });
	hitL.loop = true;
	hitL.speed = 15.625f * (app->maxFrameDuration / 1000.0);

	speedX = scalarSpeedX * app->win->GetScale();
	speedY = scalarSpeedY * app->win->GetScale();
	speedYDown = scalarSpeedYDown * app->win->GetScale(); //para cuando estamos en el aire y apretamos "S" para bajar m�s r�pido

	estadoP = STOP;
	currentAnimation = &idleRAnim;
	direccionP = DERECHA;

	//initilize textures
	texture = app->tex->Load(texturePath);
	swordUITexture = app->tex->Load(swordUIPath);
	swordUIOffTexture = app->tex->Load(swordUIOffPath);
	lifePointsTexture = app->tex->Load(lifePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 12, bodyType::DYNAMIC);
	//sensor = app->physics->CreateRectangle(position.x + 15, position.y + 15, 15, 15, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;
	//sensor->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;
	//sensor->ctype = ColliderType::SENSOR;

	teleport.turn = false;

	/*std::cout << "MASSA PLAYER - " << pbody->body->GetMass() << std::endl;

	std::cout << pbody->body->GetFixtureList()->GetDensity() << std::endl;*/

	pbody->body->GetFixtureList()->SetDensity(50.0);

	//std::cout << "new density" << pbody->body->GetFixtureList()->GetDensity() << std::endl;

	pbody->body->ResetMassData();
	//std::cout << "NUEVA MASA PLAYER - " << pbody->body->GetMass() << std::endl;

	app->audio->PlayMusic(musicScenePath, 0);

	posXBeforeAttack = 0;
	posYBeforeAttack = 0;
	checkPointsDiscovered = 0;
	skillSwitch = false;
	sword = false;
	drawSwordUI = true;
	invulnerable = false;
	posXFromCheckPoint = 0;
	collidingWithCheckPoint = false;
	timerDead = false;
	highestScore = 0;
	saveScoreFromVictory = false;
	
	return true;
}

void Player::Movimiento()
{

	
	b2Vec2 vel = b2Vec2(0, 0);

	if (!godMode)
	{


		if (oneJump && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && flapLimit < 3 && position.y>0)
		{
			flyTimer.Start(0.15); // -- ANIMACI�N -- volar
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
		else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && position.x >0) { // -- ANIMACI�N -- de caminar a la izquierda

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

			if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO) // -- ANIMACI�N -- saltar derecha
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
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && position.x<7973) { // -- ANIMACI�N -- de caminar a la derecha
			vel = b2Vec2(speedX, -GRAVITY_Y);

			if (!oneJump)
			{
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) // -- ANIMACI�N -- saltar izquierda
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


			if (jumpTimer.Test() == EJECUTANDO || flyTimer.Test() == EJECUTANDO)  // -- ANIMACI�N -- saltar izquierda
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
		else {  // -- ANIMACI�N -- IDLE

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

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && collisionP == NOCOLLISION) // -- ANIMACI�N -- caer hacia abajo (cuando NO hay collision con nada)
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
	

	if (estadoP == VICTORY)
	{
		vel = b2Vec2(0, -GRAVITY_Y);
	}

	pbody->body->SetLinearVelocity(vel);
}

void Player::ChangePosition(int x, int y)
{

	teleport.posX = x;
	teleport.posY = y;
	teleport.turn = true;

}


bool Player::Update(float dt)
{
	OPTICK_EVENT();

	//Update player position in pixels: Posici�n del COLLIDER:
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x / app->win->GetScale()) - 14;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y / app->win->GetScale()) - 10;

	auto x = app->input->GetMouseX();
	auto y = app->input->GetMouseY();


	//Camara
	PlayerDebug();
	PlayerVictory();
	

	if (estadoP == DEATH && !godMode && !deathFxbool)
	{
		deathTimer.Start(4);

	}
	if (deathTimer.Test() == FIN)
	{
		estadoP = NONE;
		app->render->playerPosition.x = 542;
		app->render->playerPosition.y = 410;
		app->sceneEnding->ending = true;
		//CleanUp();
	}

	this->active;

	switch (estadoP)
	{
	case(STOP):
		Camera();
		Movimiento();
		break;

	case(MOVIMIENTO):
		Camera();
		Movimiento();
		break;

	case(DEATH):

		Camera();

		if (!godMode && !deathFxbool)
		{
			currentAnimation = &death;
			deathFxbool = true;
			app->audio->PlayFx(kirbyDeathFx, 0);
			b2Vec2 vel = b2Vec2(0, 0);
			pbody->body->SetLinearVelocity(vel);
			pbody->body->SetActive(false);
			sword = true;
			//app->scene->timer.Start();
			timerDead = false;

		}

		break;

	case(VICTORY):
		if (!winFxbool)
		{
			Movimiento();
			Camera();
			app->audio->PlayFx(kirbyVictoryFx, 0);
			currentAnimation = &win;
			estadoP = NONE;
			b2Vec2 vel = b2Vec2(0, 0);
			pbody->body->SetLinearVelocity(vel);
			winFxbool = true;
			if (saveScoreFromVictory)
			{
				app->SaveGameRequest();
				saveScoreFromVictory = false;
			}
			highestScore = app->scene->timer.ReadSec();
		}
		break;

	case(SKILL):
		Camera();
		ChangePosition(posXBeforeAttack, posYBeforeAttack);
		break;

	case(HIT):

		if (direccionE == DERECHA)
		{
			b2Vec2 vel = b2Vec2(-1, -50);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}
		else if (direccionE == IZQUIERDA)
		{
			b2Vec2 vel = b2Vec2(-1, 50);
			pbody->body->ApplyForce(vel, pbody->body->GetLocalCenter(), true);
		}

		invulnerable = true;
		invulnerableTime.Start(2.5);

		break;

	case(NONE):

		break;

	default: 
		break;
		
	}


	if (teleport.turn == true)
	{
		b2Vec2 resetPos = b2Vec2(PIXEL_TO_METERS(teleport.posX), PIXEL_TO_METERS(teleport.posY));
		pbody->body->SetTransform(resetPos, 0);

		teleport.turn = false;
	}

	if (hitTimer.Test() == FIN && estadoP == HIT)
	{
		estadoP = MOVIMIENTO; 
		currentAnimation = &idleRAnim;
	}

	if (invulnerableTime.Test() == FIN)
	{
		invulnerable = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		if (collidingWithCheckPoint)
		{
			int checkDist = 0;
			if (posXFromCheckPoint > position.x)
			{
				checkDist = posXFromCheckPoint - position.x;
			}
			else if (position.x > posXFromCheckPoint)
			{
				checkDist = position.x - posXFromCheckPoint;
			}

			if (checkDist < 45)
			{
				TeleportToCheckPoint(playerCheckPoint);
			}
			collidingWithCheckPoint = false;
		}
	}

	
	if (app->scene->continueFromGame)
	{
		app->scene->continueFromGame = false;
		app->LoadGameRequest();
	}

	
	currentAnimation->Update();
	PostUpdate();

	if (app->scene->timer.ReadSec() > 300 && !timerDead && !godMode)
	{
		estadoP = DEATH;
		timerDead = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
	{
		highestScore = 2;
	}

	return true;
}

void Player::PostUpdate()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (currentAnimation == &swordAttackRAnim)
	{
		app->render->DrawTexture(texture, position.x-13, position.y-26, &rect);
	}
	else if (currentAnimation == &swordAttackLAnim)
	{
		app->render->DrawTexture(texture, position.x -23, position.y - 26, &rect);
	}
	else if (estadoP == HIT)
	{
		if (direccionE == DERECHA)
		{	
			currentAnimation = &hitR;
			app->render->DrawTexture(texture, position.x, position.y, &rect);
		}
		else if (direccionE == IZQUIERDA)
		{
			currentAnimation = &hitL;
			app->render->DrawTexture(texture, position.x, position.y, &rect);
		}
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}
	
	DrawLifePoints();


}

void Player::DrawLifePoints()
{
	SDL_Rect lifePoint = { 0,0,15,15 };
	SDL_Rect noLifePoint = { 0,15,15,15 };

	switch (lifePoints)
	{
	case(0):
		app->render->DrawTexture(lifePointsTexture, position.x - 20, position.y - 15, &noLifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x - 10, position.y - 15, &noLifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x, position.y - 15, &noLifePoint);
		break;

	case(1):
		app->render->DrawTexture(lifePointsTexture, position.x - 20, position.y - 15, &lifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x - 10, position.y - 15, &noLifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x, position.y - 15, &noLifePoint);
		break;

	case(2):
		app->render->DrawTexture(lifePointsTexture, position.x - 20, position.y - 15, &lifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x - 10, position.y - 15, &lifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x, position.y - 15, &noLifePoint);
		break;

	case(3):
		app->render->DrawTexture(lifePointsTexture, position.x - 20, position.y - 15, &lifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x - 10, position.y - 15, &lifePoint);
		app->render->DrawTexture(lifePointsTexture, position.x, position.y - 15, &lifePoint);
		break;
	}
}


bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(swordUITexture);
	app->tex->UnLoad(swordUIOffTexture);
	app->tex->UnLoad(lifePointsTexture);
	if(pbody != NULL)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}
	

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
		moneyPoints += 1;
		
		break;

	case ColliderType::DEATH:
		LOG("Collision DEATH");
		ChangePosition(125, 750);
		if (!godMode)
		{
			lifePoints--;
			app->audio->PlayFx(playerDmgId);

		}

		if (lifePoints == 0)
		{
			estadoP = DEATH;
		}
		break;

	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");

		break;

	case ColliderType::ENEMY:
		if (!godMode)
		{
			if (lifePoints > 0 && !invulnerable)
			{
				estadoP = HIT;
				b2Vec2 vel = b2Vec2(0, 0);
				hitTimer.Start(1.0f);
				pbody->body->SetLinearVelocity(vel);
				lifePoints--;

				if (physB->body->GetPosition().x > physA->body->GetPosition().x)
				{
					direccionE = DERECHA;
				}
				else if (physB->body->GetPosition().x < physA->body->GetPosition().x)
				{
					direccionE = IZQUIERDA;
				}

				app->audio->PlayFx(playerDmgId);
			}
			
			if(lifePoints == 0)
			{
				estadoP = DEATH;
			}
		}
		break;
	case ColliderType::ENEMYFLY:
		if (!godMode)
		{
			if (lifePoints > 0 && !invulnerable)
			{
				estadoP = HIT;
				b2Vec2 vel = b2Vec2(0, 0);
				hitTimer.Start(1.0f);
				pbody->body->SetLinearVelocity(vel);
				lifePoints--;

				if (physB->body->GetPosition().x > physA->body->GetPosition().x)
				{
					direccionE = DERECHA;
				}
				else if (physB->body->GetPosition().x < physA->body->GetPosition().x)
				{
					direccionE = IZQUIERDA;
				}

				app->audio->PlayFx(playerDmgId);
			}

			if (lifePoints == 0)
			{
				estadoP = DEATH;
			}
		}
		break;

	case ColliderType::CHECKPOINT:
		playerCheckPoint = physB->listener->numCheckPoint;
		posXFromCheckPoint = position.x;
		collidingWithCheckPoint = true;
		break;

	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}

void Player::Camera(){
	if (position.x < 257 && position.y < 195) {//Kirby esquina superior izquierda
		app->render->playerPosition.x = 542;
		app->render->playerPosition.y = 410;
		//std::cout << "SUP IZ" << std::endl;
	}
	else if (position.x < 257 && position.y > 766) {//Kirby esquina inferior izquierda
		app->render->playerPosition.x = 520;
		app->render->playerPosition.y = 1520;
		//std::cout << "INF IZ" << std::endl;
	}
	else if (position.x > 7742 && position.y < 195 ) {//Kirby esquina superior derecha
		app->render->playerPosition.x = 15512;
		app->render->playerPosition.y = 410;
		//std::cout << "SUP DER" << std::endl;
	}
	else if (position.x > 7742 && position.y > 766 ) {//Kirby esquina inferior derecha
		app->render->playerPosition.x = 15512;
		app->render->playerPosition.y = 1552;
		//std::cout << "INF DER" << std::endl;
	}
	else if (position.x < 257) {//Kirby se va para izquierda
		app->render->playerPosition.x = 542;
		app->render->playerPosition.y = position.y * app->win->GetScale();
		//std::cout << "IZ" << std::endl;
	}
	else if (position.x > 7742) {//Kirby se va para derecha
		app->render->playerPosition.x = 15512;
		app->render->playerPosition.y = position.y * app->win->GetScale();
		//std::cout << "DER" << std::endl;
	}
	else if (position.y > 766) {//Kirby se va para abajo
		app->render->playerPosition.x = position.x * app->win->GetScale();
		app->render->playerPosition.y = 1512;
		//std::cout << "INF" << std::endl;
	}
	else if (position.y < 195) { //Kirby se va para arriba
		app->render->playerPosition.x = position.x * app->win->GetScale();
		app->render->playerPosition.y = 410;
		//std::cout << "SUP" << std::endl;
	}
	else {//Kirby normal
		app->render->playerPosition.x = position.x * app->win->GetScale(); //Le pasamos la posicion del player al render para que la c�mara siga al player
		app->render->playerPosition.y = position.y * app->win->GetScale();
		//std::cout << "NORMAL" << std::endl;
	}
}

void Player::PlayerDebug() {

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)//GODMODE
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

	if (godMode)
	{
		//std::cout << "GODMODE ACTIVADO" << std::endl;
		flapLimit = 0;
		estadoP = MOVIMIENTO;
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) //comenzar desde el inicio del level
	{
		ChangePosition(125, 600);
	}

	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		b2Vec2 resetPos = b2Vec2(PIXEL_TO_METERS(30), PIXEL_TO_METERS(30));
		pbody->body->SetTransform(resetPos, 0);
	}
}

void Player::PlayerVictory() {
	if (position.x > 7630 && position.y < 500 && !victory) {
		app->audio->PlayMusic(musicStopPath, 0);
		estadoP = VICTORY;
		victory = true;
		saveScoreFromVictory = true;
	}

}

void Player::TeleportToCheckPoint(int numCheckPoint)
{
	ListItem<Entity*>* item;
	item = app->scene->entities.start;
	int goal = numCheckPoint +1;
	


	if (checkPointsDiscovered == 1)
	{
		return;
	}

	if (numCheckPoint == 6)
	{
		goal = 1;
	}

	if (goal > checkPointsDiscovered)
	{
		goal = 1;
	}

	while (item != NULL)
	{
		if (item->data->name == "checkpoint" && item->data->numCheckPoint == 1)
		{
			break;
		}
		item = item->next;
	}

	while (item != NULL)
	{
		if (goal == item->data->numCheckPoint)
		{
			auto pos = item->data->position;
			ChangePosition(pos.x, pos.y);
			
			return;
		}
		item = item->next;
	}
}