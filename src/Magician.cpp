#include "Magician.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"


Magician::Magician() : NPC(EntityType::MAGICIAN)
{
	
	this->name = name;
	
	this->tsxPath = tsxPath;
	Dialogue dialogue("assets/Dialogues/Magician/Magician_Dialogues_IntroTutorial.txt", "assets/Dialogues/Magician/Magician_Names_IntroTutorial.txt");
		dialogueMagicianStart=dialogue;
	pbody = nullptr;
	Dialogue hasCheese("assets/Dialogues/Magician/Magician_Dialogues_AfterCheeseWheel.txt", "assets/Dialogues/Magician/Magician_Names_AfterCheeseWheel.txt");
	this->AfterCheese = hasCheese;
	Dialogue BeforeCheeesee("assets/Dialogues/Magician/Magician_Dialogues_BeforeCheese.txt", "assets/Dialogues/Magician/Magician_Names_BeforeCheese.txt");
	this->BeforeCheese = BeforeCheeesee;
	Dialogue Beated("assets/Dialogues/Magician/Magician_Dialogues_AfterDefeatingBoss.txt", "assets/Dialogues/Magician/Magician_Names_AfterDefeatingBoss.txt");
	BeatBoss = Beated;
	
}

Magician::~Magician() {
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

bool Magician::Awake() {
	return true;
}

bool Magician::Start() {

		std::unordered_map<int, std::string> aliases = {
			{0, "idle"},
			{18, "down"},
			{27, "stay"},
			{28, "up"}
		};

		anims.LoadFromTSX("assets/Textures/Spritesheets/Wizard/w_spritesheet.tsx", aliases);
		anims.SetCurrent("idle");
		currentAnimName = "idle";


	texture = Engine::GetInstance().textures->Load("resources/spritesheets/Wizard/sprite_mage_02.png");
	InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

	//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
		texW = 640;
		texH = 640;
	
	
		if (pbody == nullptr) {
			position.setX(xInicial);
			position.setY(yInicial);
			pbody = Engine::GetInstance().physics->CreateRectangleSensor(
				(int)position.getX(),
				(int)position.getY(),
				texW / 2,
				texH / 2,
				bodyType::DYNAMIC
			);
			b2Body_SetGravityScale(pbody->body, 0.0f);

			pbody->listener = this;
			pbody->ctype = ColliderType::MAGICIAN;




		}
	
	

	return true;
}

bool Magician::Update(float dt) {


	if (!isGettingTouched) {
		// Si el jugador se aleja, idle 
		if (currentAnimName != "idle" && currentAnimName != "up") {
			anims.SetCurrent("idle");
			currentAnimName = "idle";
		}
	}
	else {
		// Al pulsar E para hablar estando quietos -> down
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && currentAnimName == "idle") {
			anims.SetCurrent("down");
			currentAnimName = "down";
		}
		// Si stay pero el diálogo ya ha terminado -> up
		else if (currentAnimName == "stay" && !Engine::GetInstance().scene->someoneIsTalking) {
			anims.SetCurrent("up");
			currentAnimName = "up";
		}

		// HasFinished
		if (currentAnimName == "down" && anims.HasFinished()) {
			anims.SetCurrent("stay");
			currentAnimName = "stay";
		}
		else if (currentAnimName == "up" && anims.HasFinished()) {
			anims.SetCurrent("idle");
			currentAnimName = "idle";
		}
	}

	Draw(dt);
	if (Engine::GetInstance().scene->hasTalkedMagician == true) {
		firstTime = false;
		
	}
	else if (Engine::GetInstance().scene->hasTalkedMagician == false && Engine::GetInstance().scene->cards.tieneObjeto("The fool")==false) {
	
		Engine::GetInstance().scene->showHelp = true;
		Engine::GetInstance().scene->SetHelpUI(Engine::GetInstance().scene->showHelp);
	}
	if (firstTime == true) {
		
		if (!dialogueMagicianStart.hasStarted && Engine::GetInstance().input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
			Engine::GetInstance().scene->showHelp = false;
			Engine::GetInstance().scene->SetHelpUI(Engine::GetInstance().scene->showHelp);
			dialogueMagicianStart.BeginDialogue(nameNPC);
			Engine::GetInstance().scene->SetInventariIcon(true);
			Engine::GetInstance().scene->cards.push("The fool", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Fool.png"), nullptr);
			Engine::GetInstance().scene->misiones.push("Talk with magician", Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Magician1.png"), Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Magician2.png"));
		}
		else if (dialogueMagicianStart.hasStarted && !dialogueMagicianStart.hasEnded && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
		

			if (dialogueMagicianStart.AvanzarDialogo(dt, nameNPC)) {
				firstTime = false;

			}
		}if (dialogueMagicianStart.hasStarted && !dialogueMagicianStart.hasEnded) {
			dialogueMagicianStart.Draw(dt);
			return true;
		}
	}
	if (isGettingTouched) {
		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

		if (Engine::GetInstance().scene->beatBoss == true && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			BeatBoss.AvanzarDialogo(dt, nameNPC);
			return true;
		}
		if (AfterCheese.hasStarted && !AfterCheese.hasEnded) {
			AfterCheese.Draw(dt);
			return true;
		}
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedMagician == false) {
			
				if (dialogueMagicianStart.AvanzarDialogo(dt, nameNPC)) {
					Engine::GetInstance().scene->hasTalkedMagician = true;
					Engine::GetInstance().scene->misiones.Completed("Talk with magician");
				}
			
			return true;
		}
		if (dialogueMagicianStart.hasStarted && !dialogueMagicianStart.hasEnded) {
			dialogueMagicianStart.Draw(dt);
			return true;
		}


		if (Engine::GetInstance().scene->cheese == false && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedMagician == true) {
			BeforeCheese.AvanzarDialogo(dt, nameNPC);
			return true;
		}
		if (BeforeCheese.hasStarted && !BeforeCheese.hasEnded) {
			BeforeCheese.Draw(dt);
			return true;
		}

		if (Engine::GetInstance().scene->cheese == true && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			AfterCheese.AvanzarDialogo(dt, nameNPC);
			return true;
		}
		if (AfterCheese.hasStarted && !AfterCheese.hasEnded) {
			AfterCheese.Draw(dt);
			return true;
		}
	}

	return true;
}

void Magician::Draw(float dt) {

	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	int x, y;
	pbody->GetPosition(xInicial, yInicial);
	position.setX((float)xInicial);
	position.setY((float)yInicial);

	Engine::GetInstance().render->DrawTexture(texture, xInicial - texW / 2, yInicial - texH / 2, &animFrame);

}
bool Magician::CleanUp()
{
	LOG("Unloading Coin");
	Engine::GetInstance().textures->UnLoad(texture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}

void Magician::OnCollision(PhysBody* physA, PhysBody* physB) {
	Player* pp = static_cast<Player*>(physB->listener);
	py = pp;
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isGettingTouched = true;
		break;
	}



}
void Magician:: OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	isGettingTouched = false;
	//if player moves away from magician, reset dialogue
	

}