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
		Dialogue paperDialogue("Resources/Dialogues/Interactuables/Justice_Dialogues_HiddenScrapOfPaper.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_DiscardedScroll.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_Sketches.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_WallBeforeWheel.txt");
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
	bool WallBeforeWheel::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && py->hasCheese==false) {


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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_LockedDoor.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_DestructDoor.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_NormalFlag.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CheeseFlag.txt");
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
	bool  CheeseFlag::Update(float dt) {
		if (isGettingTouched) {
			InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");



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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_NoteRoyalHalls.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_Compass.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_Portrait.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_Compass.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_HungSword.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt");
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!!
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
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
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->secondDialogue = secondDialogue;

		Dialogue percent("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
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

	Hermit::Hermit() :NPC(EntityType::DEATH) {
		Dialogue paperDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! Dialoguenormal
		this->dialogue = paperDialogue;
		Dialogue secondDialogue("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! lvl1
		this->level1 = secondDialogue;

		Dialogue percent("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! DialogueAfterOnceTalked
		this->notAdvanced = percent;
		Dialogue lvll2("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! lvl2
		this->level2 = lvll2;
		Dialogue third("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! lvl3
		this->level3 = third;

		Dialogue hasAll("resources/Dialogues/Interactuables/Justice_Dialogues_CowWeb.txt"); //change the dialogue lol!!!! all
		this->hasAll = hasAll;
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
	bool Hermit::Update(float dt) {
		if (isGettingTouched) {
			Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

			

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !dialogue.hasEnded && dialogue.hasStarted) { //primer dialogo solo sale una vez


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


			//dialogo si le ha traido el primer objeto
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !level1.hasEnded && level1.hasStarted && py!=nullptr &&py->springWater && !springWater) { //primer dialogo solo sale una vez


				if (level1.hasStarted) {

					level1.NextDialogue();
					level1.Draw(dt);
					if (level1.hasEnded) { springWater = true; }
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

			//Dialogo si tercer objeto
			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !level2.hasEnded && level2.hasStarted && py != nullptr && py->HorsekinManure && !HorsekinManure) { //primer dialogo solo sale una vez


				if (level2.hasStarted) {

					level2.NextDialogue();
					level2.Draw(dt);
					if (level2.hasEnded) { HorsekinManure = true; }
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

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !level3.hasEnded && level3.hasStarted && py != nullptr && py->Gargantuan && !Gargantuan) { //primer dialogo solo sale una vez


				if (level3.hasStarted) {

					level3.NextDialogue();
					level3.Draw(dt);
					if (level3.hasEnded) { Gargantuan = true; }
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

			if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !hasAll.hasEnded && hasAll.hasStarted && py != nullptr &&HorsekinManure && springWater&& Gargantuan && !finishedMission) { //primer dialogo solo sale una vez


				if (hasAll.hasStarted) {

					hasAll.NextDialogue();
					hasAll.Draw(dt);
					if (hasAll.hasEnded) { finishedMission = true; }
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

			//Dialogo si esta a medias 
		}
		

		return true;
	}
	bool Hermit::CleanUp() {
		LOG("Unloading Coin");
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