#pragma once

#include "Entity.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"

struct SDL_Texture;

class EnemigoVolador : public Entity
{
public:

	EnemigoVolador();
	virtual ~EnemigoVolador();
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
	bool toDelete = false;
	//fx
	bool debug = false;
	int enemyDeathFx;
	bool gamePaused = false;

private:
	b2Vec2 velocity;
	AnimationSet anims;
	std::shared_ptr<Pathfinding> pathfinding;
};