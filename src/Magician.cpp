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

Magician::Magician(Dialogue dialogueMagicianStart, std::string name, SDL_Texture* texture, const char *tsxPath, Dialogue AfterCheese, Dialogue BeforeCheese, Dialogue BeatBoss, EntityType entitytype) : NPC(name, texture, tsxPath, dialogue, entitytype)
{
	
	this->name = name;
	this->texture = texture;
	this->tsxPath = tsxPath;
	this->dialogue = dialogueMagicianStart;
	pbody = nullptr;
	this->AfterCheese = AfterCheese;
	this->BeforeCheese = BeforeCheese;
	this->BeatBoss = BeatBoss;
	
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


	std::unordered_map<int, std::string> aliases = { {0, "idle"} };
	anims.LoadFromTSX(tsxPath, aliases);
	/*coinPickupFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/coin-collision-sound-342335.wav");*/
	anims.SetCurrent("idle");

	texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png");

	//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
		texW = 32;
		texH = 32;
	
		pbody = Engine::GetInstance().physics->CreateRectangleSensor(
			(int)position.getX() + 16,
			(int)position.getY() + 16,
			32, 32,
			bodyType::STATIC
		);
	
	

	return true;
}

bool Magician::Update(float dt)
{
	if (!active) return true;

	anims.Update(dt);



	return true;
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
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		Engine::GetInstance().audio->PlayFx(coinPickupFx); //audio queue
		if (firstTime == true && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		
			if (dialogue.hasStarted)break;
			dialogue.BeginDialogue();
			firstTime = false;
			break;
		}

		Player* py = static_cast<Player*>(physB->listener);
		if (py->beatBoss && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		
			if (BeatBoss.hasStarted)break;
			BeatBoss.BeginDialogue();
			
			break;
		
		}
		if (py->hasCheese == false && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		
			if (BeforeCheese.hasStarted)break;
			BeforeCheese.BeginDialogue();
			
		
		
		}

		if (py->hasCheese == true && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		
			if (AfterCheese.hasStarted)break;
			AfterCheese.BeginDialogue();
			
		
		
		}




	
	break;
	default:
		break;


	}

	
		
	
}
