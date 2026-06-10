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
//

Dialogue :: Dialogue(const char	*tsxPath, const char* name) {
	textureDialogue = Engine::GetInstance().textures->Load("resources/UI/UI_Dialogue/UI_Dialogue_Base_01.png");
	
	this->tsxPath = tsxPath;
	nameOf = name;
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
		lenght++;
		dialogue.push_back(helper);
		while (!helper.empty()) {

			helper.pop_back();
		}
		fich.get(a);

	}
	if (name == nullptr) { return; }
	std::ifstream ficht(nameOf);
	std::string helpery;
	char b;
	hasStarted = false;
	ficht.get(b);
	while (b != '+') { //this is what ends the txt document
		while (b != '\n') {
			helpery.push_back(b);
			ficht.get(b);


		}

		this->name.push_back(helpery);
	
		while (!helpery.empty()) {

			helpery.pop_back();

		}
		ficht.get(b);
	}
}

Dialogue::Dialogue()
{
	textureDialogue = Engine::GetInstance().textures->Load("resources/UI/UI_Dialogue/UI_Dialogue_Base_01.png");
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

	Engine::GetInstance().render->DrawTextureNoCamera(textureDialogue,250, 420, w / 1.5, h / 1.5);

	if (choicesBeingMade) {
		Engine::GetInstance().render->DrawText(dialogueHelper[lenghtHelper-1].c_str(), 330, 550, 0, 0, { 0,0,0 });
		Engine::GetInstance().render->DrawText(dialogueHelper[lenghtHelper].c_str(), 330, 580, 0, 0, { 0,0,0 });
	
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
		
			choice = true;
			NextDialogue();
		
		}
	
		if (nameHelper.size() != 0) {

			Engine::GetInstance().render->DrawText(nameHelper[lenghtHelper].c_str(), 312, 520, 0, 0, { 0,0,0 });
			HandleAudio(nameHelper[lenghtHelper]);

		}
		return;
	}

	Engine::GetInstance().render->DrawText(dialogueHelper[lenghtHelper ].c_str(), 330, 550, 0, 0, { 0,0,0 });

	if (nameHelper.size() != 0) {
	
		Engine::GetInstance().render->DrawText(nameHelper[lenghtHelper].c_str(), 312, 520, 0, 0, { 0,0,0 });
		//funcio a cridar switches audios amb nameHelper[lenghtHelper] ---> passar d string a const char*
		HandleAudio(nameHelper[lenghtHelper]);
	}
}

bool Dialogue::Update(float dt)
{
	if (!hasStarted) return true;
	if (hasEnded) { return true; }
	
	
	
	Draw(dt);
	
	return true;
}
void Dialogue::BeginDialogue(std::string charName) {
	
	dialogueHelper = dialogue;
	nameHelper = name;
	lenghtHelper = 0;
	hasStarted = true;
	hasEnded = false;
	printf("%s", dialogueHelper[lenghtHelper].c_str());
	Engine::GetInstance().scene->someoneIsTalking = true;
	
	HandleAudio(charName);

}

bool Dialogue::WhatChoice() {

	return choice;

}

void Dialogue:: NextDialogue() {
	if (choicesBeingMade) {
	
		if (choice) {
			if (lenghtHelper < lenght - 1) {

				while (dialogueHelper[lenghtHelper] != "CHOICE 1:") {
				
					lenghtHelper++;
				}
				lenghtHelper++;

				printf("%s\n", dialogueHelper[lenghtHelper].c_str());
				if (nameHelper.size() != 0) {

					printf("%s\n", nameHelper[lenghtHelper].c_str());
					HandleAudio(nameHelper[lenghtHelper]);
				}
			}		
		}
		else {
		
			while (dialogueHelper[lenghtHelper] != "CHOICE 2:") {

				lenghtHelper++;
			}
			lenghtHelper++;
		}
		choicesBeingMade = false;
	
		return;
	}

	if (choice) {
	
		if (dialogueHelper[lenghtHelper + 1] == "CHOICE 2:" && choice) {
	
		lenghtHelper = lenght;
	
		}
	}
	if (lenghtHelper < lenght-1) {
	
		lenghtHelper++;
		if (dialogueHelper[lenghtHelper] == "CHOICE") {
		
			choicesBeingMade = true;
			lenghtHelper += 2;
		
		}

		printf("%s\n",dialogueHelper[lenghtHelper].c_str());
		if (nameHelper.size() != 0) {
		
			printf("%s\n",nameHelper[lenghtHelper].c_str());
			HandleAudio(nameHelper[lenghtHelper]);
		
		}
	}
	else {
		hasStarted = false;
		Engine::GetInstance().scene->someoneIsTalking = false;
	}
	HasEnded(hasStarted);
}

std::string Dialogue::GetCurrentDialogue() {
	return dialogueHelper[lenghtHelper];
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

void Dialogue::AddDialogue(const char* tsxPath) {


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
		lenght++;
		dialogue.push_back(helper);
		while (!helper.empty()) {

			helper.pop_back();

		}
		fich.get(a);
	}
}

void Dialogue::AddName(const char* tsxPath) {


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
	
		name.push_back(helper);
		while (!helper.empty()) {

			helper.pop_back();
		}
		fich.get(a);
	}
	std::string totalName;
	for (int i = 0; i < name.size(); ++i) 
	{
		totalName = totalName + name.at(i);
	}
	if (totalName == "Magician:") {
		/*LoadTalkSound(charName);
		PlayTalkSound(charName);*/
		LOG("Magician talking");
	}
}

bool Dialogue::AvanzarDialogo(float dt, std::string charName) {
	if (hasStarted) {

		NextDialogue();
		Draw(dt);
		//play general audio
	/*	LoadTalkSound(charName);
		PlayTalkSound(charName);*/
		if (hasConversation == true) {
			if (nameHelper[lenghtHelper] == "Fool: ") {
				hasPlayed = false;
			}
		}
		if (hasPlayed == true) {
			LOG("hasPlayed TRUE");
		}
		else {
			LOG("hasPlayed FALSE");
		}
		
		if (hasEnded) {
			hasPlayed = false;
			
			return true;
		}
		return false;
	}
	BeginDialogue(charName);
	Draw(dt);

	return false;
}

void Dialogue::HandleAudio(std::string name) {
	LoadTalkSound(name);
	if (hasPlayed == false) {
		PlayTalkSound(name);
	}
	
}

void Dialogue::LoadTalkSound(std::string charName) {
	int randNum;
	if (charName == "Magician: ") {
		randNum = rand() % 6;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/magician/Magician_talk1.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/magician/Magician_talk2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/magician/Magician_talk3.wav");
			break;
		case 3:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/magician/Magician_hum1.wav");
			break;
		case 4:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/magician/Magician_hum2.wav");
			break;
		case 5:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/magician/Magician_hum3.wav");
			break;
		}
	}	

	if (charName == "Milkmaid") {
		randNum = rand() % 4;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Milklady/Milklady_talk1.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Milklady/Milklady_talk2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Milklady/Milklady_talk3.wav");
			break;
		case 3:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Milklady/Milklady_talk4.wav");
			break;
		}
	}

	if (charName == "Hermit") {
		randNum = rand() % 4;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hermit/Hermit_talk.wav");
			LOG("case 0");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hermit/Hermit_talk2.wav");
			LOG("case 1");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hermit/Hermit_talk3.wav");
			LOG("case 2");
			break;
		case 3:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hermit/Hermit_talk4.wav");
			LOG("case 3");
			break;
		}
	}

	if (charName == "Hierophant") {
		randNum = rand() % 7;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hierophant/Hierophant_talk_.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hierophant/Hierophant_talk_2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hierophant/Hierophant_talk_3.wav");
			break;
		case 3:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hierophant/Hierophant_talk_4.wav");
			break;
		case 4:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hierophant/Hierophant_talk_5.wav");
			break;
		case 5:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Hierophant/Hierophant_talk_8.wav");
			break;
		}
	}

	if (charName == "Well") {
		randNum = rand() % 3;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Echo_Well/Well_talk3.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Echo_Well/Well_talk1.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Echo_Well/Well_talk2.wav");
			break;
		}
	}

	if (charName == "HighPriestess") {
		randNum = rand() % 3;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/High_Priestess/HighPriestess_talk1.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/High_Priestess/HighPriestess_talk2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/High_Priestess/HighPriestess_talk3.wav");
			break;
		case 3:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/High_Priestess/HighPriestess_talk4.wav");
			break;
		}
	}

	if (charName == "RetiredKnight") {
		randNum = rand() % 3;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Retired_Knight/Retired_Knight_talk1.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Retired_Knight/Retired_Knight_talk2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Retired_Knight/Retired_Knight_talk3.wav");
			break;
		}
	}

	if (charName == "Nohuely") {
		randNum = rand() % 4;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Lady_Nohuely/Lady_Nohuely_talk1.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Lady_Nohuely/Lady_Nohuely_talk2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Lady_Nohuely/Lady_Nohuely_talk3.wav");
			break;
		case 3:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Lady_Nohuely/Lady_Nohuely_talk4.wav");
			break;
		}
	}

	if (charName == "Sculptor") {
		randNum = rand() % 3;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Sculptor/Sculptor_talk1.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Sculptor/Sculptor_talk2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Sculptor/Sculptor_talk3.wav");
			break;
		}
	}

	if (charName == "Empress") {
		randNum = rand() % 3;
		switch (randNum) {
		case 0:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Queen/Queen_talk1.wav");
			break;
		case 1:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Queen/Queen_talk2.wav");
			break;
		case 2:
			currentFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/fx/Queen/Queen_talk3.wav");
			break;
		}
	}
}


void Dialogue::PlayTalkSound(std::string charName) {
	
	if (nameHelper[lenghtHelper] != "Fool: ") {
		Engine::GetInstance().audio->PlayFx(currentFx);
		hasPlayed = true;
	}
	
	/*if (charName == "Magician: ") {
		
	}*/
}
