#pragma once

#include "Entity.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"
#include "Dialogue.h"

struct SDL_Texture;

class NPC : public Entity
{
public:
	NPC(EntityType type);
	NPC();
	virtual ~NPC();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);


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
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;	
	Dialogue dialogue;

	
};

class HiddenScrapOfPaper : public NPC {};

class DiscardedScroll : public NPC {};
class Sketches : public NPC {};
class WallBeforeWheel : public NPC {};
class LockedDoor : public NPC {};
class DestructDoor : public NPC {};
class NormalFlag : public NPC {};
class CheeseFlag : public NPC {};
class NoteRoyalHalls : public NPC {};
class Compass : public NPC {};
class Portrait : public NPC {};
class UnfinishedPortrait : public NPC {};
class HungSword : public NPC {};
class CowWeb : public NPC {};