#pragma once

#include "Entity.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void SetPosition(Vector2D pos);
	Vector2D GetPosition();
	void Reset();

private:
	void PerformPathfinding();
	void GetPhysicsValues();
	void Move();
	void ApplyPhysics();
	void Draw(float dt);
	void Jump();


public:

	//Declare enemy parameters
	float speed = 1.0f;
	SDL_Texture* texture = NULL;
	int texW, texH;
	PhysBody* pbody;
	int xInicial;
	int yInicial;
	bool isCollidedFloor;
	bool isCollidedWall;
	bool isFollowing;
	bool JumpNeeded;
	bool toDelete = false;
	bool debug = false;
	//fx
	int enemyfx;
	int enemyDeathFx;
	int lastStepTime = 0;
	bool facingRight = false;
	bool gamePaused = false;
private:
	b2Vec2 velocity;
	AnimationSet anims;
	std::shared_ptr<Pathfinding> pathfinding;
};