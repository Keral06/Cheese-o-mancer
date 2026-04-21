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
	
}

bool Dialogue::Update(float dt)
{
	if (!hasStarted) return true;
	
	
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
	
		NextDialogue();
	
	}
	
	return true;
}
void Dialogue::BeginDialogue() {
	
	dialogueHelper = dialogue;
	lenghtHelper = lenght;
	hasStarted = true;
	printf("%s", dialogueHelper[lenghtHelper - 1].c_str());
	Engine::GetInstance().render->DrawText(dialogueHelper[lenghtHelper - 1].c_str(), 100, 100, 0, 0, { 255,255,255 });
	dialogue.pop_back();
	lenghtHelper--;


}
void Dialogue:: NextDialogue() {
	if (lenghtHelper > 0) {
	
		printf("%s",dialogueHelper[lenghtHelper-1].c_str());
		Engine::GetInstance().render->DrawText(dialogueHelper[lenghtHelper - 1].c_str(), 100, 100, 0, 0, { 255,255,255 });
		dialogue.pop_back();
		lenghtHelper--;
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

