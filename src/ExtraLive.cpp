#include "ExtraLive.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Player.h"

ExtraLive::ExtraLive() : Entity(EntityType::EXTRALIVE)
{
	name = "ExtraLive";
	pbody = nullptr;
}

ExtraLive::~ExtraLive() {}

bool ExtraLive::Awake() {
	return true;
}

bool ExtraLive::Start() {

	//Initialize Enemy parameters
	texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Healing Plant/Healing_Plant_spritesheet.png");

	// IDs de inicio de cada animación
	std::unordered_map<int, std::string> aliases = {
		{1, "healing_plant"},          // Planta intacta
		{0, "heal1"},        // Animación al comer la 1ra
		{7, "heal1_static"}, // Estado estático con 1 menos
		{8, "heal2"},        // Animación al comer la 2da
		{15, "heal2_static"}, // Estado estático con 2 menos
		{16, "heal3"},        // Animación al comer la última
		{23, "heal3_static"}  // Planta vacía
	};

	anims.LoadFromTSX("assets/Textures/Spritesheets/Healing Plant/healing_plant.tsx", aliases);
	anims.SetCurrent("healing_plant");
	currentAnimName = "healing_plant";


	texW = 256;
	texH = 384;

	// collider
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

		pbody->ctype = ColliderType::EXTRALIVE;
		pbody->listener = this;
	}

	return true;
}

bool ExtraLive::Update(float dt)
{
	if (!active) return true;

	// 1. Lógica para pasar de animación de consumo a estado estático
	if (currentAnimName == "heal1" && anims.HasFinished()) {
		anims.SetCurrent("heal1_static");
		currentAnimName = "heal1_static";
	}
	else if (currentAnimName == "heal2" && anims.HasFinished()) {
		anims.SetCurrent("heal2_static");
		currentAnimName = "heal2_static";
	}
	else if (currentAnimName == "heal3" && anims.HasFinished()) {
		anims.SetCurrent("heal3_static");
		currentAnimName = "heal3_static";
	}

	// 2. Actualizar y dibujar la animación
	anims.Update(dt);

	if (pbody != nullptr && texture != nullptr) {
		int x = 0;
		int y = 0;
		pbody->GetPosition(x, y);

		const SDL_Rect& animFrame = anims.GetCurrentFrame();
		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame);
	}

	return true;
}

bool ExtraLive::CleanUp()
{
	LOG("Unloading Healing Plant");
	Engine::GetInstance().textures->UnLoad(texture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}

bool ExtraLive::Destroy()
{
	LOG("Destroying item");
	active = false;
	Engine::GetInstance().entityManager->DestroyEntity(shared_from_this());
	return true;
}


void ExtraLive::OnCollision(PhysBody* physA, PhysBody* physB) {
	if (physB->ctype == ColliderType::PLAYER) {
		LOG("Heal plant interact");

		// 1. Evitar que se gaste la fruta si ya tiene la vida al máximo
		if (Engine::GetInstance().scene->lives >= Engine::GetInstance().scene->maxLives) {
			return;
		}

		bool fruitEaten = false;

		// 2. Evaluamos la fase de la planta

		if (healStage == 0 && currentAnimName == "healing_plant") {
			healStage = 1;
			anims.SetCurrent("heal1");
			currentAnimName = "heal1";
			fruitEaten = true;
		}
		else if (healStage == 1 && currentAnimName == "heal1_static") {
			healStage = 2;
			anims.SetCurrent("heal2");
			currentAnimName = "heal2";
			fruitEaten = true;
		}
		else if (healStage == 2 && currentAnimName == "heal2_static") {
			healStage = 3;
			anims.SetCurrent("heal3");
			currentAnimName = "heal3";
			fruitEaten = true;
		}

		// 3. Si se consumió una fruta, curamos al jugador
		if (fruitEaten) {
			Engine::GetInstance().scene->lives++;

			// Hacemos sonar el audio de curación obteniendo el ID desde el Player
			Player* p = static_cast<Player*>(physB->listener);
			if (p != nullptr) {
				Engine::GetInstance().audio->PlayFx(p->healfx);
			}

			LOG("Fruta consumida. Vidas actuales: %d", Engine::GetInstance().scene->lives);
		}
	}
}