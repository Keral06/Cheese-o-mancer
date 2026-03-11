#pragma once
#include "Entity.h"
#include "Animation.h"
#include <SDL3/SDL.h>

struct SDL_Texture;

class Protection : public Entity
{
public:

	Protection();
	virtual ~Protection();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool Destroy();
	void OnCollision(PhysBody* physA, PhysBody* physB) override;
public:

	bool isPicked = false;
	int xInicial = 0;
	int yInicial = 0;

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	AnimationSet anims;
	//L08 TODO 4: Add a physics to an item
	PhysBody* pbody;
};
