#include "Pickable_objects.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Pics::Pics(EntityType entityType) : Entity(entityType)
{
	
	this->name = name;
	this->texture = texture;
	this->tsxPath = tsxPath;
	this->dialogue = dialogue;
	pbody = nullptr;
}

Pics::Pics()
{
	pbody = nullptr;
}

Pics::~Pics() {
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

bool Pics::Awake() {
	return true;
}

bool Pics::Start() {

	//here i make them different depending on what it is
	if (name == "Dawn") {
	
		Dialogue paperDialogue("resources/Dialogues/Pickable/Dawn.txt");
		this->dialogue = paperDialogue;
		texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png"); //placeholder
		BoolOfPlayer = 1;
	
	
	}
	else if (name == "Day") {
	
		Dialogue paperDialogue("resources/Dialogues/Pickable/Day.txt");
		this->dialogue = paperDialogue;
		texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png"); //placeholder
		BoolOfPlayer = 2;

	
	
	
	}
	else if (name == "Dusk") {

		Dialogue paperDialogue("resources/Dialogues/Pickable/Dusk.txt");
		this->dialogue = paperDialogue;
		texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png"); //placeholder
		BoolOfPlayer = 3;




	}
	else if (name == "Night") {

		Dialogue paperDialogue("resources/Dialogues/Pickable/Dusk.txt");
		this->dialogue = paperDialogue;
		texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png"); //placeholder

		BoolOfPlayer = 4;



	}
	else if (name == "Spring") {

		Dialogue paperDialogue("resources/Dialogues/Pickable/SpringWater.txt");
		this->dialogue = paperDialogue;
		texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png"); //placeholder

		BoolOfPlayer = 5;



	}
	else if (name == "Horsekin") {

		Dialogue paperDialogue("resources/Dialogues/Pickable/Horsekin.txt");
		this->dialogue = paperDialogue;
		texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png"); //placeholder

		BoolOfPlayer = 6;



	}
	else if (name == "TreeRoot") {

		Dialogue paperDialogue("resources/Dialogues/Pickable/TreeRoot.txt");
		this->dialogue = paperDialogue;
		texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png"); //placeholder

		BoolOfPlayer = 6;



	}
	InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

	//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
	texW = 128;
	texH = 128;


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

	if (pbody != nullptr) {
		pbody = nullptr;
		position.setX(xInicial);
		position.setY(yInicial);
		pbody = Engine::GetInstance().physics->CreateRectangleSensor(
			(int)position.getX(),
			(int)position.getY(),
			texW,
			texH,
			bodyType::DYNAMIC
		);
		b2Body_SetGravityScale(pbody->body, 0.0f);

		pbody->listener = this;
		pbody->ctype = ColliderType::NPC;




	}

	return true;
}

bool Pics::Update(float dt)
{
	if (!active) return true;
	Draw(dt);
	if (isGettingTouched) {
		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


			if (dialogue.hasStarted) {

				dialogue.NextDialogue();
				dialogue.Draw(dt);
				if(dialogue.hasEnded){
					CleanUp();
				
				}
				return true;
			}
			dialogue.BeginDialogue();
			dialogue.Draw(dt);
			switch (BoolOfPlayer) {

			case 0:
				break;
			case 1:
				py->dawn = true;
				break;
			case 2:
				py->day = true;
				break;
			case 3:
				py->dusk = true;
				break;
			case 4:
				py->night = true;
				break;
			default: break;



			}


			return true;
		}
		if (dialogue.hasStarted && !dialogue.hasEnded) {
			dialogue.Draw(dt);
			return true;

		}
	}




	return true;
}
void Pics::Draw(float dt) {

	//anims.Update(dt);
	//const SDL_Rect& animFrame = anims.GetCurrentFrame();

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);


	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2/*, &animFrame*/);

}

bool Pics::CleanUp()
{
	LOG("Unloading Coin");
	Engine::GetInstance().textures->UnLoad(texture);
	dialogue.CleanUp();
	Engine::GetInstance().textures->UnLoad(InteractTexture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}
void Pics::OnCollision(PhysBody* physA, PhysBody* physB) {

	Player* pp = static_cast<Player*>(physB->listener);
	py = pp;
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isGettingTouched = true;
		
		break;
	}


}
void Pics::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	isGettingTouched = false;



}

