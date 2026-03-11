#pragma once

#include "Entity.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>

enum class Side {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	DIAGONALUL,
	DIAGONALUR,
	DIAGONALDL,
	DIAGONALDR
	// ..
};
struct SDL_Texture;

class FireBall : public Entity
{
public:
	void GetPhysicsValues();
	FireBall();
	virtual ~FireBall();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void SetPosition(Vector2D pos);
	Vector2D GetPosition();
	void ChooseSide(Side NAME);
	
private:
	bool gamePaused = false;
	bool wasPaused = false;

	b2Vec2 storedVelocity;

	void Draw();



public:

	//Declare enemy parameters
	float speed = 2.0f;
	SDL_Texture* texture = NULL;
	int texW, texH;
	PhysBody* pbody;
	int xInicial;
	int yInicial;
	bool isCollidedFloor;
	bool isCollidedWall;
	Vector2D spawnPos;
	Side spawnSide;
	bool toDelete = false;
	int fireballfx;
	EntityType ownerType;
	void AssignOwner(EntityType ownerType) {
		this->ownerType = ownerType;
	}
	EntityType CheckOwner() {
		if (ownerType == EntityType::PLAYER) {
			return EntityType::PLAYER;
		}
		else {
			return EntityType::FINALBOSS;
		}
	}
private:
	b2Vec2 velocity;
	AnimationSet anims;
	
};