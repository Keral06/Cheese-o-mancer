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

class HiddenScrapOfPaper : public NPC
{
public:
	HiddenScrapOfPaper();

	virtual ~HiddenScrapOfPaper();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class DiscardedScroll : public NPC
{
public:
	DiscardedScroll();

	virtual ~DiscardedScroll();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class Sketches : public NPC
{
public:
	Sketches();

	virtual ~Sketches();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class WallBeforeWheel : public NPC
{
public:
	WallBeforeWheel();

	virtual ~WallBeforeWheel();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class LockedDoor : public NPC
{
public:
	LockedDoor();
	virtual ~LockedDoor();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class DestructDoor : public NPC
{
public:
	DestructDoor();

	virtual ~DestructDoor();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class NormalFlag : public NPC
{
public:
	NormalFlag();
	virtual ~NormalFlag();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class CheeseFlag : public NPC
{
public:
	CheeseFlag();
	virtual ~CheeseFlag();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class NoteRoyalHalls : public NPC
{
public:
	NoteRoyalHalls();

	virtual ~NoteRoyalHalls();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class Compass : public NPC
{
public:
	Compass();

	virtual ~Compass();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class Portrait : public NPC
{
public:
	Portrait();
	virtual ~Portrait();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class UnfinishedPortrait : public NPC
{
public:
	UnfinishedPortrait();

	virtual ~UnfinishedPortrait();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class HungSword : public NPC
{
public:
	HungSword();

	virtual ~HungSword();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class CowWeb : public NPC
{
public:
	CowWeb();

	virtual ~CowWeb();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class CommemorativeMonument : public NPC
{
public:
	CommemorativeMonument();

	virtual ~CommemorativeMonument();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








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

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

//PEOPLE YOU CAN INTERACT LEVEL 2

class Nohuely : public NPC
{
public:
	Nohuely();

	virtual ~Nohuely();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	Dialogue secondDialogue;
	bool hasTalkedBefore = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

class milkmaid : public NPC
{
public:
	milkmaid();

	virtual ~milkmaid();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	Dialogue secondDialogue;
	bool hasTalkedBefore = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

class TowGuard : public NPC
{
public:
	TowGuard();

	virtual ~TowGuard();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	Dialogue secondDialogue;
	bool hasTalkedBefore = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};



class death : public NPC
{
public:
	death();

	virtual ~death();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	Dialogue secondDialogue;
	Dialogue PercentChance;
	bool hasTalkedBefore = false;
	int randomNumber = 0;
public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

//THE HERMIT MISSION 1

class Hermit : public NPC
{
public:
	Hermit();

	virtual ~Hermit();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue notAdvanced;
	Dialogue level1;
	Dialogue level2;
	Dialogue level3;
	Dialogue hasAll;
	Dialogue hasAllFinished;
	bool finishedMission = false;
	


	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
//Empress in disguise wells echo

class Well : public NPC
{
public:
	Well();

	virtual ~Well();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue notAdvanced;
	Dialogue level1;
	Dialogue level2;
	Dialogue level3;
	Dialogue hasAll;
	Dialogue hasAllFinished;
	bool finishedMission = false;



	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

//Level 3 NPCs

class Hierophant : public NPC
{
public:
	Hierophant();

	virtual ~Hierophant();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue notAdvanced;
	Dialogue level1;
	Dialogue level2;
	Dialogue level3;
	Dialogue hasAll;
	Dialogue hasAllFinished;
	bool finishedMission = false;
	


	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

//Little Timmy

class Timmy : public NPC
{
public:
	Timmy();

	virtual ~Timmy();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);
	void WhatPositionIsIt(int i);

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue notAdvanced;
	Dialogue hiding1;
	Dialogue hiding2;
	Dialogue hiding3;
	Dialogue AfterBoss;
	Dialogue hasAllFinished;
	Dialogue hasFinished;
	bool finishedMission = false;
	int isThereATM = false;


	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;
	int hidingPlace = 0;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

//Empress

class Empress : public NPC
{
public:
	Empress();

	virtual ~Empress();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);
	

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue regular;
	Dialogue trusted;
	Dialogue scolding;
	Dialogue GivesArtifact;
	Dialogue AfterMission;
	
	bool finishedMission = false;
	int isThereATM = false;


	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;
	int hidingPlace = 0;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

//TELEPORT MISION


class RatKing : public NPC
{
public:
	RatKing();

	virtual ~RatKing();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);


public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue initial;
	Dialogue notAdvanced;
	Dialogue completed;
	Dialogue teleport;
	Dialogue AfterMission;

	bool finishedMission = false;
	int isThereATM = false;


	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;
	int hidingPlace = 0;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};


//MISSION SCULPTOR

class RetiredKnight : public NPC
{
public:
	RetiredKnight();

	virtual ~RetiredKnight();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);


public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue initial;
	Dialogue notAdvanced;
	Dialogue completed;
	Dialogue teleport;
	Dialogue AfterMission;

	bool finishedMission = false;
	int isThereATM = false;


	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;
	int hidingPlace = 0;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
//MISSION KNIGHT
class Sculptor : public NPC
{
public:
	Sculptor();

	virtual ~Sculptor();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);


public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

	Dialogue initial;
	Dialogue notAdvanced;
	Dialogue completed;
	Dialogue teleport;
	Dialogue AfterMission;

	bool finishedMission = false;
	int isThereATM = false;


	//Hermit bools
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;
	int hidingPlace = 0;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};


//High Priestess

class HighPriestess : public NPC
{
public:
	HighPriestess();
	virtual ~HighPriestess();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);
	void Draw(float dt);

private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;
	std::string currentAnimName = "idle";

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
