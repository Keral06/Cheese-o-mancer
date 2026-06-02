#pragma once

#include "Entity.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"
#include "Dialogue.h"

struct SDL_Texture;

class Pics : public Entity
{
public:
	Pics(EntityType type);
	Pics();
	virtual ~Pics();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void CheckBoolOfPlayer();
	bool AlreadyPicked();


public:
	bool isPicked = false;

private:

	void GetPhysicsValues();
	
	void ApplyPhysics();
	void Draw(float dt);
	void Jump();
	


private:
	SDL_Texture* texture;
	SDL_Texture* PopUpImage;
	SDL_Texture* InteractTexture;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	bool beenPicked = false;
	bool objectDeleteIs = false;
	bool hasPbody = true;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;	
	Dialogue dialogue;
	int BoolOfPlayer = 0;
	Player* py;
	bool isGettingTouched = false;
	bool isDrawn = false;
	bool PopUpOn = false;
	void ChooseWhoIs();
	bool EmpressSpecial = false;
};
