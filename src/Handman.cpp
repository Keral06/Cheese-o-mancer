#include "HANDMAN.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

HANDMAN::HANDMAN() : NPC(EntityType::HANDMAN)
{
	
	this->name = name;
	this->texture = texture;
	this->tsxPath = tsxPath;
	//Dialogue firstStime("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_FirstMeeting.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_FirstMeeting.txt");
	//this->dialogue = firstStime; //dialogo al descubrirlo por primera vez
	//pbody = nullptr;
	//Dialogue dialogueHandman("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_BeforeBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_BeforeBoss.txt");
	//dialogueHANDMAN = dialogueHandman; //dialogo despues de beat el boss
	//Dialogue hasbeensold("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_Buying.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_Buying.txt");
	//this->hasBought = hasbeensold; //dialogo salir habiendo comprado
	//Dialogue notbought("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_NoMoney.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_NoMoney.txt");
	//this->hasNotBought = notbought; //dialogo salir sin haber comprado
	//Dialogue hasBeatBoss("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_AfterBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_AfterBoss.txt");
	//this->BeatBoss = hasBeatBoss;
}


HANDMAN::~HANDMAN() {
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

bool HANDMAN::Awake() {
	return true;
}

bool HANDMAN::Start() {


	std::unordered_map<int, std::string> aliases = { {0, "idle"}, { 36, "selling"}, {36+35, "talking"}};
	anims.LoadFromTSX("resources/spritesheets/Hangman/sprite_hangedman_01.tsx", aliases);
	/*coinPickupFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/coin-collision-sound-342335.wav");*/
	anims.SetCurrent("idle");

	texture = Engine::GetInstance().textures->Load("resources/spritesheets/Wizard/sprite_hangedman_01.png");
	InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

	//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
	texW = 256;
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
	//Poner dialogo dependiendo del nivel
	if (level == 1) {
		//Dialogue firstStime("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_FirstMeeting.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_FirstMeeting.txt");
//this->dialogue = firstStime; //dialogo al descubrirlo por primera vez
//pbody = nullptr;
//Dialogue dialogueHandman("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_BeforeBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_BeforeBoss.txt");
//dialogueHANDMAN = dialogueHandman; //dialogo despues de beat el boss
//Dialogue hasbeensold("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_Buying.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_Buying.txt");
//this->hasBought = hasbeensold; //dialogo salir habiendo comprado
//Dialogue notbought("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_NoMoney.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_NoMoney.txt");
//this->hasNotBought = notbought; //dialogo salir sin haber comprado
//Dialogue hasBeatBoss("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_AfterBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_AfterBoss.txt");
//this->BeatBoss = hasBeatBoss;
	}
	else if (level == 2) {
	
		//Dialogue firstStime("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_FirstMeeting.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_FirstMeeting.txt");
	//this->dialogue = firstStime; //dialogo al descubrirlo por primera vez
	//pbody = nullptr;
	//Dialogue dialogueHandman("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_BeforeBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_BeforeBoss.txt");
	//dialogueHANDMAN = dialogueHandman; //dialogo despues de beat el boss
	//Dialogue hasbeensold("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_Buying.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_Buying.txt");
	//this->hasBought = hasbeensold; //dialogo salir habiendo comprado
	//Dialogue notbought("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_NoMoney.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_NoMoney.txt");
	//this->hasNotBought = notbought; //dialogo salir sin haber comprado
	//Dialogue hasBeatBoss("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_AfterBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_AfterBoss.txt");
	//this->BeatBoss = hasBeatBoss;
	}


	return true;
}

bool HANDMAN::Update(float dt)
{
	if (!active) return true;

	Draw(dt);
	if (isGettingTouched) {
		
		if (firstTime == true && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			if (dialogue.hasEnded) {
				firstTime = false;
				return true;

			}
			if (dialogue.hasStarted) {

				dialogue.NextDialogue();
				dialogue.Draw(dt);
				return true;
			}
			dialogue.BeginDialogue();
			dialogue.Draw(dt);


			return true;
			
		}
		if (dialogue.hasStarted && !dialogue.hasEnded) {
			dialogue.Draw(dt);
			return true;

		}
		//parlarli i no has beat el boss encara
		if (py->beatBoss == false && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			
			if (dialogueHANDMAN.hasStarted) {

				dialogueHANDMAN.NextDialogue();
				dialogueHANDMAN.Draw(dt);
				return true;
			}
			dialogueHANDMAN.BeginDialogue();
			dialogueHANDMAN.Draw(dt);


			return true;
		}
		if (dialogueHANDMAN.hasStarted && !dialogueHANDMAN.hasEnded) {
			dialogueHANDMAN.Draw(dt);
			return true;

		}
		//parlarli i has beat el boss primer cop
		if (py->beatBoss && firstTimeBossKill && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			if (BeatBoss.hasStarted) {

				BeatBoss.NextDialogue();
				BeatBoss.Draw(dt);
				return true;
			}
			BeatBoss.BeginDialogue();
			BeatBoss.Draw(dt);


			return true;
			
		}
		if (BeatBoss.hasStarted && !BeatBoss.hasEnded) {
			BeatBoss.Draw(dt);
			return true;

		}
		//parlarli (obre store)
		 if (py->beatBoss && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			isStoreOn = !isStoreOn;
			if (isStoreOn == true)moneyPlayer = py->score;
			Engine::GetInstance().scene->SetStore(isStoreOn);
			if (isStoreOn == false) {
				if (moneyPlayer == py->score) { //no ha comprat
					

					if (hasNotBought.hasStarted) {

						hasNotBought.NextDialogue();
						hasNotBought.Draw(dt);
						return true;
					}
					hasNotBought.BeginDialogue();
					hasNotBought.Draw(dt);


					return true;
				}
				else {//h comprat
					

					if (hasBought.hasStarted) {

						hasBought.NextDialogue();
						hasBought.Draw(dt);
						return true;
					}
					hasBought.BeginDialogue();
					hasBought.Draw(dt);


					return true;
				}

			}




		}
		 if (hasNotBought.hasStarted && !hasNotBought.hasEnded) {
			 hasNotBought.Draw(dt);
			 return true;

		 }
		 if (hasBought.hasStarted && !hasBought.hasEnded) {
			 hasBought.Draw(dt);
			 return true;

		 }

		 Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

	}




	return true;
}
void HANDMAN::Draw(float dt) {

	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);


	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

}
bool HANDMAN::CleanUp()
{
	LOG("Unloading Coin");
	Engine::GetInstance().textures->UnLoad(texture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}
void HANDMAN::OnCollision(PhysBody* physA, PhysBody* physB) {
	Player* pp = static_cast<Player*>(physB->listener);
	py = pp;
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isGettingTouched = true;
		break;
	}



}
void HANDMAN::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	isGettingTouched = false;


}

