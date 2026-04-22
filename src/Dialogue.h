#pragma once

#include "Entity.h"
#include "Animation.h"
#include <box2d/box2d.h>
#include <SDL3/SDL.h>
#include "Pathfinding.h"
#include <stdio.h>
//#include <SDL3_ttf/SDL_ttf.h>
#include <fstream>

struct SDL_Texture;

class Dialogue 
{
public:
	
	Dialogue(const char* tsxPath,  const char* name = nullptr);
	Dialogue();
	virtual ~Dialogue();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void NextDialogue();
	bool PostUpdate();
	

	void Draw(float dt);

private:

	


private:
	//Vector de texturas
	
	
	const char *tsxPath;
	const char* nameOf;

	std::vector<std::string> dialogue;
	std::vector<std::string> dialogueHelper;
	std::vector<std::string> name;
	std::vector<std::string> nameHelper;
	int lenght = 0;
	int lenghtHelper = 0;
	

public:
	
	std::string GetCurrentDialogue();
	
	SDL_Texture* textureDialogue;
	void BeginDialogue();
	bool HasEnded(bool name);

	bool hasEnded = false;
	bool hasStarted = false;
};
