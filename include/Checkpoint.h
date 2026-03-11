#pragma once
#include "Entity.h"
#include "Animation.h"
#include <SDL3/SDL.h>

struct SDL_Texture;

class Checkpoint : public Entity
{
public:
	Checkpoint();
	virtual ~Checkpoint();

	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

public:
	bool isActivated = false;

private:
	SDL_Texture* texture = nullptr;
	Animation idleAnim;
	Animation activateAnim;
	Animation* currentAnim = nullptr;

	PhysBody* pbody = nullptr;
	int fxId = -1;
};