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
class HiddenScrapOfPaper : public NPC
{
public:
	HiddenScrapOfPaper() :NPC(EntityType::HIDDENSCRAPOFPAPER) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_HiddenScrapOfPaper.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~HiddenScrapOfPaper()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class DiscardedScroll : public NPC
{
public:
	DiscardedScroll() :NPC(EntityType::DISCARDEDSCROLL) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_DiscardedScroll.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~DiscardedScroll()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class Sketches : public NPC
{
public:
	Sketches() :NPC(EntityType::SKETCHES) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Sketches.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~Sketches()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class WallBeforeWheel : public NPC
{
public:
	WallBeforeWheel() :NPC(EntityType::WALLBEFOREWHEEL) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_WallBeforeWheel.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~WallBeforeWheel()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class LockedDoor : public NPC
{
public:
	LockedDoor() :NPC(EntityType::LOCKEDDOOR) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_LockedDoor.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~LockedDoor()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class DestructDoor : public NPC
{
public:
	DestructDoor() :NPC(EntityType::DESTRUCTDOOR) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_DestructDoor.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~DestructDoor()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class NormalFlag : public NPC
{
public:
	NormalFlag() :NPC(EntityType::NORMALFLAG) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_NormalFlag.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~NormalFlag()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class CheeseFlag : public NPC
{
public:
	CheeseFlag() :NPC(EntityType::CHEESEFLAG) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_CheeseFlag.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~CheeseFlag()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class NoteRoyalHalls : public NPC
{
public:
	NoteRoyalHalls() :NPC(EntityType::NOTEROYALHALLS) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_NoteRoyalHalls.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~NoteRoyalHalls()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class Compass : public NPC
{
public:
	Compass() :NPC(EntityType::COMPASS) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Compass.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~Compass()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class Portrait : public NPC
{
public:
	Portrait() :NPC(EntityType::PORTRAIT) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Portrait.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~Portrait()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};
class UnfinishedPortrait : public NPC
{
public:
	UnfinishedPortrait() :NPC(EntityType::UNFINISHEDPORTRAIT) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_Compass.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~UnfinishedPortrait()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
}; 
class HungSword : public NPC
{
public:
	HungSword() :NPC(EntityType::HUNGSWORD) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_HungSword.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~HungSword()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
}; 
class CowWeb : public NPC
{
public:
	CowWeb() :NPC(EntityType::COWWEB) {
		Dialogue paperDialogue("Assets/Dialogue/Interactuables/Justice_Dialogues_CowWeb.tsx");
		this->dialogue = paperDialogue;

	}

	virtual ~CowWeb()
	{
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
	}
	bool Awake() {
		return true;
	}
	bool Start() {

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
	bool Update(float dt) {
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
	bool CleanUp();
	void OnCollision(PhysBody* physA, PhysBody* physB) {

		Player* pp = static_cast<Player*>(physB->listener);
		py = pp;
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			isGettingTouched = true;
			break;
		}


	}
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
		isGettingTouched = false;



	}

public:


private:








private:
	SDL_Texture* texture;
	AnimationSet anims;
	PhysBody* pbody;
	int texW, texH;
	const char* tsxPath;

public:
	int coinFx;
	int coinPickupFx;
	int xInicial;
	int yInicial;

	bool isGettingTouched = false;
	Player* py;

	SDL_Texture* InteractTexture = nullptr;
};

