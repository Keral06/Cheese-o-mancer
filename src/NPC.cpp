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
	bool CowWeb::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


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

				if (secondDialogue.hasStarted) {

					secondDialogue.NextDialogue();
					secondDialogue.Draw(dt);
					
					return true;
				}
				secondDialogue.BeginDialogue();
				secondDialogue.Draw(dt);


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

				if (secondDialogue.hasStarted) {

					secondDialogue.NextDialogue();
					secondDialogue.Draw(dt);

					return true;
				}
				secondDialogue.BeginDialogue();
				secondDialogue.Draw(dt);


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

				if (secondDialogue.hasStarted) {

					secondDialogue.NextDialogue();
					secondDialogue.Draw(dt);

					return true;
				}
				secondDialogue.BeginDialogue();
				secondDialogue.Draw(dt);


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
					if (PercentChance.hasStarted) {

						PercentChance.NextDialogue();
						PercentChance.Draw(dt);

						return true;
					}
					PercentChance.BeginDialogue();
					PercentChance.Draw(dt);


					return true;
				
				
				}
				if (secondDialogue.hasStarted) {

					secondDialogue.NextDialogue();
					secondDialogue.Draw(dt);

					return true;
				}
				secondDialogue.BeginDialogue();
				secondDialogue.Draw(dt);


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


				if (dialogue.hasStarted) {

					dialogue.NextDialogue();
					dialogue.Draw(dt);
					if (dialogue.hasEnded) {
					
						Engine::GetInstance().scene->hasTalkedHermit = true;
					}
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


			//dialogo si le ha traido el primer objeto
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && py!=nullptr && Engine::GetInstance().scene->inventario.tieneObjeto("SpringWater") && Engine::GetInstance().scene->springWaterHermit == false) { //primer dialogo solo sale una vez


				if (level1.hasStarted) {

					level1.NextDialogue();
					level1.Draw(dt);
					if (level1.hasEnded) { 
						Engine::GetInstance().scene->springWaterHermit = true;
						Engine::GetInstance().scene->inventario.eliminarObjeto("SpringWater");
					}
					return true;
				}
				level1.BeginDialogue();
				level1.Draw(dt);


				return true;
			}
			if (level1.hasStarted && !level1.hasEnded) {
				level1.Draw(dt);
				return true;

			}

			//Dialogo si 2ndo objeto
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN  && py != nullptr && Engine::GetInstance().scene->inventario.tieneObjeto("HorseskinManure") == true && Engine::GetInstance().scene->HorsekinManureHermit == false) { //primer dialogo solo sale una vez


				if (level2.hasStarted) {

					level2.NextDialogue();
					level2.Draw(dt);
					if (level2.hasEnded) {
						Engine::GetInstance().scene->HorsekinManureHermit = true;
						Engine::GetInstance().scene->inventario.eliminarObjeto("HorseskinManure");
					}
					return true;
				}
				level2.BeginDialogue();
				level2.Draw(dt);


				return true;
			}
			if (level2.hasStarted && !level2.hasEnded) {
				level2.Draw(dt);
				return true;

			}

			//Dialogo si tercer objeto

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN  && py != nullptr && Engine::GetInstance().scene->inventario.tieneObjeto("Gargantuan") && Engine::GetInstance().scene->GargantuanHermit == false) { //primer dialogo solo sale una vez


				if (level3.hasStarted) {

					level3.NextDialogue();
					level3.Draw(dt);
					if (level3.hasEnded) { 
						Engine::GetInstance().scene->GargantuanHermit = true;
						Engine::GetInstance().scene->inventario.eliminarObjeto("Gargantuan");
					}
					return true;
				}
				level3.BeginDialogue();
				level3.Draw(dt);


				return true;
			}
			if (level3.hasStarted && !level3.hasEnded) {
				level3.Draw(dt);
				return true;

			}

			//Dialogo si los tiene todos

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN  && py != nullptr && Engine::GetInstance().scene->HorsekinManureHermit && Engine::GetInstance().scene->springWaterHermit && Engine::GetInstance().scene->GargantuanHermit && !Engine::GetInstance().scene->finishedmissionHermit) { //primer dialogo solo sale una vez


				if (hasAll.hasStarted) {

					hasAll.NextDialogue();
					hasAll.Draw(dt);
					if (hasAll.hasEnded) { Engine::GetInstance().scene->finishedmissionHermit = true; }
					return true;
				}
				hasAll.BeginDialogue();
				hasAll.Draw(dt);


				return true;
			}
			if (hasAll.hasStarted && !hasAll.hasEnded) {
				hasAll.Draw(dt);
				return true;

			}

			//Dialogo si ya ha acabado la mision
			
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && hasAll.hasEnded && py != nullptr && Engine::GetInstance().scene->finishedmissionHermit) { //primer dialogo solo sale una vez


				if (hasAllFinished.hasStarted) {

					hasAllFinished.NextDialogue();
					hasAllFinished.Draw(dt);
					if (hasAllFinished.hasEnded) { Engine::GetInstance().scene->finishedmissionHermit = true; }
					return true;
				}
				hasAllFinished.BeginDialogue();
				hasAllFinished.Draw(dt);


				return true;
			}
			if (hasAllFinished.hasStarted && !hasAllFinished.hasEnded) {
				hasAllFinished.Draw(dt);
				return true;

			}
			//Dialogo si esta a medias 
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedHermit == true) {
				if (notAdvanced.hasStarted) {

					notAdvanced.NextDialogue();
					notAdvanced.Draw(dt);
					if (notAdvanced.hasEnded) { return true; }
					return true;
				}
				

				notAdvanced.BeginDialogue();
				notAdvanced.Draw(dt);


					return true;
				

			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}

		}
		

		return true;
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


				if (dialogue.hasStarted) {

					dialogue.NextDialogue();
					dialogue.Draw(dt);
					if (dialogue.hasEnded) {

						Engine::GetInstance().scene->hasTalkedWell = true;
					}
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

			// No tiene ninguna pieza del poema

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->dawn == false && Engine::GetInstance().scene->night == false && Engine::GetInstance().scene->day == false && Engine::GetInstance().scene->dusk == false) { //primer dialogo solo sale una vez


				if (level1.hasStarted) {

					level1.NextDialogue();
					level1.Draw(dt);
					if (level1.hasEnded) {

						
					}
					return true;
				}
				level1.BeginDialogue();
				level1.Draw(dt);


				return true;
			}
			if (level1.hasStarted && !level1.hasEnded) {
				level1.Draw(dt);
				return true;

			}

			//ha completado el poema

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasAllPoems == true && Engine::GetInstance().scene->hasShownPoemToWell == false) { //primer dialogo solo sale una vez


				if (notAdvanced.hasStarted) {

					notAdvanced.NextDialogue();
					notAdvanced.Draw(dt);
					if (notAdvanced.hasEnded) {
						Engine::GetInstance().scene->hasShownPoemToWell = true;
						
						Engine::GetInstance().scene->inventario.push("EmpressKey", Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemSacredSpringWater1_.png"), nullptr); //PLACEHOLDER

					}
					return true;
				}
				notAdvanced.BeginDialogue();
				notAdvanced.Draw(dt);


				return true;
			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}

			//Ha completado el poema y ya ha hablado con el well una vez

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasAllPoems == true && Engine::GetInstance().scene->hasShownPoemToWell == true) { 


				if (level2.hasStarted) {

					level2.NextDialogue();
					level2.Draw(dt);
					if (level2.hasEnded) {
						

					}
					return true;
				}
				level2.BeginDialogue();
				level2.Draw(dt);


				return true;
			}
			if (level2.hasStarted && !level2.hasEnded) {
				level2.Draw(dt);
				return true;

			}

			//Else (tiene algunas piezas del poema)

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) { 


				if (level3.hasStarted) {

					level3.NextDialogue();
					level3.Draw(dt);
					if (level3.hasEnded) {


					}
					return true;
				}
				level3.BeginDialogue();
				level3.Draw(dt);


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

		/*std::unordered_map<int, std::string> aliases = {
				  {0, "idle"},{15, "start"}, {30, "talk"},{45, "end"}
		};
		anims.LoadFromTSX("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.png");*/
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
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
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->talkedTiredPreacher == false) { //primer dialogo solo sale una vez


				if (dialogue.hasStarted) {

					dialogue.NextDialogue();
					dialogue.Draw(dt);
					if (dialogue.hasEnded) {

						Engine::GetInstance().scene->talkedTiredPreacher = true;
					}
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

			//se ha leido todos los psalmos

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasReadAllPsalms == true && Engine::GetInstance().scene->hasTalkedAboutPsalmsB4 == false) { //primer dialogo solo sale una vez


				if (level1.hasStarted) {

					level1.NextDialogue();
					level1.Draw(dt);
					if (level1.hasEnded) {

						Engine::GetInstance().scene->hasTalkedAboutPsalmsB4 = true;
					}
					return true;
				}
				level1.BeginDialogue();
				level1.Draw(dt);


				return true;
			}
			if (level1.hasStarted && !level1.hasEnded) {
				level1.Draw(dt);
				return true;

			}

			//No se ha leido los psalmos

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasReadAllPsalms == false) {


				if (notAdvanced.hasStarted) {

					notAdvanced.NextDialogue();
					notAdvanced.Draw(dt);
					if (notAdvanced.hasEnded) {


					}
					return true;
				}
				notAdvanced.BeginDialogue();
				notAdvanced.Draw(dt);


				return true;
			}
			if (notAdvanced.hasStarted && !notAdvanced.hasEnded) {
				notAdvanced.Draw(dt);
				return true;

			}




		

		//WhistleBlower dialogue
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasTalkedAboutPsalmsB4 == true && Engine::GetInstance().scene->hasBeenWhistledblowed == false) {


				if (level2.hasStarted) {

				level2.NextDialogue();
				level2.Draw(dt);
				if (level2.hasEnded) {

					Engine::GetInstance().scene->hasBeenWhistledblowed = true;
				}
				return true;
			}
			level2.BeginDialogue();
			level2.Draw(dt);


			return true;
			}
			if (level2.hasStarted && !level2.hasEnded) {
			level2.Draw(dt);
			return true;

			}

		//Has he been whistleblown and has the high prietest been defeates?

		//NO
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasBeenWhistledblowed == true && Engine::GetInstance().scene->DefeatedHighPrietest == false) {


			if (level3.hasStarted) {

				level3.NextDialogue();
				level3.Draw(dt);
				if (level3.hasEnded) {

					Engine::GetInstance().scene->hasBeenWhistledblowed = true;
				}
				return true;
			}
			level3.BeginDialogue();
			level3.Draw(dt);


			return true;
			}
			if (level3.hasStarted && !level3.hasEnded) {
			level3.Draw(dt);
			return true;

			}

		//SÍ
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hasBeenWhistledblowed == true && Engine::GetInstance().scene->DefeatedHighPrietest == true) {


			if (hasAll.hasStarted) {

				hasAll.NextDialogue();
				hasAll.Draw(dt);
				if (hasAll.hasEnded) {

					SDL_Texture* help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemGargantualTreeRoot1_.png");
					Engine::GetInstance().scene->inventario.push("EmpressKey1", help, nullptr);
				}
				return true;
			}
			hasAll.BeginDialogue();
			hasAll.Draw(dt);


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
			 Placeholder =("assets/Dialogues/Mission_Finley/Finley_Initial_Dialogues.txt", "assets/Dialogues/Mission_Well/Finley_Initial_Names.txt"); //Primer Diálogo
			dialogue = Placeholder;


			Placeholder = ("assets/Dialogues/Mission_Finley/Finley_MissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Well/Finley_MissionCompleted_Names.txt");
			AfterBoss = Placeholder;
			Placeholder = ("assets/Dialogues/Mission_Finley/Third_FirstTimeFound_Dialogues.txt", "assets/Dialogues/Mission_Well/Third_FirstTimeFound_Names.txt");
			hasFinished = Placeholder;
			Placeholder = ("assets/Dialogues/Mission_Finley/Finley_AfterMissionCompleted_Dialogues.txt", "assets/Dialogues/Mission_Well/Finley_AfterMissionCompleted_Names.txt");

			hasAllFinished = Placeholder;
			break;

		case1:
			 Placeholder=("assets/Dialogues/Mission_Finley/Finley_FirstTimeFound_Dialogues.txt", "assets/Dialogues/Mission_Well/Finley_FirstTimeFound_Names.txt"); //Primer Diálogo
			dialogue = Placeholder;

			break;

		case 2:
			Placeholder = ("assets/Dialogues/Mission_FinleySFinley_SecondTimeFound_Dialogues.txt", "assets/Dialogues/Mission_Well/Finley_SecondTimeFound_Names.txt"); //Primer Diálogo
			dialogue = Placeholder;
			break;
		case 3:
			/*Dialogue AfterBoss;
			Dialogue hasAllFinished;
			Dialogue hasFinished;*/
			Placeholder = ("assets/Dialogues/Mission_Finley/Finley_ThirdTimeFound_Dialogues.txt", "assets/Dialogues/Mission_Well/Finley_ThirdTimeFound_Dialogues.txt");
			dialogue = Placeholder;

			break;

		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		}
	
	
	
	}
	bool Timmy::Awake() {
		return true;
	}
	bool Timmy::Start() {

		/*std::unordered_map<int, std::string> aliases = {
				  {0, "idle"},{15, "start"}, {30, "talk"},{45, "end"}
		};
		anims.LoadFromTSX("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.tsx", aliases);
		anims.SetCurrent("idle");

		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Hermit/spritesheet_Hermit.png");*/
		InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

		//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
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
			switch (hidingPlace) {
			
			case 0:

				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == false) {


					if (dialogue.hasStarted) {

						dialogue.NextDialogue();
						dialogue.Draw(dt);
						if (dialogue.hasEnded) {

							Engine::GetInstance().scene->hidingPlaceATM = 1;
						}
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
				//AfterMission
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == true && Engine::GetInstance().scene->hasTalkedToTimmyOnce == false) {


					if (hasFinished.hasStarted) {

						hasFinished.NextDialogue();
						hasFinished.Draw(dt);
						if (hasFinished.hasEnded) {

							Engine::GetInstance().scene->hasTalkedToTimmyOnce = true;
						}
						return true;
					}
					hasFinished.BeginDialogue();
					hasFinished.Draw(dt);


					return true;
				}
				if (hasFinished.hasStarted && !hasFinished.hasEnded) {
					hasFinished.Draw(dt);
					return true;

				}
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == true && Engine::GetInstance().scene->hasTalkedToTimmyOnce == true && Engine::GetInstance().scene->DefeatedHighPrietest == true && Engine::GetInstance().scene->talkedOnceAfterDefeatBoss == false) {


					if (AfterBoss.hasStarted) {

						AfterBoss.NextDialogue();
						AfterBoss.Draw(dt);
						if (AfterBoss.hasEnded) {

							Engine::GetInstance().scene->talkedOnceAfterDefeatBoss = true;
						}
						return true;
					}
					AfterBoss.BeginDialogue();
					AfterBoss.Draw(dt);


					return true;
				}
				if (AfterBoss.hasStarted && !AfterBoss.hasEnded) {
					AfterBoss.Draw(dt);
					return true;

				}

				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 0 && Engine::GetInstance().scene->hasFoundTimmyThreeTimes == true && Engine::GetInstance().scene->hasTalkedToTimmyOnce == true) {


					if (hasAllFinished.hasStarted) {

						hasAllFinished.NextDialogue();
						hasAllFinished.Draw(dt);
						if (hasAllFinished.hasEnded) {

							Engine::GetInstance().scene->hasTalkedToTimmyOnce = true;
						}
						return true;
					}
					hasAllFinished.BeginDialogue();
					hasAllFinished.Draw(dt);


					return true;
				}
				if (hasAllFinished.hasStarted && !hasAllFinished.hasEnded) {
					hasAllFinished.Draw(dt);
					return true;

				}









				break;

			case 1:
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 1) {


					if (dialogue.hasStarted) {

						dialogue.NextDialogue();
						dialogue.Draw(dt);
						if (dialogue.hasEnded) {

							Engine::GetInstance().scene->hidingPlaceATM = 2;
						}
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
				break;

			case 2:
				if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && Engine::GetInstance().scene->hidingPlaceATM == 2) {


					if (dialogue.hasStarted) {

						dialogue.NextDialogue();
						dialogue.Draw(dt);
						if (dialogue.hasEnded) {

							Engine::GetInstance().scene->hidingPlaceATM = 0;
							Engine::GetInstance().scene->hasFoundTimmyThreeTimes = true;
						}
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