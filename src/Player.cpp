// =====================
// PLAYER IMPLEMENTATION
// =====================

#include "Player.h"
#include "GameManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Item.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Map.h"
#include "Protection.h"
#include "Scene.h"
#include "Window.h"
#include "CheeseBall.h"
#include <algorithm>

// Variables estaticas del jugador
int Player::score = 0;
bool Player::IsPlayerProtected = false;

// =====================
// CONSTRUCTOR / DESTRUCTOR
// =====================

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
	pbody = nullptr;
	godMode = false;
}

Player::~Player() {

}

// =====================
// CICLO DE VIDA
// =====================

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(86, 86);
	return true;
}

bool Player::Start() {
	godMode = false;
	state = RUNNING;
	lastState = RUNNING;
	// load
	std::unordered_map<int, std::string> aliases2x3 = { {2,"run"},{16,"jump"},{28,"hoponcheese"} };
	std::unordered_map<int, std::string> aliases3x3 = { {0,"idle"},{17,"idleOnCheese"} };
	std::unordered_map<int, std::string> aliases3x4 = { {0,"attack2"},{6,"attack3"},{16,"attack1"} };
	std::unordered_map<int, std::string> aliases4x4 = { {0,"ballroll"} };
	std::unordered_map<int, std::string> aliases5x5 = { {0,"ballkick"} };
	float limitUp = Engine::GetInstance().render->camera.h / 4;
	Engine::GetInstance().render->camera.y = limitUp;

	anims2x3.LoadFromTSX("assets/Textures/Spritesheets/Jester/2x3/j_sp.tsx", aliases2x3);
	anims3x3.LoadFromTSX("assets/Textures/Spritesheets/Jester/3x3/j_sp_idle.tsx", aliases3x3);
	anims3x4.LoadFromTSX("assets/Textures/Spritesheets/Jester/3x4/j_sp_3x4.tsx", aliases3x4);
	anims4x4.LoadFromTSX("assets/Textures/Spritesheets/Jester/4x4/j_sp_ballroll.tsx", aliases4x4);
	anims5x5.LoadFromTSX("assets/Textures/Spritesheets/Jester/5x5/j_sp_5x5.tsx", aliases5x5);

	texture2x3 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/2x3/j_2x3.png");
	texture3x3 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/3x3/sprite_jester_idles_02.png");
	texture3x4 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/3x4/j_3x4.png");
	texture4x4 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/4x4/j_ballroll.png");
	texture5x5 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/5x5/j_5x5.png");


	textureShowCheese = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/4x4/Jester_Show_That_Cheese_spritesheet.png");
	std::unordered_map<int, std::string> aliasesShowCheese = { {0, "show_cheese"} };
	animsShowCheese.LoadFromTSX("assets/Textures/Spritesheets/Jester/4x4/j_sp_cheese_show.tsx", aliasesShowCheese);


	//L03: TODO 2: Initialize Player parameters
	texture = texture3x3;
	currentAnimSet = &anims3x3;
	currentAnimSet->SetCurrent("idle");
	// L08 TODO 5: Add physics to the player - initialize physics body
	texW = 215;
	texH = 384;
	pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.0f);

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;
	b2Body_SetGravityScale(pbody->body, 2.25f); // Borrar para tener el salto antiguo

	attackHitbox = Engine::GetInstance().physics->CreateRectangleSensor(
		position.getX(),
		position.getY(),
		80,   // ancho
		120,  // alto
		bodyType::KINEMATIC
	);

	attackHitbox->listener = this;
	attackHitbox->ctype = ColliderType::PLAYERATTACK;
	hitboxActive = false;

	feetHitbox = Engine::GetInstance().physics->CreateRectangleSensor(
		position.getX(),
		position.getY() + (texH / 2),
		texW - 60,
		20,
		bodyType::KINEMATIC
	);
	feetHitbox->listener = this;
	feetHitbox->ctype = ColliderType::PLAYERFEET;

	//initialize audio effect

	//jumpfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/salto.wav");
	checkpointfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/checkpoint.wav");
	deathfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/player_death.wav");
	pickCoinFxId = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/coin-collision-sound-342335.wav");
	healfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Heal_plant.wav");

	respawnPosition = { PIXEL_TO_METERS(position.getX()), PIXEL_TO_METERS(position.getY()) };
	
	return true;
}

bool Player::Update(float dt)
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
		Engine::GetInstance().scene->beatBoss = true;
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		Engine::GetInstance().scene->beatPrincess = true;
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) {
		AddPoints(100);
	}
	bool isPaused = Engine::GetInstance().scene->isPaused || Engine::GetInstance().scene->showHelp;
	const SDL_Rect& animFrame = currentAnimSet->GetCurrentFrame();
	if (!isPaused) {
		if (animFrame.x == 160 && animFrame.y == 96 && isdead) {
			Reset();
		}
		GetPhysicsValues();
		if (isMounted && mountedBall)
		{
			int bx, by;
			mountedBall->pbody->GetPosition(bx, by);

			position.setX(bx);
			position.setY(by - mountedBall->radius - texH / 2);
			pbody->SetPosition(bx, by - mountedBall->radius - texH / 2);
			attackHitbox->SetPosition(bx, by - mountedBall->radius - texH / 2);

			HandleMountedMovement();
		}
		else
		{
			if (Engine::GetInstance().scene->someoneIsTalking == false) {
				Move();
				Jump();

				// 1. LÓGICA SUELO
				if (isWalking && isCollidedFloor && !isWallWalking) {
					stepParticleTimer += dt;

					int lookX = (int)position.getX() + (facingLeft ? 25 : -25);
					int lookY = (int)position.getY() + (texH / 2);
					int tileID = Engine::GetInstance().map->GetTileFromLayer("MetadataSuelos", lookX, lookY);

					// Determinamos si el suelo actual es de polvo/hierba (cooldown largo) o moho (cooldown corto)
					bool esSueloEspecial = Engine::GetInstance().map->IsPolvo(tileID) || Engine::GetInstance().map->IsHierba(tileID);
					float currentCooldown = esSueloEspecial ? 500.0f : 250.0f;

					if (stepParticleTimer >= currentCooldown) {
						stepParticleTimer = 0.0f;
						int spawnY = lookY - 20;

						if (Engine::GetInstance().map->IsMoho(tileID)) {
							Engine::GetInstance().map->SpawnParticle(ParticleExample::MOHO, lookX, spawnY, 0.15f);
						}
						else if (Engine::GetInstance().map->IsPolvo(tileID)) {
							Engine::GetInstance().map->SpawnParticle(ParticleExample::POLVO, lookX, spawnY, 0.15f);
						}
						else if (Engine::GetInstance().map->IsHierba(tileID)) {
							Engine::GetInstance().map->SpawnParticle(ParticleExample::HIERBAJO, lookX, spawnY + 10, 0.15f);
						}
					}
				}
				// 2. LÓGICA PARED
				else if (isWallWalking && (velocity.x != 0.0f || velocity.y != 0.0f)) {
					stepParticleTimer += dt;

					if (stepParticleTimer >= 250.0f) { // 250ms para la pared
						stepParticleTimer = 0.0f;

						int spawnX = (int)position.getX() + (facingLeft ? 25 : -25);
						int spawnY = (int)position.getY();

						Engine::GetInstance().map->SpawnParticle(ParticleExample::MOHO, spawnX, spawnY, 0.15f);
					}
				}
				// 3. SI SE PARA
				else if (!isWalking) {
					// Lo ponemos a 1000.0f para que el primer paso sea instantáneo
					stepParticleTimer = 1000.0f;
				}
				// --- FIN LÓGICA PARTÍCULAS ---
			}
			else {
				currentAnimSet->SetCurrent("idle");
			}
		}
		if (isMounted && Engine::GetInstance().input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			DismountAndLaunch();
		}
		Attack();
		HandleAttack();
		CheckKickFrame();
		SpawnCheeseBall();
		if (state == ONCHEESE && currentAnimSet->HasFinished())
		{
			state = IDLE_ON_CHEESE;
		}
		ChangeCurrentAnimation();

		
		ApplyPhysics();

		// MOSTRAR QUESO (Asegúrate de que esto quede dentro de if(!isPaused) )
		if (isShowingCheese) {
			showCheeseTimer -= dt;

			// Mantenemos la textura activa (pero sin reiniciar el fotograma)
			currentAnimSet = &animsShowCheese;
			texture = textureShowCheese;

			b2Vec2 vel = b2Body_GetLinearVelocity(pbody->body);
			vel.x = 0.0f;
			b2Body_SetLinearVelocity(pbody->body, vel);

			if (showCheeseTimer <= 0.0f) {
				isShowingCheese = false;
				lastState = DEFAULT; // <--- Esto obliga al jugador a volver al IDLE normal al terminar
			}
		}
	}
	if (Engine::GetInstance().scene->ObjectObserved == false) {

		Draw(dt);

	}

	CameraRender(dt);
	if (!isPaused) {
		if (IsProtected) {
			//check if the protection has been active for more than 10 seconds
			static Uint32 protectionStartTime = SDL_GetTicks();
			Uint32 currentTime = SDL_GetTicks();
			if (currentTime - protectionStartTime >= 10000) {
				IsProtected = false;
				protectionStartTime = currentTime; //reset timer
				LOG("Protection expired");
			}
		}
		IsPlayerProtected = IsProtected;

		//Miramos si se ha clicado el boton para crear la fireball
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
			ThrowFireBall(Side::RIGHT);
			LOG("Created Fireball");
		}
		else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
			ThrowFireBall(Side::LEFT);
			LOG("Created Fireball");
		}
		UpdateFireballs(dt);
	}

	if (pendingDismount && mountedBall)
	{
		pendingDismount = false;

		int bx, by;
		mountedBall->pbody->GetPosition(bx, by);

		mountedBall->ismounted = false;
		mountedBall = nullptr;
		isMounted = false;

		SetPosition(Vector2D(bx, by - texH));

		Engine::GetInstance().physics->SetLinearVelocity(pbody, { 0, 0 });

		if (doVerticalJump)
		{
			doVerticalJump = false;

			b2Body_SetAwake(pbody->body, true);
			b2Body_SetLinearVelocity(pbody->body, { 0.0f, -35.0f });

			state = JUMPING;
			isJumping = true;
		}
	}

	// MOSTRAR QUESO
	if (isShowingCheese) {
		showCheeseTimer -= dt;

		// Forzamos la animación Y LA TEXTURA
		currentAnimSet = &animsShowCheese;
		texture = textureShowCheese; // <--- ¡ESTA ES LA LÍNEA MÁGICA QUE FALTABA!
		currentAnimSet->SetCurrent("show_cheese");

		// Frenamos al jugador constantemente por si estaba cayendo/resbalando
		b2Vec2 vel = b2Body_GetLinearVelocity(pbody->body);
		vel.x = 0.0f;
		b2Body_SetLinearVelocity(pbody->body, vel);

		// Si el temporizador llega a 0, devolvemos el control
		if (showCheeseTimer <= 0.0f) {
			isShowingCheese = false;
		}
	}

	return true;
}

void Player::UpdateFireballs(float dt) {

	//for (auto it = fireballs.begin(); it != fireballs.end(); ) {

	//	if ((*it)->toDelete) { //si se tiene que borrar la destruye
	//		it = fireballs.erase(it);
	//	}
	//	else {
	//		(*it)->Update(dt);
	//		++it;
	//	}
	//}
}

// =====================
// MOVIMIENTO Y FISICAS
// =====================

void Player::GetPhysicsValues() {
	if (pbody != nullptr)
	{
		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);
	}

	// Read current velocity
	velocity = Engine::GetInstance().physics->GetLinearVelocity(pbody);
	if (!godMode) { velocity = { 0, velocity.y }; }
	else { velocity = { 0, 0 }; }
}

void Player::Move() {
	/*if (isdead || Engine::GetInstance().scene->IsGamePaused())
		return;*/
	if (isAttacking && isCollidedFloor) return;
	isWalking = false;

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && isOnSpecialWall)
	{
		isWallWalking = true;
		isJumping = false;
	}
	else {
		isWallWalking = false;
	}

	if (isWallWalking)
	{
		velocity.y = 0;
		velocity.x = 0;

		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			velocity.y = -speed;

		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			velocity.y = speed;

		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			velocity.x = -speed;

		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			velocity.x = speed;

	}

	// =====================
	// INPUT HORIZONTAL
	// =====================
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		velocity.x = -speed;
		isWalking = true;
		facingLeft = true;
	}
	else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		velocity.x = speed;
		isWalking = true;
		facingLeft = false;
	}
	else
	{
		velocity.x = 0;
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_U) == KEY_REPEAT)
	{
		Engine::GetInstance().render->SetZoomSmooth(0.7f, 600.0f);
	}
	// =====================
	// GOD MODE (VERTICAL)
	// =====================
	if (godMode)
	{/*
		Engine::GetInstance().scene->lives = Engine::GetInstance().scene->maxLives;*/
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			velocity.y = -godmodeSpeed;
			isWalking = true;
		}
		else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			velocity.y = godmodeSpeed;
			isWalking = true;
		}
		else
		{
			velocity.y = 0;
		}
	}

	// =====================
	// STATE MACHINE
	// =====================
	if (state == ONCHEESE) {
		return;
	}
	// PRIORIDAD 1: aire
	if (!isCollidedFloor)
	{
		state = JUMPING;

	}
	else
	{
		isJumping = false;
		// SOBRE QUESO
		if (isMounted)
		{
			return; // aquí sí puedes cortar si quieres
		}
		// SUELO NORMAL
		else
		{
			if (isWalking)
				state = RUNNING;
			else
				state = IDLE;
		}
	}

	// =====================
	// SONIDO PASOS
	// =====================
	if (isWalking && isCollidedFloor)
	{
		int randNum = rand() % 4;
		switch (randNum) {
		case 0:
			movefx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Jester/Jester_paso1.wav");
			break;
		case 1:
			movefx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Jester/Jester_paso2.wav");
			break;
		case 2:
			movefx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Jester/Jester_paso3.wav");
			break;
		case 3:
			movefx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Jester/Jester_paso4.wav");
			break;
		}

		int currentTime = (int)SDL_GetTicks();
		if (currentTime - lastStepTime > 600)
		{
			Engine::GetInstance().audio->PlayFx(movefx);
			lastStepTime = currentTime;
		}
	}
	else
	{
		lastStepTime = 0;
	}
}

void Player::Jump() {
	// This function can be used for more complex jump logic if needed
	bool spacePressed = Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	//LOG("Space pressed");

	bool canFirstJump = spacePressed && !isJumping && isCollidedFloor && !godMode; // seguido lo que propuso el profesor, añadido canFirstJump y Can second jump para acortar codigo
	bool canSecondJump = spacePressed && isJumping && !secondJump && !godMode;
	/*if ((Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false && !godMode && (isCollidedFloor)|| Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping && !secondJump && !godMode)) {
		Engine::GetInstance().audio->PlayFx(jumpfx);
		anims.SetCurrent("jump");
		isJumping = true;
		if (firstJump) {

		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, 0.0f, -jumpForce, true);
			secondJump = false;
		}
		else { secondJump = true;

		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, 0.0f, -jumpForce+0.1f, true);
		}
		firstJump = false;

	}
	*/

	if (spacePressed && canFirstJump) {
		//Engine::GetInstance().audio->PlayFx(jumpfx);
		if (IsProtected) {
			state = JUMPING;

		}
		else {
			state = JUMPING;

		}
		isJumping = true;
		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, 0.0f, -jumpForce, true);
		LOG("Player jumped action");
		secondJump = false;
		firstJump = false;
	}
	/*else if(spacePressed && canSecondJump){
		Engine::GetInstance().audio->PlayFx(jumpfx);
		if(IsProtected){
			currentAnimSet.SetCurrent("jump");
		}
		else {
			currentAnimSet.SetCurrent("jump");
		}
		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(pbody, 0.0f, -jumpForce + 0.1f, true);
		secondJump = true;
	}*/

}

void Player::ApplyPhysics() {
	if (isMounted) return;
	
	// Preserve vertical speed while jumping
	if (isJumping == true) {
		velocity.y = Engine::GetInstance().physics->GetYVelocity(pbody);

	}

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);
	UpdateAttackHitbox();
}

// =====================
// RENDER Y CAMARA
// =====================

void Player::Draw(float dt) {



	currentAnimSet->Update(dt);
	const SDL_Rect& animFrame = currentAnimSet->GetCurrentFrame();

	int x, y;

	if (isMounted && mountedBall)
	{
		x = (int)position.getX();
		y = (int)position.getY();
	}
	else
	{
		pbody->GetPosition(x, y);
	}

	SDL_FlipMode flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

	int drawX = x - animFrame.w / 2;
	int drawY = y - animFrame.h / 2;

	float rotation = 0.0f;

	if (isWallWalking)
	{
		rotation = 90.0f;
	}

	Engine::GetInstance().render->DrawTexture(
		texture,
		drawX,
		drawY,
		&animFrame,
		1.0f,
		rotation,
		INT_MAX,
		INT_MAX,
		flip
	);

}

void Player::CameraRender(float dt) {

	auto& engine = Engine::GetInstance();
	auto render = engine.render;

	Vector2D mapSize = engine.map->GetMapSizeInPixels();

	int px, py;
	pbody->GetPosition(px, py);

	int rw, rh;
	SDL_GetRenderOutputSize(render->renderer, &rw, &rh);

	float camW = (float)render->camera.w;
	float camH = (float)render->camera.h;

	float centerX = camW * 0.5f / render->zoom;
	float centerY = camH * 0.5f / render->zoom;

	float targetX = -px + centerX;
	float targetY = -py + centerY;

	float visibleW = camW / render->zoom;
	float visibleH = camH / render->zoom;

	float minX = -(mapSize.getX() - visibleW);
	float maxX = 0.0f;

	float minY = -(mapSize.getY() - visibleH);
	float maxY = 0.0f;

	targetX = std::clamp(targetX, minX, maxX);
	targetY = std::clamp(targetY, minY, maxY);

	render->camera.x = targetX;
	render->camera.y = targetY;


}

// =====================
// COLISIONES
// =====================

void Player::OnCollision(PhysBody* physA, PhysBody* physB)
{
	ColliderType other = physB->ctype;

	// =========================
		// 0. COLISIÓN DE LOS PIES
		// =========================
	if (physA == feetHitbox)
	{
		if (other == ColliderType::PLATFORM)
		{
			// Comprobamos si venimos del aire antes de actualizar los booleanos
			bool justLanded = !isCollidedFloor;

			floorContacts++;
			isJumping = false;
			firstJump = true;
			isCollidedFloor = true;

			// --- INICIO PARTICULAS DE ATERRIZAJE ---
			if (justLanded && !isWallWalking) {

				int bx, by;
				pbody->GetPosition(bx, by);
				int lookX = bx;
				int lookY = by + (texH / 2) + 10;

				int tileID = Engine::GetInstance().map->GetTileFromLayer("MetadataSuelos", lookX, lookY);

				int idMoho = 1;
				int idPolvo = 2;
				int idHierbajo = 3;

				// 1. Suelo de Polvo / Tierra
				if (tileID == idPolvo) {
					Engine::GetInstance().map->SpawnParticle(ParticleExample::POLVO_SALTO, lookX - 35, lookY - 30, 0.15f);
					Engine::GetInstance().map->SpawnParticle(ParticleExample::POLVO_SALTO, lookX + 35, lookY - 30, 0.15f);
				}
				// 2. Suelo de Césped
				else if (tileID == idHierbajo) {
					Engine::GetInstance().map->SpawnParticle(ParticleExample::HIERBAJO_SALTO, lookX - 30, lookY - 20, 0.15f);
					Engine::GetInstance().map->SpawnParticle(ParticleExample::HIERBAJO_SALTO, lookX + 30, lookY - 20, 0.15f);
				}
				// 3. Suelo de Moho
				else if (tileID == idMoho) {
					// Si creas el estilo MOHO_SALTO úsalo aquí. De momento usa el normal.
					Engine::GetInstance().map->SpawnParticle(ParticleExample::MOHO_SALTO, lookX - 30, lookY - 30, 0.15f);
					Engine::GetInstance().map->SpawnParticle(ParticleExample::MOHO_SALTO, lookX + 30, lookY - 30, 0.15f);
				}
				// 4. SUELO POR DEFECTO (Piedra, vacío, etc.)
				else {
					// Impacto de polvo potente siempre que aterrices en suelo sin metadato
					Engine::GetInstance().map->SpawnParticle(ParticleExample::POLVO_SALTO, lookX - 35, lookY - 30, 0.15f);
					Engine::GetInstance().map->SpawnParticle(ParticleExample::POLVO_SALTO, lookX + 35, lookY - 30, 0.15f);
				}
			}
			// --- FIN PARTICULAS DE ATERRIZAJE ---

			if (isMounted) {
				//DismountAndLaunch();
			}
		}

		return;
	}


	// =========================
	// 2. COLISIONES DEL PLAYER (CUERPO PRINCIPAL)
	// =========================
	switch (other)
	{
	case ColliderType::PLATFORM:
	{
		break;
	}

	case ColliderType::PARED:
	{
		isCollidedWall = true;
		if (isMounted) {
			DismountAndLaunch();
		}
		break;
	}

	case ColliderType::COIN:
	{
		Engine::GetInstance().audio->PlayFx(pickCoinFxId);
		physB->listener->Destroy();
		AddPoints(10);
		break;
	}
	case ColliderType::MOHOWALL:
		isOnSpecialWall = true;
		break;
	case ColliderType::SAVE:
	{
		// EL CHECKPOINT YA GESTIONA SU AUDIO
		// 
		// if (physB->objectName != "Player")
		// 	Engine::GetInstance().audio->PlayFx(checkpointfx); 

		if (pbody && !B2_IS_NULL(pbody->body))
			respawnPosition = b2Body_GetPosition(pbody->body);
		Engine::GetInstance().scene->SaveLevel();
		Engine::GetInstance().gameManager->SaveGame();
		hasHealed = false;
		LOG("Checkpoint reached.");
		break;
	}
	case ColliderType::ARENA_TRIGGER:
	{
		Engine::GetInstance().scene->StartMiniBoss();
		b2Body_Disable(physB->body);
		break;
	}
	case ColliderType::DANGER:
	case ColliderType::ENEMY:
	case ColliderType::FIREBALL:
	case ColliderType::FINALBOSS:
	{
		if (godMode) break;
		if (IsProtected) break;

		if (!isdead)
		{
			Engine::GetInstance().audio->PlayFx(deathfx);
			isdead = true;

			if (extralife) {
				extralife = false;
				break;
			}

			Engine::GetInstance().scene->lives--;

			if (Engine::GetInstance().scene->lives <= 0)
			{
				isDeadDefinitive = true;
			}

			currentAnimSet->SetCurrent("jump");
		}
		break;
	}

	case ColliderType::PROTECTION:
	{
		IsProtected = true;
		break;
	}

	case ColliderType::EXTRALIVE:
	{
		// Dejamos este case vacío porque ExtraLive.cpp ahora se encarga de 
		// dar la vida y reproducir el sonido según sus animaciones.
		break;
	}

	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	// =========================
	// SALIDA DE LOS PIES DEL SUELO
	// =========================
	if (physA == feetHitbox && physB->ctype == ColliderType::PLATFORM)
	{
		isCollidedFloor = false;
		return;
	}

	// =========================
	// SALIDAS DEL CUERPO PRINCIPAL
	// =========================
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		break;
	case ColliderType::ITEM:
		LOG("End Collision ITEM");
		break;
	case ColliderType::DANGER:
		LOG("End Collision DANGER");
		break;
	case ColliderType::PARED:
		LOG("End Collision PARED");
		isCollidedWall = false;
		break;
	case ColliderType::SAVE:
		LOG("End Collision SAVE");
		break;
	case ColliderType::FIREBALL:
		break;
	case ColliderType::MOHOWALL:
		isOnSpecialWall = false;
		isWallWalking = false;
		break;
	default:
		break;
	}
}

// =====================
// UTILIDADES
// =====================

Vector2D Player::GetPosition() {
	int x, y;
	pbody->GetPosition(x, y);
	// Adjust for center
	return Vector2D((float)x - texW / 2, (float)y - texH / 2);
}

bool Player::CleanUp()
{
	LOG("Cleanup player");

	if (pbody != nullptr) {
		pbody->listener = nullptr;
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}

	if (attackHitbox != nullptr) {
		attackHitbox->listener = nullptr;
		Engine::GetInstance().physics->DeletePhysBody(attackHitbox);
		attackHitbox = nullptr;
	}

	if (feetHitbox != nullptr) {
		feetHitbox->listener = nullptr;
		Engine::GetInstance().physics->DeletePhysBody(feetHitbox);
		feetHitbox = nullptr;
	}

	Engine::GetInstance().textures->UnLoad(texture);

	return true;
}

void Player::SetPosition(Vector2D pos)
{

	this->position = pos;

	if (pbody != nullptr && !B2_IS_NULL(pbody->body)) {
		int centerX = (int)pos.getX() + texW / 2;
		int centerY = (int)pos.getY() + texH / 2;
		pbody->SetPosition(centerX, centerY);
	}
	else {
		LOG("Warning: pbody is null when setting position");
	}
}

void Player::Reset()
{

	b2Vec2 initialPos = respawnPosition;
	b2Rot rotation = b2MakeRot(0.0f);
	b2Body_SetTransform(pbody->body, initialPos, rotation);
	Engine::GetInstance().physics->SetLinearVelocity(pbody, { 0.0f, 0.0f });
	isdead = false;
	isJumping = false;
	isCollidedFloor = true;
	firstJump = true;

	float limitUp = Engine::GetInstance().render->camera.h / 4;
	Engine::GetInstance().render->camera.y = limitUp;
	IsProtected = false;
	currentAnimSet->SetCurrent("jump");

}

bool Player::isDead()
{
	return isdead;
}

void Player::AddPoints(int points)
{
	Engine::GetInstance().scene->score += points;
	LOG("Score: %d", Engine::GetInstance().scene->score);
}

bool Player::isPlayerProtectedquestion() {
	return IsPlayerProtected;
}

void Player::ThrowFireBall(Side side) {

	/*std::shared_ptr<FireBall> fireball =std::dynamic_pointer_cast<FireBall>(Engine::GetInstance().entityManager->CreateEntity(EntityType::FIREBALL));
	fireball->spawnPos = Vector2D(position.getX(), position.getY());
	fireball->spawnSide = side;

	fireball->Start();
	fireballs.push_back(fireball);*/
}

void Player::ChangeCurrentAnimation() {

	if (isKicking) return;
	if (state == ATTACKING) return;
	if (state == lastState) return;

	lastState = state;

	switch (state)
	{
	case JUMPING:
		currentAnimSet = &anims2x3;
		texture = texture2x3;
		currentAnimSet->SetCurrent("jump");
		break;

	case RUNNING:
		currentAnimSet = &anims2x3;
		texture = texture2x3;
		currentAnimSet->SetCurrent("run");
		break;

	case ONCHEESE:
		currentAnimSet = &anims2x3;
		texture = texture2x3;
		currentAnimSet->SetCurrent("hoponcheese");
		break;
	case IDLE:
		currentAnimSet = &anims3x3;
		texture = texture3x3;
		currentAnimSet->SetCurrent("idle");
		break;

	case IDLE_ON_CHEESE:
		currentAnimSet = &anims3x3;
		texture = texture3x3;
		currentAnimSet->SetCurrent("idleOnCheese");
		break;
	case RUNNING_ON_CHEESE:
		currentAnimSet = &anims4x4;
		texture = texture4x4;
		currentAnimSet->SetCurrent("ballroll");
		break;
	default:
		break;
	}
}

// =====================
// ATAQUES
// =====================

void Player::Attack() {
	static bool wasPressedLastFrame = false;

	bool isPressed = Engine::GetInstance().input->GetMouseButtonDown(1);

	// Detectar flanco (click real)
	if (isPressed && !wasPressedLastFrame) {
		attackRequested = true;
		LOG("Attack requested (single click)");
	}

	wasPressedLastFrame = isPressed;
}

void Player::HandleAttack()
{
	const float comboResetTimeMs = 400;
	static Uint32 lastAttackTime = 0;
	Uint32 now = SDL_GetTicks();

	if (attackRequested) {
		attackRequested = false;
		if (!isAttacking) {
			// INCREMENTAR EL ID: Cada vez que el usuario inicia un ataque nuevo,
			// el identificador cambia, haciendo que los enemigos vuelvan a ser vulnerables.
			currentAttackId++;

			attackCombo = (!isCollidedFloor) ? 3 : 1;
			StartAttack(attackCombo);
		}
		else {
			bufferedAttack = true;
		}
	}

	if (isAttacking) {
		int currentFrame = currentAnimSet->GetCurrentFrameIndex();

		// ACTIVACIÓN DEL HITBOX (Frames activos)
		if (currentFrame >= 1 && currentFrame <= 5) {
			hitboxActive = true;
			UpdateAttackHitbox();

			int x, y;
			attackHitbox->GetPosition(x, y);

			SDL_Rect hitboxRect = { (int)x, (int)y, 80, 120 };
			std::vector<PhysBody*> hits = Engine::GetInstance().physics->QueryArea(hitboxRect);

			for (auto* body : hits) {
				if (body->ctype == ColliderType::ENEMY) {
					Enemy* e = static_cast<Enemy*>(body->listener);

					// LÓGICA DE DAÑO ÚNICO:
					// Solo daña si el ID guardado en el enemigo es distinto al ID actual.
					if (e && e->lastAttackId != currentAttackId) {
						e->DecreaseHealth(20);
						e->lastAttackId = currentAttackId; // Marcamos al enemigo con el ID actual
					}
				}
			}
		}
		else {
			hitboxActive = false;
		}

		if (currentAnimSet->HasFinished()) {
			if (bufferedAttack) {
				bufferedAttack = false;
				attackCombo = (!isCollidedFloor) ? 3 : (attackCombo >= 3 ? 1 : attackCombo + 1);
				StartAttack(attackCombo);
			}
			else {
				isAttacking = false;
				state = DEFAULT;
				// YA NO NECESITAS RESETEAR ENEMIGOS AQUÍ. 
				// El cambio de ID en el próximo clic se encarga de todo.
			}
		}
	}
}

void Player::StartAttack(int combo)
{
	isAttacking = true;
	state = ATTACKING;

	currentAnimSet = &anims3x4;
	texture = texture3x4;

	switch (combo)
	{
	case 1:
		currentAnimSet->SetCurrent("attack1");
		currentAnimSet->Resets();
		attackfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Jester/Jester_ataque1.wav");
		Engine::GetInstance().audio->PlayFx(attackfx);
		break;
	case 2:
		currentAnimSet->SetCurrent("attack2");
		currentAnimSet->Resets();
		attackfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Jester/Jester_ataque2.wav");
		Engine::GetInstance().audio->PlayFx(attackfx);
		break;
	case 3:
		currentAnimSet->SetCurrent("attack3");
		currentAnimSet->Resets();
		attackfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/Jester/Jester_ataque3.wav");
		Engine::GetInstance().audio->PlayFx(attackfx);
		break;
	}

	hitboxActive = true;

	LOG("Player attack start combo %d", combo);
}

void Player::UpdateAttackHitbox()
{
	if (!attackHitbox) return;

	int x, y;
	pbody->GetPosition(x, y);

	int offsetX = facingLeft ? -offsetAttackHitboxX : offsetAttackHitboxX;

	attackHitbox->SetPosition(
		x + offsetX,
		y + offsetAttackHitboxY
	);

	if (feetHitbox) {
		feetHitbox->SetPosition(x, y + (texH / 2));
	}
}

// =====================
// CHEESEBALL
// =====================

void Player::SpawnCheeseBall()
{
	// 1. Al pulsar C, solo activamos la espera
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_C) == KEY_DOWN && !isMounted && Engine::GetInstance().scene->cheese == true)
	{
		pendingCheeseSpawn = true;
		state = ONCHEESE; // Cambiamos estado para que ChangeCurrentAnimation ponga la de saltar al queso
	}

	// 2. Si estamos esperando a que la animación termine
	if (pendingCheeseSpawn && currentAnimSet->GetCurrentFrameIndex() == 3)
	{
		pendingCheeseSpawn = false; // Reseteamos el flag

		// --- AQUÍ VA TU LÓGICA DE CREACIÓN ---
		auto entity = Engine::GetInstance().entityManager->CreateEntity(EntityType::CHEESEBALL);
		auto cb = std::dynamic_pointer_cast<CheeseBall>(entity);

		if (cb)
		{
			cb->ismounted = true;
			int px, py;
			pbody->GetPosition(px, py);

			Vector2D spawnPos(px, py + texH / 2 + cb->radius - 200);
			cb->SetPosition(spawnPos);
			cb->Start();

			int bx, by;
			cb->pbody->GetPosition(bx, by);
			SetPosition(Vector2D(bx, by - cb->radius - texH / 2));
			cb->SetVelocityy({velocity.x, velocity.y});
			mountedBall = cb;
			isMounted = true;
			mountedBall->firstjump = true;
			cheeseTime = 300.0f;
			//Engine::GetInstance().physics->SetLinearVelocity(pbody, { 0,0 });
		}
	}
}

void Player::HandleMountedMovement()
{
	if (!isMounted || !mountedBall)
	{
		ResetCheeseState();
		return;
	}

	if (mountedBall->launch == true) {
		DismountAndLaunch();
		return;
	}
	cheeseTime--;

	if (waitingAttackAnimEnd)
	{
		int currentFrame = currentAnimSet->GetCurrentFrameIndex();
		if (currentFrame >= jumpTriggerOffset)
		{
			waitingAttackAnimEnd = false;
			currentAnimSet->SetSpeed(1.0f);
			pendingDismount = true;
			doVerticalJump = true;
		}
	}

	b2Vec2 vel = b2Body_GetLinearVelocity(mountedBall->pbody->body);
	movingBall = false;
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

		if (!facingLeft) {
			cheeseSpeed = 13.0f;
			cheeseTime = 300.0f;
		}
		if (cheeseTime < 200.0f && cheeseTime >= 100.0f && !isKicking) {
			vel.x = -cheeseSpeed * 2.0f;
		}
		else if (cheeseTime < 100.0f && !isKicking) {
			vel.x = -cheeseSpeed * 3.5f;
		}
		else {

			vel.x = -cheeseSpeed;

		}
		movingBall = true;
		facingLeft = true;
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		if (facingLeft) {
			cheeseSpeed = 10.0f;
			cheeseTime = 300.0f;
		}
		if (cheeseTime < 200.0f && cheeseTime >= 100.0f && !isKicking) {
			vel.x = cheeseSpeed * 2.0f;
			mountedBall->canSmash = false;
		}
		else if (cheeseTime < 100.0f && !isKicking) {
			vel.x = cheeseSpeed * 3.5f;
			mountedBall->canSmash = true;
		}
		else {

			vel.x = cheeseSpeed;
			mountedBall->canSmash = false;

		}
		movingBall = true;
		facingLeft = false;

	}
	if (state == ONCHEESE) {
		return;
	}
	if (movingBall)
		state = RUNNING_ON_CHEESE;
	else
		state = IDLE_ON_CHEESE;

	if (isKicking) {
		vel.y = 0.0f;
	}
	mountedBall->SetVelocityy(vel);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// =========================
		// PRIMER SALTO
		// =========================
		if (mountedBall->firstjump)
		{
			Engine::GetInstance().physics->ApplyLinearImpulseToCenter(
				mountedBall->pbody,
				0.0f,
				-jumpForce,
				true
			);

			mountedBall->firstjump = false;
		}
		// =========================
		// DOBLE SALTO
		// =========================
		else
		{
			waitingAttackAnimEnd = true;

			// forzar animación attack3
			currentAnimSet = &anims3x4;
			texture = texture3x4;
			currentAnimSet->SetCurrent("attack3");
			currentAnimSet->SetSpeed(2.0f);
			currentAnimSet->Resets();

			return;
		}
	}
}

void Player::DismountAndLaunch()
{
	if (!isMounted || !mountedBall || isKicking) return;

	isKicking = true;

	// Cambiamos a la animación de chute
	currentAnimSet = &anims5x5;
	texture = texture5x5;
	currentAnimSet->SetCurrent("ballkick"); // Asegúrate de tener este alias en tus anims
	currentAnimSet->Resets();

	
}

void Player::DismountVerticalJump()
{
	if (!isMounted || !mountedBall) return;

	// =========================
	// DATOS DE LA BOLA
	// =========================

	int bx, by;
	mountedBall->pbody->GetPosition(bx, by);

	// =========================
	// HACER CAER LA BOLA VERTICAL
	// =========================

	b2Vec2 ballVel = b2Body_GetLinearVelocity(mountedBall->pbody->body);

	ballVel.x = 0.0f;
	ballVel.y = 50.0f;

	b2Body_SetLinearVelocity(mountedBall->pbody->body, ballVel);

	mountedBall->ismounted = false;
	mountedBall->canSmash = false;

	// =========================
	// DESMONTAR ANTES DEL IMPULSO
	// =========================

	mountedBall = nullptr;
	isMounted = false;

	// =========================
	// POSICIONAR PLAYER ARRIBA
	// =========================

	SetPosition(Vector2D(
		bx,
		by - texH
	));

	// =========================
	// RESET VELOCIDAD PLAYER
	// =========================

	Engine::GetInstance().physics->SetLinearVelocity(
		pbody,
		{ 0.0f, 0.0f }
	);

	// =========================
	// IMPULSO HACIA ARRIBA
	// =========================

	b2Body_SetAwake(pbody->body, true);

	b2Vec2 playerVel = { 0.0f, -18.0f };

	b2Body_SetLinearVelocity(pbody->body, playerVel);

	state = JUMPING;

	cheeseSpeed = 10.0f;
	ResetCheeseState();
}

void Player::ResetCheeseState()
{
	waitingAttackAnimEnd = false;
	pendingDismount = false;
	doVerticalJump = false;
	if (mountedBall) {
		mountedBall->firstjump = false;
	}
}

void Player::CheckKickFrame()
{
	if (!isKicking) return;

	// Comprobamos si hemos llegado al frame 5
	if (currentAnimSet->GetCurrentFrameIndex() == 8 && !mountedBall == NULL)
	{
		
		// --- 1. INICIO DEL CHUTE (Frame 0) ---
		if (currentAnimSet->GetCurrentFrameIndex() == 0)
		{
			b2Body_SetAwake(pbody->body, true);
			// Cambiamos la escala de gravedad a 0 para que flote
			b2Body_SetGravityScale(pbody->body, 0.0f);
			// Eliminamos velocidad vertical para que no siga subiendo/cayendo
			b2Vec2 vel = b2Body_GetLinearVelocity(pbody->body);
			vel.y = 0.0f;
			b2Body_SetLinearVelocity(pbody->body, vel);
		}

		// --- LÓGICA DE LANZAMIENTO ORIGINAL ---
		mountedBall->canSmash = false;
		mountedBall->StartLifespan();

		int x, y;
		mountedBall->pbody->GetPosition(x, y);
		SetPosition(Vector2D(x, y - mountedBall->radius - texH));

		float forceX = facingLeft ? -2000.0f : 2000.0f;
		float forceY = -6.0f;

		Engine::GetInstance().physics->ApplyLinearImpulseToCenter(mountedBall->pbody, forceX, forceY, true);

		mountedBall->ismounted = false;
		mountedBall = nullptr;
		isMounted = false;
		state = DEFAULT;
		ResetCheeseState();

		LOG("Bola chutada en el frame 5");
	}

	if (currentAnimSet->HasFinished()) {
		isKicking = false;

		// RESTAURAR GRAVEDAD: 2.25f es el valor que usas en Player::Start()
		b2Body_SetGravityScale(pbody->body, 2.25f);

		// Limpieza de punteros
		mountedBall = nullptr;
		isMounted = false;
		state = DEFAULT;
		ResetCheeseState();
	}
}

void Player::PlayShowCheese()
{
	isShowingCheese = true;
	showCheeseTimer = 2000.0f;

	b2Vec2 vel = b2Body_GetLinearVelocity(pbody->body);
	vel.x = 0.0f;
	b2Body_SetLinearVelocity(pbody->body, vel);

	// ASIGNAMOS LA ANIMACIÓN AQUÍ, SOLO UNA VEZ PARA QUE NO SE CONGELE
	currentAnimSet = &animsShowCheese;
	texture = textureShowCheese;
	currentAnimSet->SetCurrent("show_cheese");
}