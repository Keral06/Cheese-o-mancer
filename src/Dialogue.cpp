#include "Dialogue.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

#include "EntityManager.h"

//El primer dialogo es el ultimo del vector!!


Dialogue :: Dialogue(const char	*tsxPath) {

	this->tsxPath = tsxPath;
	std::ifstream fich(tsxPath);
	std::string helper;
	char a;
	hasStarted = false;
	fich.get(a);
	while (a != '+') { //this is what ends the txt document
		while (a != '\n') {

			helper.push_back(a);
			fich.get(a);
			

		}
		dialogue.push_back(helper);
		while (!helper.empty()) {

			helper.pop_back();

		}
		fich.get(a);




	}

	lenght = dialogue.size();
	textureDialogue = Engine::GetInstance().textures->Load("resources/UI/UI_Dialogue/UI_Dialogue_Base_01.png");
}

Dialogue::Dialogue()
{
}

Dialogue::~Dialogue() {
	
}

bool Dialogue::Awake() {
	return true;
}

bool Dialogue::Start() {

	


	return true;
}
void Dialogue::Draw(float dt) {
	float w, h;
	SDL_GetTextureSize(textureDialogue, &w, &h);
	Engine::GetInstance().render->DrawText(dialogueHelper[lenghtHelper ].c_str(), 100, 100, 0, 0, { 255,255,255 });

		
	Engine::GetInstance().render->DrawTextureNoCamera(textureDialogue,250, 420, w / 1.5, h / 1.5);
}

bool Dialogue::Update(float dt)
{
	if (!hasStarted) return true;
	if (hasEnded) { CleanUp(); }
	
	
	
	Draw(dt);
	
	return true;
}
void Dialogue::BeginDialogue() {
	
	dialogueHelper = dialogue;
	lenghtHelper = 0;
	hasStarted = true;
	printf("%s", dialogueHelper[lenghtHelper].c_str());
	
	
	


}

void Dialogue:: NextDialogue() {
	if (lenghtHelper < lenght-1) {
	
		lenghtHelper++;
	
		printf("%s",dialogueHelper[lenghtHelper].c_str());
		
	}
	else {
		hasStarted = false;
		
	}
	HasEnded(hasStarted);
}
bool Dialogue:: HasEnded(bool name){

	hasEnded = !name;
	return hasEnded;


}


bool Dialogue::CleanUp()
{
	LOG("Unloading Coin");

		dialogue.clear();

	
	return true;
}

bool Dialogue::PostUpdate() {
	if (hasStarted) {
	
		Engine::GetInstance().render->DrawText(dialogueHelper[lenghtHelper - 1].c_str(), 100, 100, 0, 0, { 255,255,255 });
		
		Engine::GetInstance().render->DrawTextureNoCamera(textureDialogue, 100, 100, 1, 1);
	}

	return true;
}

