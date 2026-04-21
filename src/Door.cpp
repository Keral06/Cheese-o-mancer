#include "Door.h"
#include "Engine.h"
#include "Physics.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Log.h"

Door::Door() : Entity(EntityType::UNKNOWN)
{
    name = "door";
}

Door::~Door()
{
}

bool Door::Start()
{
    // Crear sensor físico (no colisiona, solo detecta)
    pbody = Engine::GetInstance().physics->CreateRectangleSensor(
        position.getX(),
        position.getY(),
        64, 64, // tamaño puerta (ajústalo)
        bodyType::STATIC
    );

    pbody->listener = this;
    pbody->ctype = ColliderType::PARED;

    return true;
}

bool Door::Update(float dt)
{
    return true;
}

bool Door::CleanUp()
{
    if (pbody != nullptr)
    {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    return true;
}

void Door::SetDoorData(
    const std::string& targetMap,
    const std::string& targetDoor,
    int offsetX,
    int offsetY
)
{
    this->targetMap = targetMap;
    this->targetDoor = targetDoor;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
}

void Door::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype != ColliderType::PLAYER)
        return;

    LOG("Door triggered -> loading map: %s", targetMap.c_str());

    // Guardamos a qué puerta queremos aparecer
   Engine::GetInstance().scene->nextSpawnPoint = targetDoor;

    // Cambiar de mapa
    Engine::GetInstance().scene->LoadMap(targetMap);
}