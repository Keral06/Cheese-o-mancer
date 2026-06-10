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

	if (cheesePower==1) {
		dialogue = Dialogue("Assets/Dialogues/Interactuables/Justice_Dialogues_FindCheeseWheel.txt"); return true;
		dialogue.hasConversation = false;
	}
	else if (cheesePower==2) { 
		dialogue = Dialogue("Assets/Dialogues/Interactuables/Justice_Dialogues_FindCheeseWheel2.txt");  
		dialogue.hasConversation = false;
		return true; 
	}
	else if (cheesePower==3) { 
		dialogue = Dialogue("Assets/Dialogues/Interactuables/Justice_Dialogues_FindCheeseWheel3.txt");  
		dialogue.hasConversation = false;
		return true; 
	}


	return true;
}

bool CheeseBallInteract::Update(float dt)
{
	if (!active) return true;

	if (cheesePower == 1) {
	
		if (Engine::GetInstance().scene->cheese)return true;
	
	}if (cheesePower == 2) {
	
		if (Engine::GetInstance().scene->dobleSalto)return true;
	
	
	}if (cheesePower == 3) {
	
		if (Engine::GetInstance().scene->mohoWalls)return true;
	
	}

	Draw(dt);
	CheeseBallTexture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/Cheese_wheel/Cheese_flat_asset.png");

	// FASE 2: Esperamos a que la animación fluida termine para lanzar el tutorial
	if (waitingForAnim) {
		waitTimer -= dt;
		if (waitTimer <= 0.0f) {
			waitingForAnim = false;

			// AHORA SÍ, LANZAMOS EL TUTORIAL
			Engine::GetInstance().scene->helpTextures.push_back(Engine::GetInstance().textures->Load("assets/UI/UI_Tutorial/UI_TutorialControls3_.png"));
			Engine::GetInstance().scene->showHelp = true;
			Engine::GetInstance().scene->actualHelpTexture = 1;
			Engine::GetInstance().scene->SetHelpUI(true);
		}
		return true;
	}

	

	// FASE 1: Diálogo e interacción
	if (isGettingTouched) {
		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);
		dialogue.hasConversation = false;
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			if (dialogue.AvanzarDialogo(dt, nameNPC)) {
				if (cheesePower == 1) {
					Engine::GetInstance().scene->cheese = true;
					Engine::GetInstance().scene->cards.push("WheelOfFortune", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_WheelOfFortune.png"), nullptr);
					Engine::GetInstance().scene->helpTextures.push_back(Engine::GetInstance().textures->Load(("assets/UI/UI_Tutorial/UI_TutorialControls3_.png")));
					Engine::GetInstance().scene->actualHelpTexture++;
					// Activamos la animación
					if (py != nullptr) {
						py->PlayShowCheese();
					}

					// Iniciamos el cronómetro (1900 ms para que congele la pose justo al final)
					waitingForAnim = true;
					waitTimer = 1900.0f;
				}
				else if (cheesePower == 2) {
					Engine::GetInstance().scene->dobleSalto = true;
					Engine::GetInstance().scene->cards.push("WheelOfFortune", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_WheelOfFortune.png"), nullptr);
					Engine::GetInstance().scene->actualHelpTexture++;
					Engine::GetInstance().scene->helpTextures.push_back(Engine::GetInstance().textures->Load(("assets/UI/UI_Tutorial/UI_TutorialControls4_jump.png")));
					// Activamos la animación
					if (py != nullptr) {
						py->PlayShowCheese();
					}

					// Iniciamos el cronómetro (1900 ms para que congele la pose justo al final)
					waitingForAnim = true;
					waitTimer = 1900.0f;
				}
				else if (cheesePower == 3) {
					Engine::GetInstance().scene->mohoWalls = true;
					Engine::GetInstance().scene->cards.push("WheelOfFortune", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_WheelOfFortune.png"), nullptr);
					Engine::GetInstance().scene->actualHelpTexture++;
					Engine::GetInstance().scene->helpTextures.push_back(Engine::GetInstance().textures->Load(("assets/UI/UI_Tutorial/UI_TutorialControls4_climb.png")));
					// Activamos la animación
					if (py != nullptr) {
						py->PlayShowCheese();
					}

					// Iniciamos el cronómetro (1900 ms para que congele la pose justo al final)
					waitingForAnim = true;
					waitTimer = 1900.0f;
				}
				

			}
			return true;
		}

		if (dialogue.hasStarted && !dialogue.hasEnded) {
			dialogue.hasConversation = false;
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

