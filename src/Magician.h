#pragma once

#include "NPC.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"
#include "Dialogue.h"

struct SDL_Texture;

class Magician : public NPC
{
public:
	Magician();
	virtual ~Magician();

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
	SDL_Texture* InteractTexture = nullptr;
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;	
	Dialogue dialogueMagicianStart;
	Dialogue AfterCheese;
	Dialogue BeforeCheese;
	Dialogue BeatBoss;
	bool hasBeenKilled = false;
	bool firstTime = true;
	bool isGettingTouched = false;
	Player* py;
	bool isTalking = false;
	
	SDL_Texture* textureDialogue;
	
};
