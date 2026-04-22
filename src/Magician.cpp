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
	Dialogue dialogue("resources/Dialogues/Magician/Magician_Dialogues_IntroTutorial.txt", "resources/Dialogues/Magician/Magician_Names_IntroTutorial.txt");
		dialogueMagicianStart=dialogue;
	pbody = nullptr;
	Dialogue hasCheese("resources/Dialogues/Magician/Magician_Dialogues_AfterCheeseWheel.txt", "resources/Dialogues/Magician/Magician_Names_AfterCheeseWheel.txt");
	this->AfterCheese = AfterCheese;
	Dialogue BeforeCheeese("resources/Dialogues/Magician/Magician_Dialogues_BeforeCheese.txt", "resources/Dialogues/Magician/Magician_Names_BeforeCheese.txt");
	this->BeforeCheese = BeforeCheese;
	Dialogue Beated("resources/Dialogues/Magician/Magician_Dialogues_AfterDefeatingBoss.txt", "resources/Dialogues/Magician/Magician_Names_AfterDefeatingBoss.txt");
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


	std::unordered_map<int, std::string> aliases = { {0, "idle"}, { 19, "talking"} };
	anims.LoadFromTSX("resources/spritesheets/Wizard/sprite_mage_01.tsx", aliases);
	/*coinPickupFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/coin-collision-sound-342335.wav");*/
	anims.SetCurrent("idle");

	texture = Engine::GetInstance().textures->Load("resources/spritesheets/Wizard/sprite_mage_01.png");

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

bool Magician::Update(float dt)
{
	Draw(dt);

	if(isGettingTouched){
		
		
		if (firstTime == true && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

			if (dialogueMagicianStart.hasEnded) {
				firstTime = false;
				return true;

			}
			if (dialogueMagicianStart.hasStarted) { 
			
				dialogueMagicianStart.NextDialogue();
				dialogueMagicianStart.Draw(dt);
				return true;
				 }
			dialogueMagicianStart.BeginDialogue();
			dialogueMagicianStart.Draw(dt);
			

			return true;
		}
		if (dialogueMagicianStart.hasStarted && !dialogueMagicianStart.hasEnded) {
			dialogueMagicianStart.Draw(dt);
			return true;
			
		}
		if (py->beatBoss && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

			
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
			dialogueMagicianStart.Draw(dt);
			return true;

		}
		if (py->hasCheese == false && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

			if (BeforeCheese.hasStarted) {

				BeforeCheese.NextDialogue();
				BeforeCheese.Draw(dt);
				return true;
			}
			BeforeCheese.BeginDialogue();
			BeforeCheese.Draw(dt);


			return true;



		}if (BeforeCheese.hasStarted && !BeforeCheese.hasEnded) {
			dialogueMagicianStart.Draw(dt);
			return true;

		}

		if (py->hasCheese == true && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

			if (AfterCheese.hasStarted) {

				AfterCheese.NextDialogue();
				AfterCheese.Draw(dt);
				return true;
			}
			AfterCheese.BeginDialogue();
			AfterCheese.Draw(dt);


			return true;



		}if (AfterCheese.hasStarted && !AfterCheese.hasEnded) {
			dialogueMagicianStart.Draw(dt);
			return true;

		}





		

	}

	


	return true;
}
void Magician::Draw(float dt) {

	anims.Update(dt);
	const SDL_Rect& animFrame = anims.GetCurrentFrame();

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);


	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

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


}