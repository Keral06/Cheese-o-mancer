#include "WeakWall.h"
#include "Engine.h"
#include "Physics.h"
#include "Log.h"
#include "Textures.h"

WeakWall::WeakWall() : DestructibleEntity(EntityType::WEAKWALL)
{
    hp = 1;
}

bool WeakWall::Start()
{
    pbody = Engine::GetInstance().physics->CreateRectangle(position.getX() + width/2, position.getY() + height/2, width, height, STATIC);

    texture = Engine::GetInstance().textures->Load("assets/Textures/Columna.png");

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

bool WeakWall::Update(float dt)
{
    if (texture != nullptr)
    {
        Engine::GetInstance().render->DrawTexture(
            texture,
            (int)position.getX(),
            (int)position.getY()
        );
    }

    return true;
}

bool WeakWall::CleanUp()
{
    if (texture != nullptr)
    {
        Engine::GetInstance().textures->UnLoad(texture);
        texture = nullptr;
    }

    return DestructibleEntity::CleanUp();
}