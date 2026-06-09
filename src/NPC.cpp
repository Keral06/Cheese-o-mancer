#include "NPC.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

NPC::NPC(EntityType entityType) : Entity(entityType)
{
	
	this->name = name;
	this->texture = texture;
	this->tsxPath = tsxPath;
	this->dialogue = dialogue;
	pbody = nullptr;
}

NPC::NPC()
{
	pbody = nullptr;
}

NPC::~NPC() {
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

bool NPC::Awake() {
	return true;
}

bool NPC::Start() {


	//std::unordered_map<int, std::string> aliases = { {0, "idle"} };
	//anims.LoadFromTSX(tsxPath, aliases);
	///*coinPickupFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/coin-collision-sound-342335.wav");*/
	//anims.SetCurrent("idle");

	//texture = Engine::GetInstance().textures->Load("Assets/Textures/PREV/coin_sprite.png");

	////32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
	//	texW = 32;
	//	texH = 32;
	//
	//	pbody = Engine::GetInstance().physics->CreateRectangleSensor(
	//		(int)position.getX() + 16,
	//		(int)position.getY() + 16,
	//		32, 32,
	//		bodyType::STATIC
	//	);
	//
	//

	return true;
}

bool NPC::Update(float dt)
{
	if (!active) return true;

	anims.Update(dt);



	return true;
}

bool NPC::CleanUp()
{
	LOG("Unloading Coin");
	Engine::GetInstance().textures->UnLoad(texture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}
void NPC::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		
		Engine::GetInstance().audio->PlayFx(coinPickupFx); //audio queue

		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			if (dialogue.hasStarted)break;
			dialogue.BeginDialogue();
		}
		break;

	default:
		break;
	}
}

HiddenScrapOfPaper::HiddenScrapOfPaper() :NPC(EntityType::HIDDENSCRAPOFPAPER) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_HiddenScrapOfPaper.txt");
		this->dialogue = paperDialogue;

	}

	HiddenScrapOfPaper::~HiddenScrapOfPaper()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool HiddenScrapOfPaper::Awake() {
		return true;
	}
	bool HiddenScrapOfPaper::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
		//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
		texW = 128;
		texH = 128;


		if (pbody == nullptr) {
			position.setX(xInicial);
			position.setY(yInicial);
			pbody = Engine::GetInstance().physics->CreateRectangleSensor(
				(int)position.getX(),
				(int)position.getY(),
				texW ,
				texH	,
				bodyType::DYNAMIC
			);
			b2Body_SetGravityScale(pbody->body, 0.0f);

			pbody->listener = this;
			pbody->ctype = ColliderType::NPC;




		}if (pbody != nullptr) {
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
	bool  HiddenScrapOfPaper::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);
				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool HiddenScrapOfPaper::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void  HiddenScrapOfPaper::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void  HiddenScrapOfPaper::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}


	DiscardedScroll::DiscardedScroll() :NPC(EntityType::DISCARDEDSCROLL) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_DiscardedScroll.txt");
		this->dialogue = paperDialogue;

	}

	DiscardedScroll::~DiscardedScroll()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool DiscardedScroll::Awake() {
		return true;
	}
	bool DiscardedScroll::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool DiscardedScroll::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool DiscardedScroll::CleanUp(){
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void DiscardedScroll::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void DiscardedScroll::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	Sketches::Sketches() :NPC(EntityType::SKETCHES) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_Sketches.txt");
		this->dialogue = paperDialogue;

	}

	Sketches:: ~Sketches()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Sketches::Awake() {
		return true;
	}
	bool Sketches::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
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
	bool Sketches::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool Sketches::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Sketches::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void Sketches::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	WallBeforeWheel::WallBeforeWheel() :NPC(EntityType::WALLBEFOREWHEEL) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_WallBeforeWheel.txt");
		this->dialogue = paperDialogue;

	}

	WallBeforeWheel:: ~WallBeforeWheel()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool WallBeforeWheel::Awake() {
		return true;
	}
	bool WallBeforeWheel::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
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
	bool WallBeforeWheel::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->cheese == true) {


				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool WallBeforeWheel::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void WallBeforeWheel::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break; case ColliderType::CHEESEBALL:
				CleanUp();
				break;
		}
		}


	
	void WallBeforeWheel::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	LockedDoor::LockedDoor() :NPC(EntityType::LOCKEDDOOR) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_LockedDoor.txt");
		this->dialogue = paperDialogue;

	}

	LockedDoor:: ~LockedDoor()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool LockedDoor::Awake() {
		return true;
	}
	bool LockedDoor::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
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
	bool LockedDoor::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool LockedDoor::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void LockedDoor::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void LockedDoor::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	DestructDoor::DestructDoor() :NPC(EntityType::DESTRUCTDOOR) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_DestructDoor.txt");
		this->dialogue = paperDialogue;

	}

	DestructDoor:: ~DestructDoor()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool DestructDoor::Awake() {
		return true;
	}
	bool DestructDoor::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
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
	bool DestructDoor::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool DestructDoor::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void DestructDoor::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void DestructDoor::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	NormalFlag::NormalFlag() :NPC(EntityType::NORMALFLAG) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_NormalFlag.txt");
		this->dialogue = paperDialogue;

	}

	NormalFlag:: ~NormalFlag()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool NormalFlag::Awake() {
		return true;
	}
	bool NormalFlag::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool NormalFlag::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool NormalFlag::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void NormalFlag::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void NormalFlag::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	CheeseFlag::CheeseFlag() :NPC(EntityType::CHEESEFLAG) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_CheeseFlag.txt");
		this->dialogue = paperDialogue;

	}

	CheeseFlag:: ~CheeseFlag()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool CheeseFlag::Awake() {
		return true;
	}
	bool CheeseFlag::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool  CheeseFlag::Update(float dt) {
		if (isGettingTouched) {
			InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool  CheeseFlag::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void  CheeseFlag::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void  CheeseFlag::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	NoteRoyalHalls::NoteRoyalHalls() :NPC(EntityType::NOTEROYALHALLS) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_NoteRoyalHalls.txt");
		this->dialogue = paperDialogue;

	}

	NoteRoyalHalls:: ~NoteRoyalHalls()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool NoteRoyalHalls::Awake() {
		return true;
	}
	bool NoteRoyalHalls::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
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
	bool NoteRoyalHalls::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool NoteRoyalHalls::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void NoteRoyalHalls::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void NoteRoyalHalls::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	Compass::Compass() :NPC(EntityType::COMPASS) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_Compass.txt");
		this->dialogue = paperDialogue;

	}

	Compass:: ~Compass()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Compass::Awake() {
		return true;
	}
	bool Compass::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool Compass::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);
				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool Compass::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Compass::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void Compass::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	Portrait::Portrait() :NPC(EntityType::PORTRAIT) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_Portrait.txt");
		this->dialogue = paperDialogue;

	}

	Portrait:: ~Portrait()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Portrait::Awake() {
		return true;
	}
	bool Portrait::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool Portrait::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool Portrait::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Portrait::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void Portrait::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	UnfinishedPortrait::UnfinishedPortrait() :NPC(EntityType::UNFINISHEDPORTRAIT) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_Compass.txt");
		this->dialogue = paperDialogue;

	}

	UnfinishedPortrait::~UnfinishedPortrait()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool UnfinishedPortrait::Awake() {
		return true;
	}
	bool UnfinishedPortrait::Start() {
		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
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
	bool UnfinishedPortrait::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool UnfinishedPortrait::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void UnfinishedPortrait::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void UnfinishedPortrait::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	HungSword::HungSword() :NPC(EntityType::HUNGSWORD) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_HungSword.txt");
		this->dialogue = paperDialogue;

	}

	HungSword:: ~HungSword()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool HungSword::Awake() {
		return true;
	}
	bool HungSword::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");
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
	bool HungSword::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}
		return true;
	}
	bool HungSword::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void HungSword::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void HungSword::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	CowWeb::CowWeb() :NPC(EntityType::COWWEB) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt");
		this->dialogue = paperDialogue;

	}

	CowWeb::~CowWeb()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool CowWeb::Awake() {
		return true;
	}
	bool CowWeb::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

		//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
		texW = 128;
		texH = 128;


		if (pbody == nullptr) {
			position.setX(xInicial);
			position.setY(yInicial);
			pbody = Engine::GetInstance().physics->CreateRectangleSensor(
				(int)position.getX(),
				(int)position.getY(),
				texW *3,
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
	bool CowWeb::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				dialogue.AvanzarDialogo(dt);
				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}

		return true;
	}
	bool CowWeb::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void CowWeb::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void CowWeb::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}
	CommemorativeMonument::CommemorativeMonument() :NPC(EntityType::MONUMENT) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!!
		this->dialogue = paperDialogue;

	}

	CommemorativeMonument::~CommemorativeMonument()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool CommemorativeMonument::Awake() {
		return true;
	}
	bool CommemorativeMonument::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool CommemorativeMonument::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}

		return true;
	}
	bool CommemorativeMonument::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void CommemorativeMonument::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void CommemorativeMonument::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	//PERSONAS SEGUNDO NIVEL

	Nohuely::Nohuely() :NPC(EntityType::NOHUELY) {
		Dialogue paperDialogue("assets/Dialogues/Lady_Nohuely/Lady_Nohuely_Regular_Dialogues.txt","assets/Dialogues/Lady_Nohuely/Lady_Nohuely_Regular_Names.txt" ); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Lady_Nohuely/Lady_Nohuely_AfterRegular_Dialogues.txt", "assets/Dialogues/Lady_Nohuely/Lady_Nohuely_AfterRegular_Names.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->secondDialogue = secondDialogue;
	}

	Nohuely::~Nohuely()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Nohuely::Awake() {
		return true;
	}
	bool Nohuely::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

		std::unordered_map<int, std::string> aliases = { {70, "idle"} };

		anims.LoadFromTSX("assets/Textures/Spritesheets/Lady Nohely NPC/ladyNohuely_sit.tsx", aliases);
		anims.SetCurrent("sit");

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
	bool Nohuely::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

			if (hasTalkedBefore && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (secondDialogue.hasStarted && !secondDialogue.hasEnded) {
				secondDialogue.Draw(dt);
				return true;

			}
			
			


			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				if (dialogue.hasStarted) {

					dialogue.NextDialogue();
					dialogue.Draw(dt);
					if (dialogue.hasEnded) { dialogue.CleanUp(); hasTalkedBefore = true; }
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
		}

		return true;
	}
	bool Nohuely::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Nohuely::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void Nohuely::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}
	milkmaid::milkmaid() :NPC(EntityType::MILKMAID) {
		Dialogue paperDialogue("assets/Dialogues/Milkmaid/Milkmaid_Inital_Dialogues.txt", "assets/Dialogues/Milkmaid/Milkmaid_Inital_Names.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Milkmaid/Milkmaid_AfterInitial_Dialogues.txt", "assets/Dialogues/Milkmaid/Milkmaid_AfterInitial_Names.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->secondDialogue = secondDialogue;
	}

	milkmaid::~milkmaid()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool milkmaid::Awake() {
		return true;
	}
	bool milkmaid::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

		std::unordered_map<int, std::string> aliases = {{70, "idle"}};

		anims.LoadFromTSX("assets/Textures/Spritesheets/Milkmaid_NPC/milkmaid_idle.tsx", aliases);
		anims.SetCurrent("idle");

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
	bool milkmaid::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

			if (hasTalkedBefore && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				secondDialogue.AvanzarDialogo(dt);
				return true;
			}
			if (secondDialogue.hasStarted && !secondDialogue.hasEnded) {
				secondDialogue.Draw(dt);
				return true;

			}




			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}

		return true;
	}
	bool milkmaid::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void milkmaid::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void milkmaid::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	TowGuard::TowGuard() :NPC(EntityType::GUARDTOWER) {
		Dialogue paperDialogue("assets/Dialogues/Interactuables/Tower_Guard_Inital_Dialogues.txt", "assets/Dialogues/Interactuables/Tower_Guard_Inital_Names.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Interactuables/Tower_Guard_AfterInital_Dialogues.txt", "assets/Dialogues/Interactuables/Tower_Guard_AfterInital_Names.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->secondDialogue = secondDialogue;
	}

	TowGuard::~TowGuard()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool TowGuard::Awake() {
		return true;
	}
	bool TowGuard::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool TowGuard::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

			if (hasTalkedBefore && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				secondDialogue.AvanzarDialogo(dt);

				return true;
			}
			if (secondDialogue.hasStarted && !secondDialogue.hasEnded) {
				secondDialogue.Draw(dt);
				return true;

			}




			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

				dialogue.AvanzarDialogo(dt);

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}

		return true;
	}
	bool TowGuard::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void TowGuard::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void TowGuard::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}
	//Horsing arround


	death::death() :NPC(EntityType::DEATH) {
		Dialogue paperDialogue("assets/Dialogues/Horse/Horse_Initial_Dialogues.txt", "assets/Dialogues/Horse/Horse_Initial_Names.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Horse/Horse_AfterInitial_Dialogues.txt", "assets/Dialogues/Horse/Horse_AfterInitial_Names.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->secondDialogue = secondDialogue;

		Dialogue percent("assets/Dialogues/Horse/Horse_AfterInitialLowChance_Dialogues.txt", "assets/Dialogues/Horse/Horse_AfterInitialLowChance_Names.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->PercentChance = percent;
	}

	death::~death()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool death::Awake() {
		return true;
	}
	bool death::Start() {

		InteractTexture = Engine::GetInstance().textures->Load("assets/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

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
	bool death::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

			if (hasTalkedBefore && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				if (randomNumber == 5) {
					PercentChance.AvanzarDialogo(dt);

					return true;
				
				
				}
				secondDialogue.AvanzarDialogo(dt);

				return true;
			}
			if (PercentChance.hasStarted && !PercentChance.hasEnded) {
				PercentChance.Draw(dt);
				return true;

			}
			if (PercentChance.hasStarted && !PercentChance.hasEnded) {
				PercentChance.Draw(dt);
				return true;

			}



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


				if (dialogue.AvanzarDialogo(dt)) {
				
					Engine::GetInstance().scene->cards.push("Death", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Death.png"), nullptr);
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
	bool death::CleanUp() {
		LOG("Unloading Coin");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void death::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			randomNumber = (rand() % 100) + 1;
			break;
		}


	}
	void death::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	Hermit::Hermit() :NPC(EntityType::HERMIT) {



		Dialogue paperDialogue("assets/Dialogues/Mission_Hermit/Hermit_Initial_Dialogues.txt", "assets/Dialogues/Mission_Hermit/Hermit_Initial_Names.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Mission_Hermit/HermitMission_LVL1_Dialogues.txt", "assets/Dialogues/Mission_Hermit/HermitMission_LVL1_Names.txt"); //change the dialogue lol!!!! lvl1
		this->level1 = secondDialogue;

		Dialogue percent("assets/Dialogues/Mission_Hermit/Hermit_NotAdvanced_Dialogues.txt", "assets/Dialogues/Mission_Hermit/Hermit_NotAdvanced_Names.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->notAdvanced = percent;
		Dialogue lvll2("assets/Dialogues/Mission_Hermit/HermitMission_LVL2_Dialogues.txt", "assets/Dialogues/Mission_Hermit/HermitMission_LVL2_Names.txt"); //change the dialogue lol!!!! lvl2
		this->level2 = lvll2;
		Dialogue third("assets/Dialogues/Mission_Hermit/HermitMission_LVL3_Dialogues.txt", "assets/Dialogues/Mission_Hermit/HermitMission_LVL3_Names.txt"); //change the dialogue lol!!!! lvl3
		this->level3 = third;

		Dialogue hasAll("assets/Dialogues/Mission_Hermit/Hermit_MissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Hermit/Hermit_MissionCompleted_Names.txt"); //change the dialogue lol!!!! all
		this->hasAll = hasAll;
		Dialogue hasAllFinished("assets/Dialogues/Mission_Hermit/Hermit_AfterMissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Hermit/Hermit_AfterMissionCompleted_Names.txt"); //change the dialogue lol!!!! all
		this->hasAllFinished = hasAllFinished;
	}

	Hermit::~Hermit()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Hermit::Awake() {
		return true;
	}
	bool Hermit::Start() {

		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"},{15, "start"}, {30, "talk"},{45, "end"}
		};
		anims.LoadFromTSX("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
		texW = 128*3;
		texH = 128*4 ;
		pbody = nullptr;
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
			pbody->ctype = ColliderType::NPC;
		}


		return true;
	}
	void Hermit ::Draw(float dt) {
		
		anims.Update(dt);
const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);
		
	}
	bool Hermit::Update(float dt) {
		Draw(dt);
		if (!isGettingTouched) {
			// Solo si no estaba ya en idle
			if (currentAnimName != "idle") {
				anims.SetCurrent("idle");
				currentAnimName = "idle";
			}
		}
		else {
			// Si tocamos la E estando en idle, "start"
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && currentAnimName == "idle") {
				anims.SetCurrent("start");
				currentAnimName = "start";
			}
			// Si ya no estamos hablando, "end"
			else if (currentAnimName == "talk" && !Engine::GetInstance().scene->someoneIsTalking) {
				anims.SetCurrent("end");
				currentAnimName = "end";
			}

			// Transiciones cuando terminan los fotogramas
			if (currentAnimName == "start" && anims.HasFinished()) {
				anims.SetCurrent("talk");
				currentAnimName = "talk";
			}
			else if (currentAnimName == "end" && anims.HasFinished()) {
				anims.SetCurrent("idle");
				currentAnimName = "idle";
			}
		}

		//draw
		anims.Update(dt);
		if (texture != nullptr) {
			SDL_Rect rect = anims.GetCurrentFrame();
			int drawX = (int)position.getX() - (texW / 2);
			int drawY = (int)position.getY() - (texH / 2);
			Engine::GetInstance().render->DrawTexture(texture, drawX, drawY, &rect);
		}


		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
				int i = 3383;

			}


			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedHermit == false) { //primer dialogo solo sale una vez

				if (dialogue.AvanzarDialogo(dt) == true) {

					Engine::GetInstance().scene->hasTalkedHermit = true;
					Engine::GetInstance().scene->misiones.push("Hermit", Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Botanist1.png"), Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Botanist2.png"));


				}


				return true;


				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}


			//dialogo si le ha traido el primer objeto
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && py != nullptr && Engine::GetInstance().scene->inventario.tieneObjeto("SpringWater") && Engine::GetInstance().scene->springWaterHermit == false) { //primer dialogo solo sale una vez

				if (level1.AvanzarDialogo(dt) == true) {
					Engine::GetInstance().scene->springWaterHermit = true;
					Engine::GetInstance().scene->inventario.eliminarObjeto("SpringWater");


				}


				return true;
			}
			if (level1.hasStarted && !level1.hasEnded) {
				level1.Draw(dt);
				return true;

			}

			//Dialogo si 2ndo objeto
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && py != nullptr && Engine::GetInstance().scene->inventario.tieneObjeto("HorseskinManure") == true && Engine::GetInstance().scene->HorsekinManureHermit == false) { //primer dialogo solo sale una vez

				if (level2.AvanzarDialogo(dt) == true) {

					Engine::GetInstance().scene->HorsekinManureHermit = true;
					Engine::GetInstance().scene->inventario.eliminarObjeto("HorseskinManure");
				}



				return true;
			}
			if (level2.hasStarted && !level2.hasEnded) {
				level2.Draw(dt);
				return true;

			}

			//Dialogo si tercer objeto

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && py != nullptr && Engine::GetInstance().scene->inventario.tieneObjeto("Gargantuan") && Engine::GetInstance().scene->GargantuanHermit == false) { //primer dialogo solo sale una vez


				if (level3.AvanzarDialogo(dt) == true) {


					Engine::GetInstance().scene->GargantuanHermit = true;
					Engine::GetInstance().scene->inventario.eliminarObjeto("Gargantuan");

					return true;



					return true;
				}
			}
			if (level3.hasStarted && !level3.hasEnded) {
				level3.Draw(dt);
				return true;

			}


			//Dialogo si los tiene todos

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && py != nullptr && Engine::GetInstance().scene->HorsekinManureHermit && Engine::GetInstance().scene->springWaterHermit && Engine::GetInstance().scene->GargantuanHermit && !Engine::GetInstance().scene->finishedmissionHermit) { //primer dialogo solo sale una vez

				if (hasAll.AvanzarDialogo(dt)) {

					Engine::GetInstance().scene->finishedmissionHermit = true;

				}


				return true;
			}
			if (hasAll.hasStarted && !hasAll.hasEnded) {
				hasAll.Draw(dt);
				return true;

			}


			//Dialogo si ya ha acabado la mision

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && hasAll.hasEnded && py != nullptr && Engine::GetInstance().scene->finishedmissionHermit) { //primer dialogo solo sale una vez


				if (hasAllFinished.AvanzarDialogo(dt)) {

					Engine::GetInstance().scene->finishedmissionHermit = true;
					Engine::GetInstance().scene->cards.push("Hermit", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Hermit.png"), nullptr);
					Engine::GetInstance().scene->misiones.Completed("Hermit");



					return true;
				}
			}
			if (hasAllFinished.hasStarted && !hasAllFinished.hasEnded) {
				hasAllFinished.Draw(dt);
				return true;

			}
			//Dialogo si esta a medias 
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedHermit == true) {
				notAdvanced.AvanzarDialogo(dt);



				return true;


			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}



		}
				
			
		
	}
	bool Hermit::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Hermit::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			
			break;
		}


	}
	void Hermit::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}
	//Empress in disguise, wells echo

	Well::Well() :NPC(EntityType::NPC) {



		Dialogue paperDialogue("assets/Dialogues/Mission_Well/Well_Initial_Dialogues.txt", "assets/Dialogues/Mission_Well/Well_Initial_Names.txt"); //Primer Diálogo
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Mission_Well/Well_NotAdvanced_Dialogues.txt", "assets/Dialogues/Mission_Well/Well_NotAdvanced_Names.txt"); //Dialogo All psalms
		this->level1 = secondDialogue;

		Dialogue percent("assets/Dialogues/Mission_Well/Well_MissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Well/Well_MissionCompleted_Names.txt"); //Tiene todos los poemas
		this->notAdvanced = percent;
		Dialogue lvll2("assets/Dialogues/Mission_Well/Well_AfterMissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Well/Well_AfterMissionCompleted_Names.txt"); //ya ha completado la mision
		this->level2 = lvll2;
		Dialogue third("assets/Dialogues/Mission_Well/Well_MissingPages_Dialogues.txt", "assets/Dialogues/Mission_Well/Well_MissingPages_Names.txt"); //Algunas piuezas del poema
		this->level3 = third;

	

	}

	Well::~Well()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Well::Awake() {
		return true;
	}
	bool Well::Start() {

		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"},{15, "start"}, {30, "talk"},{45, "end"}
		};
		anims.LoadFromTSX("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
		texW = 128*3;
		texH = 128*3;
		pbody = nullptr;
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
			pbody->ctype = ColliderType::NPC;
		}


		return true;
	}
	void Well::Draw(float dt) {
		if (texture == nullptr) { return; }
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

	}
	bool Well::Update(float dt) {
		Draw(dt);


		//draw
		anims.Update(dt);
		if (texture != nullptr) {
			SDL_Rect rect = anims.GetCurrentFrame();
			int drawX = (int)position.getX() - (texW / 2);
			int drawY = (int)position.getY() - (texH / 2);
			Engine::GetInstance().render->DrawTexture(texture, drawX, drawY, &rect);
		}

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);
			
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedWell == false) { //primer dialogo solo sale una vez


				
					if (dialogue.AvanzarDialogo(dt)) {
					
						Engine::GetInstance().scene->hasTalkedWell = true;
						Engine::GetInstance().scene->misiones.push("Well", Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_VoiceWell1.png"), Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_VoiceWell2.png"));

					}
					
					return true;
				
			


				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}

			// No tiene ninguna pieza del poema

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->dawn == false && Engine::GetInstance().scene->night == false && Engine::GetInstance().scene->day == false && Engine::GetInstance().scene->dusk == false) { //primer dialogo solo sale una vez


				level1.AvanzarDialogo(dt);

				return true;
			}
			if (level1.hasStarted && !level1.hasEnded) {
				level1.Draw(dt);
				return true;

			}

			//ha completado el poema

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasAllPoems == true && Engine::GetInstance().scene->hasShownPoemToWell == false) { //primer dialogo solo sale una vez

				if (notAdvanced.AvanzarDialogo(dt)) {
				
				
					Engine::GetInstance().scene->hasShownPoemToWell = true;

					Engine::GetInstance().scene->misiones.Completed("Well");
				
				}
				

				return true;
			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}

			//Ha completado el poema y ya ha hablado con el well una vez

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasAllPoems == true && Engine::GetInstance().scene->hasShownPoemToWell == true) { 


				level2.AvanzarDialogo(dt);

				return true;
			}
			if (level2.hasStarted && !level2.hasEnded) {
				level2.Draw(dt);
				return true;

			}

			//Else (tiene algunas piezas del poema)

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) { 


				level3.AvanzarDialogo(dt);

				return true;
			}
			if (level3.hasStarted && !level3.hasEnded) {
				level3.Draw(dt);
				return true;

			}
		}





		return true;
	}
	bool Well::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Well::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;

			break;
		}


	}
	void Well::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	//Level 3 NPC

	Hierophant::Hierophant() :NPC(EntityType::HERMIT) {



		Dialogue paperDialogue("assets/Dialogues/Hierophant/Hierophant_Inital_Dialogues.txt", "assets/Dialogues/Hierophant/Hierophant_Inital_Names.txt"); //Primer Diálogo
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Hierophant/Hierophant_AllPsalms_Dialogues.txt", "assets/Dialogues/Hierophant/Hierophant_AllPsalms_Names.txt"); //Dialogo All psalms
		this->level1 = secondDialogue;

		Dialogue percent("assets/Dialogues/Hierophant/Hierophant_AfterInital_Dialogues.txt", "assets/Dialogues/Hierophant/Hierophant_AfterInital_Names.txt"); //Dialogo despues de segunda interaccion
		this->notAdvanced = percent;
		Dialogue lvll2("assets/Dialogues/Hierophant/Hierophant_AllPsalms_Dialogues.txt", "assets/Dialogues/Hierophant/Hierophant_AllPsalms_Names.txt"); //Whistleblower has read all psalms
		this->level2 = lvll2;
		Dialogue third("assets/Dialogues/Hierophant/Hierophant_BeforeBoss_Dialogues.txt", "assets/Dialogues/Hierophant/Hierophant_BeforeBoss_Names.txt"); //Has not defeated 
		this->level3 = third;

		Dialogue hasAll("assets/Dialogues/Hierophant/Hierophant_AfterBoss_Dialogues.txt", "assets/Dialogues/Hierophant/Hierophant_AfterBoss_Names.txt"); //Has defeated
		this->hasAll = hasAll;
	
	}

	Hierophant::~Hierophant()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Hierophant::Awake() {
		return true;
	}
	bool Hierophant::Start() {


		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"},{10, "talk"}
		};


		anims.LoadFromTSX("assets/Textures/Spritesheets/Hierophant/spritesheet_hierophant.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Hierophant/sprite_hierophant_01.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		texW = 128*3;
		texH = 128*4;
		pbody = nullptr;
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
			pbody->ctype = ColliderType::NPC;
		}

		return true;
	}
	void Hierophant::Draw(float dt) {
		if (texture==nullptr) { return; }
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

	}
	bool Hierophant::Update(float dt) {

		if (!isGettingTouched) {
			// Siempre en idle
			if (currentAnimName != "idle") {
				anims.SetCurrent("idle");
				currentAnimName = "idle";
			}
		}
		else {
			// Si pulsamos E para hablar, cambiamos directamente a "talk"
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && currentAnimName == "idle") {
				anims.SetCurrent("talk");
				currentAnimName = "talk";
			}
			// Si estabamos hablando pero el dialogo general de la escena ha terminado, volvemos a "idle"
			else if (currentAnimName == "talk" && !Engine::GetInstance().scene->someoneIsTalking) {
				anims.SetCurrent("idle");
				currentAnimName = "idle";
			}
		}

		Draw(dt);

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->talkedTiredPreacher == false) { //primer dialogo solo sale una vez


				
					if (dialogue.AvanzarDialogo(dt)) {

						Engine::GetInstance().scene->talkedTiredPreacher = true;
						Engine::GetInstance().scene->misiones.push("Psalms", Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Hierophant1.png"), Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Hierophant2.png"));
						

					}
					
				

				return true;
			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}

			//se ha leido todos los psalmos

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasReadAllPsalms == true && Engine::GetInstance().scene->hasTalkedAboutPsalmsB4 == false) { //primer dialogo solo sale una vez


				
					if (level1.AvanzarDialogo(dt)) {

						Engine::GetInstance().scene->hasTalkedAboutPsalmsB4 = true;
					}
					return true;
				
			}
			if (level1.hasStarted && !level1.hasEnded) {
				level1.Draw(dt);
				return true;

			}

			//No se ha leido los psalmos

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasReadAllPsalms == false) {


				notAdvanced.AvanzarDialogo(dt);


				return true;
			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}




		

		//WhistleBlower dialogue
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedAboutPsalmsB4 == true && Engine::GetInstance().scene->hasBeenWhistledblowed == false) {


				
				if (level2.AvanzarDialogo(dt)) {

					Engine::GetInstance().scene->hasBeenWhistledblowed = true;
				}
				return true;
			
			return true;
			}
			if (level2.hasStarted && !level2.hasEnded) {
			level2.Draw(dt);
			return true;

			}

		//Has he been whistleblown and has the high prietest been defeates?

		//NO
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasBeenWhistledblowed == true && Engine::GetInstance().scene->DefeatedHighPrietest == false) {


			
				if (level3.AvanzarDialogo(dt)) {

					Engine::GetInstance().scene->hasBeenWhistledblowed = true;
				}
			

			return true;
			}
			if (level3.hasStarted && !level3.hasEnded) {
			level3.Draw(dt);
			return true;

			}

		//SÍ
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasBeenWhistledblowed == true && Engine::GetInstance().scene->DefeatedHighPrietest == true) {


				if (hasAll.AvanzarDialogo(dt)) {

					SDL_Texture* help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemGargantualTreeRoot1_.png");
					Engine::GetInstance().scene->inventario.push("EmpressKey1", help, nullptr);
					Engine::GetInstance().scene->misiones.Completed("Psalms");
					Engine::GetInstance().scene->cards.push("Hierophant", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Hierophant.png"), nullptr);
				}
				return true;
			
			return true;
			}
			if (hasAll.hasStarted && !hasAll.hasEnded) {
			hasAll.Draw(dt);
			return true;
	
			}

		}

	


		return true;
	}
	bool Hierophant::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Hierophant::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;

			break;
		}


	}
	void Hierophant::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	//Little timmy mission
	Timmy::Timmy() :NPC(EntityType::NPC) {




	}
	Timmy::~Timmy()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}

	void Timmy::WhatPositionIsIt(int i) {
		Dialogue Placeholder;
		switch (i) {
		
		case 0:
			
			dialogue .AddDialogue("assets/Dialogues/Mission_Finley/Finley_Initial_Dialogues.txt");
			dialogue.AddName("assets/Dialogues/Mission_Finley/Finley_Initial_Names.txt");

			
	
			AfterBoss.AddDialogue("assets/Dialogues/Mission_Finley/Finley_MissionCompleted_Dialogues.txt");
			AfterBoss.AddName("assets/Dialogues/Mission_Finley/Finley_MissionCompleted_Names.txt");
		
			
			hasFinished.AddDialogue("assets/Dialogues/Mission_Finley/Finley_MissionCompleted_Dialogues.txt");
			hasFinished.AddName("assets/Dialogues/Mission_Finley/Finley_MissionCompleted_Names.txt");
		
			hasAllFinished.AddDialogue("assets/Dialogues/Mission_Finley/Finley_AfterMissionCompleted_Dialogues.txt");
			hasAllFinished.AddName("assets/Dialogues/Mission_Finley/Finley_AfterMissionCompleted_Names.txt");
			break;

		case 1:
			
			dialogue.AddDialogue("assets/Dialogues/Mission_Finley/Finley_FirstTimeFound_Dialogues.txt");
			dialogue.AddName("assets/Dialogues/Mission_Finley//Finley_FirstTimeFound_Names.txt");

			break;

		case 2:
			
			dialogue.AddDialogue("assets/Dialogues/Mission_Finley/Finley_SecondTimeFound_Dialogues.txt");
			dialogue.AddName("assets/Dialogues/Mission_Finley/Finley_SecondTimeFound_Names.txt");
			break;
		case 3:
			
			dialogue.AddName("assets/Dialogues/Mission_Finley/Finley_ThirdTimeFound_Names.txt");
			dialogue.AddDialogue("assets/Dialogues/Mission_Finley/Finley_ThirdTimeFound_Dialogues.txt");

			break;

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		}
	
	
	
	}
	bool Timmy::Awake() {
		return true;
	}
	bool Timmy::Start() {

		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"}
		};

		anims.LoadFromTSX("assets/Textures/Spritesheets/Little Finley_NPC/finley.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Little Finley_NPC/Finley_spritesheet.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		texW = 128*2;
		texH = 128*3;
		pbody = nullptr;

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
			pbody->ctype = ColliderType::NPC;
		}

		return true;
	}
	void Timmy::Draw(float dt) {
		if (texture == nullptr) { return; }
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

	}
	bool Timmy::Update(float dt) {

		if (hidingPlace != Engine::GetInstance().scene->hidingPlaceATM) {
			return true;
		}

		Draw(dt);

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

			switch (hidingPlace) {
			
			case 0:

				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == false) {


					
						if (dialogue.AvanzarDialogo(dt)) {

							Engine::GetInstance().scene->hidingPlaceATM = 1;
							Engine::GetInstance().scene->misiones.push("Timmy", Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_LittleFinley1.png"), Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_LittleFinley2.png"));

						}
					

					return true;
				}
				if (dialogue.hasStarted && !dialogue.hasEnded) {
					dialogue.Draw(dt);
					return true;

				}
				//AfterMission
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == true && Engine::GetInstance().scene->hasTalkedToTimmyOnce == false) {


					
						if (hasFinished.AvanzarDialogo(dt)) {

							Engine::GetInstance().scene->hasTalkedToTimmyOnce = true;
							Engine::GetInstance().scene->misiones.Completed("Timmy");
						}
					

					return true;
				}
				if (hasFinished.hasStarted && !hasFinished.hasEnded) {
					hasFinished.Draw(dt);
					return true;

				}
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == true && Engine::GetInstance().scene->hasTalkedToTimmyOnce == true && Engine::GetInstance().scene->DefeatedHighPrietest == true && Engine::GetInstance().scene->talkedOnceAfterDefeatBoss == false) {


					
						if (AfterBoss.AvanzarDialogo(dt)) {

							Engine::GetInstance().scene->talkedOnceAfterDefeatBoss = true;
						}
					
					return true;
				}
				if (AfterBoss.hasStarted && !AfterBoss.hasEnded) {
					AfterBoss.Draw(dt);
					return true;

				}

				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == true && Engine::GetInstance().scene->hasTalkedToTimmyOnce == true) {


				
						if (hasAllFinished.AvanzarDialogo(dt)) {

							Engine::GetInstance().scene->hasTalkedToTimmyOnce = true;
						}
					
					return true;
				}
				if (hasAllFinished.hasStarted && !hasAllFinished.hasEnded) {
					hasAllFinished.Draw(dt);
					return true;

				}









				break;

			case 1:
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 1) {


						if (dialogue.AvanzarDialogo(dt)) {

							Engine::GetInstance().scene->hidingPlaceATM = 2;
						}
					
					return true;
				}
				if (dialogue.hasStarted && !dialogue.hasEnded) {
					dialogue.Draw(dt);
					return true;

				}
				break;

			case 2:
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 2) {


					
						if (dialogue.AvanzarDialogo(dt)) {

							Engine::GetInstance().scene->hidingPlaceATM = 3;
							
						}
					
					return true;
				}
				if (dialogue.hasStarted && !dialogue.hasEnded) {
					dialogue.Draw(dt);
					return true;

				}
				break;
			case 3:
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 3) {


					
						if (dialogue.AvanzarDialogo(dt)) {

							Engine::GetInstance().scene->hidingPlaceATM = 0;
							Engine::GetInstance().scene->hasFoundTimmyThreeTimes = true;
						}
						

					return true;
				}
				if (dialogue.hasStarted && !dialogue.hasEnded) {
					dialogue.Draw(dt);
					return true;

				}
				break;


			
			
			}
		}




		return true;
	}
	bool Timmy::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Timmy::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;

			break;
		}


	}
	void Timmy::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	//Empress
	Empress::Empress() :NPC(EntityType::NPC) {

		Dialogue paperDialogue("assets/Dialogues/Mission_Empress/Empress_Initial_NOP_Dialogues.txt", "assets/Dialogues/Mission_Empress/Empress_Initial_NOP_Names.txt"); //Primer Diálogo
		this->regular = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Mission_Empress/Empress_Initial_Dialogues.txt", "assets/Dialogues/Mission_Empress/Empress_Initial_Names.txt"); //trusted
		this->trusted = secondDialogue;

		Dialogue percent("assets/Dialogues/Mission_Empress/Empress_MissingArtifact_Dialogues.txt", "assets/Dialogues/Mission_Empress/Empress_MissingArtifact_Names.txt"); //Dialogo despues de segunda interaccion
		this->scolding = percent;
		Dialogue lvll2("assets/Dialogues/Mission_Empress/Empress_AllArtifact_Dialogues.txt", "assets/Dialogues/Mission_Empress/Empress_AllArtifact_Names.txt"); //Whistleblower has read all psalms
		this->GivesArtifact = lvll2;
		Dialogue third("assets/Dialogues/Mission_Empress/Empress_MissingArtifact_Dialogues.txt", "assets/Dialogues/Mission_Empress/Empress_MissingArtifact_Names.txt"); //Has not defeated 
		this->AfterMission = third;



	}
	Empress::~Empress()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}

	
	bool Empress::Awake() {
		return true;
	}
	bool Empress::Start() {

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/empress_queen/empress_01_idle.png");

		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"}
		};

		anims.LoadFromTSX("assets/Textures/Spritesheets/empress_queen/empress_idle.tsx", aliases);
		anims.SetCurrent("idle");


		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		texW = 128;
		texH = 128;
		pbody = nullptr;

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
			pbody->ctype = ColliderType::NPC;
		}

		return true;
	}
	void Empress::Draw(float dt) {
		if (texture == nullptr) { return; }
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

	}
	bool Empress::Update(float dt) {

		if (Engine::GetInstance().scene->hasSparedPrincessAndKnight == false) {
			return true;
		}

		Draw(dt);

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			//trusted dialogue
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasShownPoemToWell == true && Engine::GetInstance().scene->talkedTwiceEmpress == false) {


				if (trusted.AvanzarDialogo(dt)) {


					Engine::GetInstance().scene->talkedTwiceEmpress = true;
					Engine::GetInstance().scene->EmpressTrustedDialogue = true;
					Engine::GetInstance().scene->misiones.push("Empress", Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Queen1.png"), Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_MissionNotes_Queen1.png"));


				}

				return true;
			}
			if (trusted.hasStarted && !trusted.hasEnded) {
				trusted.Draw(dt);
				return true;

			}
			//has artifact
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasShownPoemToWell == true && Engine::GetInstance().scene->hasAllFragments == true && !Engine::GetInstance().scene->inventario.tieneObjeto("Artifact") && Engine::GetInstance().scene->talkedTwiceEmpress == true) {


				if (GivesArtifact.AvanzarDialogo(dt)) {


					SDL_Texture* help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ArtifactAssembled2_.png");
					Engine::GetInstance().scene->inventario.push("Artifact", help, nullptr);
					Engine::GetInstance().scene->inventario.eliminarObjeto("Bloody");
					Engine::GetInstance().scene->inventario.eliminarObjeto("Moldy");
					Engine::GetInstance().scene->inventario.eliminarObjeto("Core");
					Engine::GetInstance().scene->inventario.eliminarObjeto("Rusty");
					Engine::GetInstance().scene->misiones.Completed("Empress");
					Engine::GetInstance().scene->cards.push("Empress", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Empress.png"), nullptr);
				}


				return true;
			}
			if (GivesArtifact.hasStarted && !GivesArtifact.hasEnded) {
				GivesArtifact.Draw(dt);
				return true;

			}

			//has ntartifact
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasShownPoemToWell == true && Engine::GetInstance().scene->talkedTwiceEmpress == true) {




				if (scolding.AvanzarDialogo(dt)) {





				}




				return true;
			}
			if (scolding.hasStarted && !scolding.hasEnded) {
				scolding.Draw(dt);
				return true;



				//After Mission
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasAllFragments == true && Engine::GetInstance().scene->inventario.tieneObjeto("Artifact")) {


					if (AfterMission.AvanzarDialogo(dt)) {


					}

					return true;
				}
				if (AfterMission.hasStarted && !AfterMission.hasEnded) {
					AfterMission.Draw(dt);
					return true;

				}
				//regular
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasShownPoemToWell == false) {


					if (regular.AvanzarDialogo(dt)) {


						Engine::GetInstance().scene->hasTalkedOnceEmpress = true;
					}


					return true;
				}
				if (regular.hasStarted && !regular.hasEnded) {
					regular.Draw(dt);
					return true;

				}
			}
		}



		return true;
	}
	bool Empress::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Empress::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;

			break;
		}


	}
	void Empress::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	//TELEPORT MISSION
	RatKing::RatKing() :NPC(EntityType::NPC) {

		/*Dialogue regular;
		Dialogue trusted;
		Dialogue scolding;
		Dialogue GivesArticact;
		Dialogue AfterMission;*/

		Dialogue paperDialogue("assets/Dialogues/Mission_Chariot/Chariot_Initial_Dialogues.txt", "assets/Dialogues/Mission_Chariot/Chariot_Initial_Names.txt"); //Primer Diálogo
		this->initial = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Mission_Chariot/Chariot_NotAdvanced_Dialogues.txt", "assets/Dialogues/Mission_Chariot/Chariot_NotAdvanced_Names.txt"); //Dialogo All psalms
		this->notAdvanced = secondDialogue;

		Dialogue percent("assets/Dialogues/Mission_Chariot/Chariot_MissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Chariot/Chariot_MissionCompleted_Names.txt"); //Dialogo despues de segunda interaccion
		this->completed = percent;
		Dialogue lvll2("assets/Dialogues/Mission_Chariot/Chariot_BeforeSelectingDestination_Dialogues.txt", "assets/Dialogues/Mission_Chariot/Chariot_BeforeSelectingDestination_Dialogues.txt"); //Whistleblower has read all psalms
		this->teleport = lvll2;
		




	}
	RatKing::~RatKing()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}


	bool RatKing::Awake() {
		return true;
	}
	bool RatKing::Start() {

		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"}
		};

		// ¡Ojo con las rutas! Cámbialas por las reales del Rey Rata
		anims.LoadFromTSX("assets/Textures/Spritesheets/rat king/ratking_idle.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/rat king/sprite_ratking_01_idle.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		texW = 384;
		texH = 384;
		pbody = nullptr;

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
			pbody->ctype = ColliderType::NPC;
		}

		return true;
	}
	void RatKing::Draw(float dt) {
		if (texture == nullptr) { return; }
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

	}
	bool RatKing::Update(float dt) {

		Draw(dt);

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			//INITIAL dialogue
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->ratTalkedOnce == false) {

				
					if (initial.AvanzarDialogo(dt)) {


						Engine::GetInstance().scene->ratTalkedOnce = true;
						Engine::GetInstance().scene->cards.push("Chariot", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Chariot.png"), nullptr);
					

						Engine::GetInstance().scene->misiones.push("Chariot", Engine::GetInstance().textures->Load("assets/UI/Spritesheets/UI_Mission_Info/UI_MissionNotes_Teleport1.png"), Engine::GetInstance().textures->Load("assets/UI/Spritesheets/UI_Mission_Info/UI_MissionNotes_Teleport2.png"));

						
					}
					return true;
				
				return true;
			}
			if (initial.hasStarted && !initial.hasEnded) {
				initial.Draw(dt);
				return true;

			}
			//Not advanced
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->ratmissionfinished==false) {

				
					if (notAdvanced.AvanzarDialogo(dt)) {


					

					}
					
				return true;
			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}
			//Completed mission
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->ratmissionfinished == true && Engine::GetInstance().scene->teleportUnlocked == false) {

				
					if (completed.AvanzarDialogo(dt)) {


						Engine::GetInstance().scene->teleportUnlocked = true;
						Engine::GetInstance().scene->misiones.Completed("Chariot");
						Engine::GetInstance().scene->cards.push("Chariot", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Chariot.png"), nullptr);
					}
					
				return true;
			}
			if (completed.hasStarted && !completed.hasEnded) {
				completed.Draw(dt);
				return true;

			}

			//opens teleport
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->ratmissionfinished == true && Engine::GetInstance().scene->teleportUnlocked == true) {

				
					if (teleport.AvanzarDialogo(dt)) {
						// Abrir el menú de teletransporte después del diálogo
						switch (level) {

						case 1:
							Engine::GetInstance().scene->rat1 = true;
							break;
						case 2:
							Engine::GetInstance().scene->rat2 = true;
							break;
						case 3:
							Engine::GetInstance().scene->rat3 = true;
							break;
						case 4:
							Engine::GetInstance().scene->rat4 = true;
							break;

						}
						Engine::GetInstance().scene->SetTeleport(true);
					}
					
				return true;
			}
			if (teleport.hasStarted && !teleport.hasEnded) {
				teleport.Draw(dt);
				return true;

			}
		}



		return true;
	}
	bool RatKing::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void RatKing::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;

			break;
		case ColliderType::CHEESEBALL:
			Engine::GetInstance().scene->ratmissionfinished=true;
			break;
		}

	}
	void RatKing::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	//MISSION SCULPTOR
	Sculptor::Sculptor() :NPC(EntityType::NPC) {

		/*Dialogue regular;
		Dialogue trusted;
		Dialogue scolding;
		Dialogue GivesArticact;
		Dialogue AfterMission;*/

		Dialogue paperDialogue("assets/Dialogues/Mission_Sculptor/Sculptor_Initial_Dialogues.txt", "assets/Dialogues/Mission_Sculptor/Sculptor_Initial_Names.txt"); //Primer Diálogo
		this->initial = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Mission_Sculptor/Sculptor_NotAdvanced_Dialogues.txt", "assets/Dialogues/Mission_Sculptor/Sculptor_NotAdvanced_Names.txt"); //No avanzado
		this->notAdvanced = secondDialogue;

		Dialogue percent("assets/Dialogues/Mission_Sculptor/Sculptor_MissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Sculptor/Sculptor_MissionCompleted_Names.txt"); //Acabado
		this->completed = percent;
		Dialogue lvll2("assets/Dialogues/Mission_Sculptor/Sculptor_BringStatue_Dialogues.txt", "assets/Dialogues/Mission_Sculptor/Sculptor_BringStatue_Names.txt"); //Bring one statue
		this->teleport = lvll2;

		Dialogue aa("assets/Dialogues/Mission_Sculptor/Sculptor_AfterMissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Sculptor/Sculptor_AfterMissionCompleted_Names.txt"); //AfterFinished
		this->AfterMission = aa;




	}
	Sculptor::~Sculptor()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}


	bool Sculptor::Awake() {
		return true;
	}
	bool Sculptor::Start() {

		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"}
		};

		anims.LoadFromTSX("assets/Textures/Spritesheets/Frustated Sculptor_NPC/frustatedSculptor.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Frustated Sculptor_NPC/Frustated_Sculptor.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		texW = 384;
		texH = 384+ 128;
		pbody = nullptr;

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
			pbody->ctype = ColliderType::NPC;
		}

		return true;

	}	void Sculptor::Draw(float dt) {
		if (texture == nullptr) { return; }
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

	}
	bool Sculptor::Update(float dt) {

		Draw(dt);

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			//INITIAL dialogue
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedSculptor == false) {

				
					if (initial.AvanzarDialogo(dt)) {


						Engine::GetInstance().scene->hasTalkedSculptor = true;
						Engine::GetInstance().scene->misiones.push("Sculptor", Engine::GetInstance().textures->Load("assets/UI/Spritesheets/UI_Mission_Info/UI_MissionNotes_Sculptor1.png"), Engine::GetInstance().textures->Load("assets/UI/Spritesheets/UI_Mission_Info/UI_MissionNotes_Sculptor2.png"));



					}
					

				return true;
			}
			if (initial.hasStarted && !initial.hasEnded) {
				initial.Draw(dt);
				return true;

			}
			//Not advanced
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->ratmissionfinished == false) {

				
					if (notAdvanced.AvanzarDialogo(dt)) {




					}
					
				return true;
			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}
			//Completed mission
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->finishedMissionSculptor == false ) { //y mirar si tiene cadáveres???

				
					if (completed.AvanzarDialogo(dt)) {


						Engine::GetInstance().scene->finishedMissionSculptor = true;

					}
					return true;
				

				return true;
			}
			if (completed.hasStarted && !completed.hasEnded) {
				completed.Draw(dt);
				return true;

			}

			//opens teleport
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN  && Engine::GetInstance().scene->finishedMissionSculptor == true) {

				
					if (teleport.AvanzarDialogo(dt)) {
						Engine::GetInstance().scene->misiones.Completed("Sculptor");
					}
				

				return true;
			}
			if (teleport.hasStarted && !teleport.hasEnded) {
				teleport.Draw(dt);
				return true;

			}
		}



		return true;
	}
	bool Sculptor::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void Sculptor::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;

			break;
		
		}

	}
	void Sculptor::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

	//MISSION KNIGHT
	RetiredKnight::RetiredKnight() : NPC(EntityType::NPC) {

		/*Dialogue regular;
		Dialogue trusted;
		Dialogue scolding;
		Dialogue GivesArticact;
		Dialogue AfterMission;*/

		Dialogue paperDialogue("assets/Dialogues/Mission_Strength/Strength_Inital_Dialogues.txt", "assets/Dialogues/Mission_Strength/Strength_Inital_Names.txt"); //Primer Diálogo
		this->initial = paperDialogue;
		Dialogue secondDialogue("assets/Dialogues/Mission_Strength/Strength_NotAdvanced_Dialogues.txt", "assets/Dialogues/Mission_Strength/Strength_NotAdvanced_Names.txt"); //Dialogo All psalms
		this->notAdvanced = secondDialogue;

		Dialogue percent("assets/Dialogues/Mission_Strength/Strength_MissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Strength/Strength_MissionCompleted_Names.txt"); //Dialogo despues de segunda interaccion
		this->completed = percent;
		Dialogue lvll2("assets/Dialogues/Mission_Strength/Strength_AfterMissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Strength/Strength_AfterMissionCompleted_Names.txt"); //Whistleblower has read all psalms
		this->teleport = lvll2;





	}
	RetiredKnight::~RetiredKnight()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}


	bool RetiredKnight::Awake() {
		return true;
	}
	bool RetiredKnight::Start() {

		std::unordered_map<int, std::string> aliases = {
					  {0, "idle"}
		};

		anims.LoadFromTSX("assets/Textures/Spritesheets/Retired Knight_NPC/retiredKnight.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Retired Knight_NPC/spritesheet.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		texW = 384;
		texH = 384;
		pbody = nullptr;

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
			pbody->ctype = ColliderType::NPC;
		}


		return true;
	}
	void RetiredKnight::Draw(float dt) {
		if (texture == nullptr) { return; }
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);


		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);

	}
	bool RetiredKnight::Update(float dt) {

		Draw(dt);

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			//INITIAL dialogue
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedSculptor == false) {

			
					if (initial.AvanzarDialogo(dt)) {


						Engine::GetInstance().scene->hasTalkedSculptor = true;

						Engine::GetInstance().scene->misiones.push("Retired", Engine::GetInstance().textures->Load("assets/UI/Spritesheets/UI_Mission_Info/UI_MissionNotes_RetiredKnight1.png"), Engine::GetInstance().textures->Load("assets/UI/Spritesheets/UI_Mission_Info/UI_MissionNotes_RetiredKnight2.png"));

					}
					

				return true;
			}
			if (initial.hasStarted && !initial.hasEnded) {
				initial.Draw(dt);
				return true;

			}
			//Not advanced
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->HasKilledOneOfEachType() == false && Engine::GetInstance().scene->finishedRetiredKnight == false) {

				
					if (notAdvanced.AvanzarDialogo(dt)) {




					}
					

				return true;
			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}
			//Completed mission
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->HasKilledOneOfEachType()==true && Engine::GetInstance().scene->finishedRetiredKnight == false) { //y mirar si tiene cadáveres???

				
					if (completed.AvanzarDialogo(dt)) {


						Engine::GetInstance().scene->finishedRetiredKnight = true;
						

						Engine::GetInstance().scene->misiones.Completed("Retired");


					}
					

				return true;
			}
			if (completed.hasStarted && !completed.hasEnded) {
				completed.Draw(dt);
				return true;

			}

			//After finished
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->finishedRetiredKnight == true) {

			
					if (teleport.AvanzarDialogo(dt)) {
						
					}
					return true;
				
			}
			if (teleport.hasStarted && !teleport.hasEnded) {
				teleport.Draw(dt);
				return true;

			}
		}



		return true;
	}
	bool RetiredKnight::CleanUp() {
		LOG("Unloading Hermit");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}
	void RetiredKnight::OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;

			break;
		}

	}
	void RetiredKnight::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}



	HighPriestess::HighPriestess() : NPC(EntityType::HIGHPRIESTESS)
	{
		Dialogue dialogue1("assets/Dialogues/HighPriestess/HighPriestess_Initial_Dialogues.txt", "assets/Dialogues/HighPriestess/HighPriestess_Initial_Names.txt");
		dialogue = dialogue1;
		Dialogue dialogue2("assets/Dialogues/HighPriestess/HighPriestess_AfterInitial_Dialogues.txt", "assets/Dialogues/HighPriestess/HighPriestess_AfterInitial_Names.txt");
		dos = dialogue2;
	}

	HighPriestess::~HighPriestess()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}

	bool HighPriestess::Awake()
	{
		return true;
	}

	bool HighPriestess::Start()
	{
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		// De momento solo idle
		std::unordered_map<int, std::string> aliases = {
				  {0, "idle"}
		};

		anims.LoadFromTSX("assets/Textures/Spritesheets/High Priestess/high_priestess_idle.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("resources/spritesheets/High Priestess/priestess_idle_01.png");
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		texW = 128;
		texH = 128;
		pbody = nullptr;

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
			pbody->ctype = ColliderType::NPC;
		}

		return true;
	}

	void HighPriestess::Draw(float dt)
	{
		if (texture == nullptr) return;
		anims.Update(dt);
		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		int x, y;
		pbody->GetPosition(x, y);
		position.setX((float)x);
		position.setY((float)y);

		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);
	}

	bool HighPriestess::Update(float dt)
	{
		if (!active) return true;

		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);
		
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedPreach == false) {
			
			
			
				if (dialogue.AvanzarDialogo(dt)) {
					Engine::GetInstance().scene->hasTalkedPreach = true;
				}
				return true;

			}
			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		
		
		
		
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN ) {



				if (dos.AvanzarDialogo(dt)) {

				}
				return true;

			}
			if (dos.hasStarted && !dos.hasEnded) {
				dos.Draw(dt);
				return true;

			}
		
		
		
		
		
		
		}
		Draw(dt);
		return true;
	}

	bool HighPriestess::CleanUp()
	{
		LOG("Unloading High Priestess");
		Engine::GetInstance().textures->UnLoad(texture);
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		return true;
	}

	void HighPriestess::OnCollision(PhysBody* physA, PhysBody* physB)
	{
		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}
	}

	void HighPriestess::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
	{
		isGettingTouched = false;
	}