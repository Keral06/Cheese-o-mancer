#pragma once

#include "Entity.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"

struct SDL_Texture;

enum class EnemyType {
	GROUND,
	AIR,
	BOSS,
	RANGED,
	MELEE
};

enum class EnemyState {
	IDLE,
	WALKING,
	RUNNING,
	JUMPING,
	FALLING,
	ATTACKING,
	HIT,
	DYING
};

class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();
	bool Awake();
	virtual bool Start();
	virtual bool Update(float dt);
	bool CleanUp();
	virtual void OnCollision(PhysBody* physA, PhysBody* physB);
	virtual void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	void CreateAttackHitbox(int x, int y, int w, int h);
	
	float CalculateDistance();
	void SetPosition(Vector2D pos);
	Vector2D GetPosition();

	void ResetPathfinding(Vector2D pos);

	virtual void Attack();

	void DecreaseHealth(int amount);
	void Die();

	void SetState(EnemyState newState);

	void ChangeCurrentAnimation();

	void MoveGround();

	void MoveAir();

	void MoveRanged();

	void MoveMelee();

protected:
	void PerformPathfinding();
	void GetPhysicsValues();
	virtual void Move();
	void ApplyPhysics();
	virtual void Draw(float dt);

public:

	//Declare enemy parameters
	float speed = 20.0f;
	SDL_Texture* texture = NULL;
	int texW = 215;
	int texH = 384;
	PhysBody* pbody;
	float detectionRange = 8.0f;
	Vector2D lastPlayerTile = { -1, -1 };
	int repathTimer = 0;
	int repathDelay = 100;
	char* texName = "";
	char* spriteSheetName = "";
	bool isboss = false;
	int attackRange = 0;
	float distanceToPlayer = 0.0f;
	int offsetAttackHitboxX = 0;
	int offsetAttackHitboxY = 0;
	bool direction = false;
	int health = 100;
	bool facingLeft = true;
	bool isKnockback = false;
	float knockbackForce = 10.0f;
	int knockbackTimer = 0;
	int knockbackDuration = 50;

	Vector2D deathPosition;
protected:
	b2Vec2 velocity;
	AnimationSet anims;
	std::shared_ptr<Pathfinding> pathfinding;
	PhysBody* attackHitbox;

	EnemyType type = EnemyType::GROUND;

	EnemyState state = EnemyState::IDLE;
	EnemyState lastState = EnemyState::IDLE;
};