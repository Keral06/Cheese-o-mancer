#include "DestructibleEntity.h"
#include "Engine.h"
#include "Physics.h"
#include "Log.h"

DestructibleEntity::DestructibleEntity(EntityType type)
    : Entity(type)
{
}

DestructibleEntity::~DestructibleEntity()
{
}

bool DestructibleEntity::Start()
{
    return true;
}

bool DestructibleEntity::Update(float dt)
{
    if (toDelete) return true;

    // Aquí podrías sincronizar posición si quieres
    return true;
}

bool DestructibleEntity::CleanUp()
{
    LOG("Cleaning DestructibleEntity");

    if (pbody != nullptr)
    {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    return true;
}

void DestructibleEntity::TakeDamage(int amount)
{
    if (destroyed) return;

    hp -= amount;

    LOG("DestructibleEntity took %d damage. HP left: %d", amount, hp);

    OnHit();

    if (hp <= 0)
    {
        Destroy();
    }
}

void DestructibleEntity::OnHit()
{
    // Por defecto no hace nada
    // Puedes overridear en hijos (sonido, partículas, etc.)
}

bool DestructibleEntity::Destroy()
{
    if (destroyed) return true;

    destroyed = true;

    LOG("DestructibleEntity destroyed");

    // Aquí puedes meter FX genéricos si quieres

    toDelete = true;

    return true;
}