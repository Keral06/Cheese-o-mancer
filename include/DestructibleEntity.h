#pragma once

#include "Entity.h"

class PhysBody;

class DestructibleEntity : public Entity
{
public:
    DestructibleEntity(EntityType type);
    virtual ~DestructibleEntity();

    // Ciclo de vida
    virtual bool Start() override;
    virtual bool Update(float dt) override;
    virtual bool CleanUp() override;

    // Sistema de daño
    virtual void TakeDamage(int amount);
    virtual bool Destroy();

    // Opcional: para personalizar comportamiento
    virtual void OnHit();

protected:
    int hp = 1;
    bool destroyed = false;

    PhysBody* pbody = nullptr;
};