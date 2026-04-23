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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_HiddenScrapOfPaper.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_DiscardedScroll.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Sketches.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_WallBeforeWheel.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_LockedDoor.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_DestructDoor.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_NormalFlag.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_CheeseFlag.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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



		return true;

	}
	bool  CheeseFlag::Update(float dt) {
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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_NoteRoyalHalls.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Compass.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Portrait.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Compass.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_HungSword.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_CowWeb.tsx");
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

		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);

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

