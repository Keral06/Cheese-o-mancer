#include "Checkpoint.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Scene.h"
#include "Physics.h"
#include "Player.h"
#include "Map.h"

Checkpoint::Checkpoint() : Entity(EntityType::CHECKPOINT)
{
	name = "Checkpoint";
}

Checkpoint::~Checkpoint() {}

bool Checkpoint::Start() {

	// 1. Cargar el Spritesheet
	texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Bandera checkpoint/Bandera checkpoint.png");

	// 2. Cargar las animaciones desde el TSX 
	std::unordered_map<int, std::string> aliases = {
		{1, "static1"},    // Bandera inicial
		{0, "animation"},  // Animacion
		{11, "static2"}    // Bandera pintada
	};

	anims.LoadFromTSX("assets/Textures/Spritesheets/Bandera checkpoint/checkpoint.tsx", aliases);
	anims.SetCurrent("static1");
	currentAnimName = "static1";

	// 3. Audio y dimensiones
	fxId = Engine::GetInstance().audio->LoadFx("assets/Audio/Fx/PREV/checkpoint.wav");

	texW = 128; 
	texH = 128; 

	// 4. Crear el collider
	if (pbody == nullptr) {
		position.setX(xInicial);
		position.setY(yInicial);
		pbody = Engine::GetInstance().physics->CreateRectangleSensor(
			(int)position.getX() + texW / 2,
			(int)position.getY() + texH / 2 - 16,
			texW / 2,
			texH / 2,
			bodyType::DYNAMIC
		);
		b2Body_SetGravityScale(pbody->body, 0.0f);

		pbody->ctype = ColliderType::SAVE;
		pbody->listener = this;
	}

	return true;
}

bool Checkpoint::Update(float dt)
{
	// 1. Transición de la animación de activación a la estática final
	if (currentAnimName == "animation" && anims.HasFinished()) {
		anims.SetCurrent("static2");
		currentAnimName = "static2";
	}

	// 2. Actualizar fotogramas
	anims.Update(dt);

	// 3. Dibujar
	if (pbody != nullptr && texture != nullptr) {
		int x = 0;
		int y = 0;
		pbody->GetPosition(x, y);

		const SDL_Rect& animFrame = anims.GetCurrentFrame();

		Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2 - 145, &animFrame);
	}

	return true;
}

void Checkpoint::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER && !isActivated) {
		isActivated = true;

		// 1. Iniciar la animación
		anims.SetCurrent("animation");
		currentAnimName = "animation";

		// 2. Reproducir el sonido
		Engine::GetInstance().audio->PlayFx(fxId);

		// 3. Guardar partida
		Player* rawPlayer = Engine::GetInstance().scene->GetPlayer();
		if (rawPlayer != nullptr) {
			std::shared_ptr<Player> playerPtr(rawPlayer, [](Player*) {});
			Engine::GetInstance().map->SaveEntities(playerPtr);
		}
	}
}

bool Checkpoint::CleanUp()
{
	Engine::GetInstance().textures->UnLoad(texture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}