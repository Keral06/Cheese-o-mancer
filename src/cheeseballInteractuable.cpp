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
	CheeseBallTexture = Engine::GetInstance().textures->Load("resources/spritesheets/Jester/Cheese_flat_asset.png");
	InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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

bool CheeseBallInteract::Update(float dt)
{
	if (!active) return true;

	Draw(dt);
	if (isGettingTouched) {
		
		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - 16, (int)position.getY() - texH / 2 - 32);
		if ( Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			
			py->hasCheese = true;
			CleanUp();

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

