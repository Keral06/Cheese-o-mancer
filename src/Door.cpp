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
    pbody->ctype = ColliderType::DOOR;

    return true;
}

bool Door::Update(float dt)
{
    if (requiresInteraction &&
        playerInside &&
        Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
    {
        if (isLocked) {
            if (Engine::GetInstance().scene->inventario.tieneObjeto(requiredKey)) {
                isLocked = false;
                Engine::GetInstance().scene->inventario.eliminarObjeto(requiredKey);
                LOG("Puerta desbloqueada con %s!", requiredKey.c_str());
            }
            else {
                LOG("Puerta bloqueada. Necesitas la llave: %s", requiredKey.c_str());
                return true;
            }
        }
        Engine::GetInstance().scene->nextSpawnPoint = targetDoor;
        Engine::GetInstance().scene->firstDoor = false;
        Engine::GetInstance().scene->nextMap = targetMap;
        Engine::GetInstance().scene->StartFadeOut(0.5f);
    }

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
    int height,
    bool requiresInteraction,
    bool isLocked,
    std::string requiredKey
)
{
    this->targetMap = targetMap;
    this->targetDoor = targetDoor;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->width = width;
    this->height = height;
    this->requiresInteraction = requiresInteraction;
    this->isLocked = isLocked;
    this->requiredKey = requiredKey;
}

void Door::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype != ColliderType::PLAYER)
        return;
    if (Engine::GetInstance().scene->isInBossfight) {
        return;
    }
    if (requiresInteraction){
        playerInside = true;
        return;
    }
    if (isLocked) {
        if (Engine::GetInstance().scene->inventario.tieneObjeto("Key")) {
            isLocked = false;
            Engine::GetInstance().scene->inventario.eliminarObjeto("Key");
            LOG("Puerta desbloqueada con la llave!");
        }
        else {
            LOG("Puerta bloqueada. Necesitas comprar la llave.");
            return; 
        }
    }

    LOG("Door triggered -> loading map: %s", targetMap.c_str());

    // Guardamos a qué puerta queremos aparecer
   Engine::GetInstance().scene->nextSpawnPoint = targetDoor;

   Engine::GetInstance().scene->firstDoor = false;

   Engine::GetInstance().scene->nextMap = targetMap;
    // Cambiar de mapa
   Engine::GetInstance().scene->StartFadeOut(0.5f);
}

void Door::OnCollisionEnd(PhysBody* physA, PhysBody* physB){
    if (physB->ctype != ColliderType::PLAYER)
        return;

    playerInside = false;
}