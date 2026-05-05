
// =====================
// ENEMY BASE IMPLEMENTATION
// =====================

#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Map.h"

// =====================
// CONSTRUCTOR / DESTRUCTOR
// =====================

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name = "Enemy";
}

Enemy::~Enemy() {

}

// =====================
// CICLO DE VIDA
// =====================

bool Enemy::Awake() {
	return true;
}

bool Enemy::Start() {

	// load
	/*std::unordered_map<int, std::string> aliases = { {0,"idle"} };*/
	/*anims.LoadFromTSX("Assets/Textures/enemy_Spritesheet.tsx", aliases);
	anims.SetCurrent("idle");*/

	//Initialize Player parameters
	texture = Engine::GetInstance().textures->Load(texName);

	/*anims.LoadFromTSX(spriteSheetName, aliases);*/
	//Add physics to the enemy - initialize physics body
	pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.0f);

	//Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	//ssign collider type
	pbody->ctype = ColliderType::ENEMY;

	// Initialize pathfinding
	pathfinding = std::make_shared<Pathfinding>();
	//Get the position of the enemy
	Vector2D pos = GetPosition();
	//Convert to tile coordinates
	Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());
	//Reset pathfinding
	pathfinding->ResetPath(tilePos);

	return true;
}

bool Enemy::Update(float dt)
{
	repathTimer++;

	if (isKnockback) {
		GetPhysicsValues();
	}
	distanceToPlayer = CalculateDistance();

	if (distanceToPlayer < detectionRange) {

		if (isKnockback) {
			knockbackTimer--;

			if (knockbackTimer <= 0) {
				isKnockback = false;
			}
		}
		if (distanceToPlayer < attackRange) {
			SetState(EnemyState::ATTACKING);
		}
		else if (velocity.x != 0) {
			SetState(EnemyState::WALKING);
		}
		else {
			SetState(EnemyState::IDLE);
		}
		
	}
	else {
		
		Vector2D enemyPos = GetPosition();
		Vector2D enemyTilePos = Engine::GetInstance().map->WorldToMap(enemyPos.getX(), enemyPos.getY());
		ResetPathfinding(enemyTilePos);
	}
	
	ApplyPhysics();
	Draw(dt);

	return true;
}

void Enemy::PerformPathfinding() {

	Vector2D currentPlayerTile = Engine::GetInstance().scene->GetPlayerPosition();

	if ((currentPlayerTile.getX() != lastPlayerTile.getX() ||
		currentPlayerTile.getY() != lastPlayerTile.getY()) && repathTimer >= repathDelay) {

		int ex, ey;
		pbody->GetPosition(ex, ey);
		Vector2D enemyTilePos = Engine::GetInstance().map->WorldToMap(ex, ey);

		pathfinding->ResetPath(enemyTilePos);

		repathTimer = 0;
		lastPlayerTile = currentPlayerTile;
	}

	pathfinding->PropagateAStar(SQUARED);

}

void Enemy::GetPhysicsValues() {
	// Read current velocity
	velocity = Engine::GetInstance().physics->GetLinearVelocity(pbody);
	
}

// =====================
// MOVIMIENTO Y FISICAS
// =====================

void Enemy::Move() {
	if (isKnockback) return;

	switch (type)
	{
	case EnemyType::GROUND:
		MoveGround();
		break;

	case EnemyType::AIR:
		MoveAir();
		break;

	case EnemyType::RANGED:
		MoveRanged();
		break;

	case EnemyType::MELEE:
		MoveMelee();
		break;

	default:
		MoveGround();
		break;
	}
}

void Enemy::ApplyPhysics() {

	// Apply velocity via helper
	Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);
	if (attackHitbox != nullptr) {
		attackHitbox->SetPosition(GetPosition().getX() - offsetAttackHitboxX, GetPosition().getY() - offsetAttackHitboxY);
	}
}

// =====================
// RENDER Y CAMARA
// =====================

void Enemy::Draw(float dt) {

	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	//// Update render position using your PhysBody helper
	int x, y;
	if (pbody != nullptr) {
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);
	}
	else {
		x = (int)deathPosition.getX();
		y = (int)deathPosition.getY();
	}

	// Draw pathfinding debug
	//pathfinding->DrawPath();

	SDL_Rect sect = {0,0,texW,texH};
	//Draw the player using the texture and the current animation frame
	/*Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &sect, 1, 0, 0, 0, SDL_FLIP_NONE);*/

	SDL_FlipMode flip = facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame, 1.0f, 0.0, INT_MAX, INT_MAX, flip);
}

bool Enemy::CleanUp()
{
	LOG("Cleanup enemy");

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

	Engine::GetInstance().textures->UnLoad(texture);

	return true;
}

// =====================
// COLISIONES
// =====================

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{

}

void Enemy::CreateAttackHitbox(int x, int y, int w, int h) {
	attackHitbox = Engine::GetInstance().physics->CreateRectangleSensor(x, y, w, h, bodyType::KINEMATIC);
	attackHitbox->ctype = ColliderType::ENEMY_ATTACK;
	attackHitbox->listener = this;
}

// =====================
// UTILIDADES
// =====================

float Enemy::CalculateDistance() {
	
    int ex, ey;
    pbody->GetPosition(ex, ey); 
	Vector2D enemyWorld = Engine::GetInstance().map->WorldToMap(ex, ey);
   
    Vector2D pTile = Engine::GetInstance().scene->GetPlayerPosition();
    Vector2D pWorld = Engine::GetInstance().map->WorldToMap((int)pTile.getX(), (int)pTile.getY());
    
    float dx = (float)enemyWorld.getX() - pWorld.getX();
    float dy = (float)enemyWorld.getY() - pWorld.getY();
	
    float distance = sqrtf((dx * dx) + (dy * dy));

    return (distance);
}

void Enemy::SetPosition(Vector2D pos) {
	pbody->SetPosition((int)(pos.getX()), (int)(pos.getY()));
}

Vector2D Enemy::GetPosition() {
	if (pbody != nullptr) {
		int x, y;
		pbody->GetPosition(x, y);
		return Vector2D((float)x - texW / 2, (float)y - texH / 2);
	}
	else {
		return deathPosition;
	}
}

void Enemy::ResetPathfinding(Vector2D pos) {
	pathfinding->ResetPath(pos);
}

// =====================
// ATAQUES
// =====================

void Enemy::Attack() {

	SetState(EnemyState::ATTACKING);
}

void Enemy::DecreaseHealth(int amount) {
	health -= amount;

	SetState(EnemyState::HIT);

	if (pbody != nullptr) {

		b2Vec2 vel = Engine::GetInstance().physics->GetLinearVelocity(pbody);

		float dir = facingLeft ? 1.0f : -1.0f;

		vel.x = dir * knockbackForce;
		vel.y = -20.0f; // pequeño salto

		Engine::GetInstance().physics->SetLinearVelocity(pbody, vel);

		isKnockback = true;
		knockbackTimer = knockbackDuration;
	}

	if (health <= 0) {
		SetState(EnemyState::DYING);
		Die();
	}
}
void Enemy::Die() {
	
	toDelete = true;
}

void Enemy::SetState(EnemyState newState)
{
	if (state == newState) return;

	lastState = state;
	state = newState;

	ChangeCurrentAnimation();
}

void Enemy::ChangeCurrentAnimation()
{
	if (state == lastState) return;

	switch (state)
	{
	case EnemyState::IDLE:
		anims.SetCurrent("idle");
		break;

	case EnemyState::WALKING:
		anims.SetCurrent("walk");
		break;

	case EnemyState::RUNNING:
		anims.SetCurrent("run");
		break;

	case EnemyState::JUMPING:
		anims.SetCurrent("jump");
		break;

	case EnemyState::FALLING:
		anims.SetCurrent("fall");
		break;

	case EnemyState::ATTACKING:
		anims.SetCurrent("attack");
		break;

	case EnemyState::HIT:
		anims.SetCurrent("hit");
		break;

	case EnemyState::DYING:
		anims.SetCurrent("death");
		break;
	}
}

void Enemy::MoveGround() {
	// tu lógica actual
	if (isKnockback) return; // Si no hay camino o ya llegamos, detenemos el movimiento horizontal 
	if (pathfinding->pathTiles.empty()) { return; } // Obtenemos el siguiente paso del path (el último elemento añadido a la lista) 
	Vector2D nextTile = pathfinding->pathTiles.front(); 
	Vector2D nextPosWorld = Engine::GetInstance().map->MapToWorld((int)nextTile.getX(), (int)nextTile.getY()); 
	// Posición actual del enemigo 
	int ex, ey; 
	pbody->GetPosition(ex, ey); 
	float speed = 5.0f; 
	// Ajusta según la escala de tu mundo Box2D 
	// Lógica de movimiento horizontal hacia el siguiente tile 
	float threshold = 5.0f; 
	if (ex < nextPosWorld.getX() - threshold) velocity.x = speed; 
	else if (ex > nextPosWorld.getX() + threshold) velocity.x = -speed; 
	else velocity.x = 0; if (velocity.x < 0) facingLeft = true; 
	else if (velocity.x > 0) facingLeft = false; // Move

	if (velocity.y < -0.1f) {
		SetState(EnemyState::JUMPING);
	}
	else if (velocity.y > 0.1f) {
		SetState(EnemyState::FALLING);
	}
}

void Enemy::MoveAir() {
	// ignora gravedad, mueve en Y también
}

void Enemy::MoveRanged() {
	// mantener distancia del player
}

void Enemy::MoveMelee()
{
	// tu lógica actual
	if (isKnockback) return; // Si no hay camino o ya llegamos, detenemos el movimiento horizontal 
	if (pathfinding->pathTiles.empty()) { return; } // Obtenemos el siguiente paso del path (el último elemento añadido a la lista) 
	Vector2D nextTile = pathfinding->pathTiles.front();
	Vector2D nextPosWorld = Engine::GetInstance().map->MapToWorld((int)nextTile.getX(), (int)nextTile.getY());
	// Posición actual del enemigo 
	int ex, ey;
	pbody->GetPosition(ex, ey);
	// Ajusta según la escala de tu mundo Box2D 
	// Lógica de movimiento horizontal hacia el siguiente tile 
	float threshold = 5.0f;
	if (ex < nextPosWorld.getX() - threshold) velocity.x = speed;
	else if (ex > nextPosWorld.getX() + threshold) { 
		velocity.x = -speed; 
	}else velocity.x = 0; 
	
	if (velocity.x < 0) facingLeft = true;
	else if (velocity.x > 0) facingLeft = false; // Move

	if (velocity.y < -0.1f) {
		SetState(EnemyState::JUMPING);
	}
	else if (velocity.y > 0.1f) {
		SetState(EnemyState::FALLING);
	}
	
}