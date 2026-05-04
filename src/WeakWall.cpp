#include "WeakWall.h"
#include "Engine.h"
#include "Physics.h"
#include "Log.h"

WeakWall::WeakWall() : DestructibleEntity(EntityType::WEAKWALL)
{
    hp = 1;
}

bool WeakWall::Start()
{
    pbody = Engine::GetInstance().physics->CreateRectangle(position.getX() + width/2, position.getY() + height/2, width, height, STATIC);

    pbody->listener = this;
    pbody->ctype = ColliderType::WEAKWALL;

    return true;
}

bool WeakWall::Destroy()
{
    LOG("WeakWall broken!");

    if (owner != nullptr)
    {
        owner->OnWallDestroyed();
    }
    // Aquí puedes meter animación o sonido

    DestructibleEntity::Destroy(); // importante llamar al base
    return true;
}