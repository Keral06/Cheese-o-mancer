#pragma once

#include "NPC.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"
#include "Dialogue.h"

struct SDL_Texture;

class CheeseBallInteract : public NPC
{
public:
	CheeseBallInteract();
	
	~CheeseBallInteract();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);


public:
	bool isPicked = false;

private:

	void GetPhysicsValues();
	
	void ApplyPhysics();
	void Draw(float dt);
	void Jump();
	


private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	SDL_Texture* CheeseBallTexture=nullptr;
	SDL_Texture* InteractTexture=nullptr;
	int xInicial;
	int yInicial;	

	Player* py;
	int moneyPlayer = 0;
	bool isGettingTouched = false;
	
};
