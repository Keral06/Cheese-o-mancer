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
   
    pbody = Engine::GetInstance().physics->CreateRectangleSensor(
        position.getX() + width/2,
        position.getY() + height/2,
        width, height,
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
    int offsetY,
    int width,
    int height
)
{
    this->targetMap = targetMap;
    this->targetDoor = targetDoor;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->width = width;
    this->height = height;
}

void Door::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype != ColliderType::PLAYER)
        return;

    LOG("Door triggered -> loading map: %s", targetMap.c_str());

    // Guardamos a qué puerta queremos aparecer
   Engine::GetInstance().scene->nextSpawnPoint = targetDoor;

   Engine::GetInstance().scene->firstDoor = false;

    // Cambiar de mapa
    Engine::GetInstance().scene->LoadMap(targetMap);
}