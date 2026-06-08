#include "cheeseballInteractuable.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

CheeseBallInteract::CheeseBallInteract() : NPC(EntityType::CHEESEBALLINTERACT)
{
	pbody = nullptr;
	if(Engine::GetInstance().scene->cheese==false)dialogue = Dialogue("Assets/Dialogues/Interactuables/Justice_Dialogues_FindCheeseWheel.txt");
	/*else if(Engine::GetInstance().scene->doublejump == false) { dialogue = Dialogue("Assets/Dialogues/Interactuables/Justice_Dialogues_FindCheeseWheel2.txt"); }
	else if (Engine::GetInstance().scene->moho == false) { dialogue = Dialogue("Assets/Dialogues/Interactuables/Justice_Dialogues_FindCheeseWheel3.txt"); }*/
}


CheeseBallInteract::~CheeseBallInteract() {
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

bool CheeseBallInteract::Awake() {
	return true;
}

bool CheeseBallInteract::Start() {

	
	texW = 256;
	texH = 128;
	CheeseBallTexture = Engine::GetInstance().textures->Load("resources/spritesheets/Jester/Cheese_wheel/Cheese_flat_asset.png");
	InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

	if (pbody == nullptr) {
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
		pbody->ctype = ColliderType::MAGICIAN;




	}



	return true;
}

bool CheeseBallInteract::Update(float dt)
{
	if (!active) return true;
	if (Engine::GetInstance().scene->cheese)return true;

	Draw(dt);
	CheeseBallTexture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/Cheese_wheel/Cheese_flat_asset.png");
			if (isGettingTouched) {
				Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


					
					if (dialogue.AvanzarDialogo(dt)) {

						if (Engine::GetInstance().scene->cheese == false) {
							Engine::GetInstance().scene->cheese = true;


							Engine::GetInstance().scene->cards.push("WheelOfFortune", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_WheelOfFortune.png"), nullptr);

							Engine::GetInstance().scene->helpTextures.push_back(Engine::GetInstance().textures->Load("assets/UI/UI_Tutorial/UI_TutorialControls3_.png"));
							Engine::GetInstance().scene->showHelp = true;
							Engine::GetInstance().scene->actualHelpTexture = 1;
							Engine::GetInstance().scene->SetHelpUI(Engine::GetInstance().scene->showHelp);

							if (py != nullptr) {
								py->PlayShowCheese();

							}

							return true;

						}

							
						/*if (Engine::GetInstance().scene->doublejump == false) {
						
							Engine::GetInstance().scene->doublejump = true;
						
						}
						if (Engine::GetInstance().scene->moho == false) {

							Engine::GetInstance().scene->moho = true;

						}*/

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
void CheeseBallInteract::Draw(float dt) {

	
	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);


	Engine::GetInstance().render->DrawTexture(CheeseBallTexture, x - texW / 2, y - texH / 2);

}
bool CheeseBallInteract::CleanUp()
{
	LOG("Unloading Coin");
	Engine::GetInstance().textures->UnLoad(texture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	active = false;
	return true;
}
void CheeseBallInteract::OnCollision(PhysBody* physA, PhysBody* physB) {
	Player* pp = static_cast<Player*>(physB->listener);
	py = pp;
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isGettingTouched = true;

		break;
	}



}
void CheeseBallInteract::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	isGettingTouched = false;


}

