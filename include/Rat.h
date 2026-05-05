#pragma once
#include "Enemy.h"
#include "Physics.h"
class Rat : public Enemy
{
public:
    Rat();
    ~Rat();

    bool Start() override;
    bool Update(float dt) override;
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;
    void Die();
    void Patrol();
protected:

    bool coinDropped = false;

    Vector2D patrolStart;
    Vector2D patrolEnd;
    bool goingToEnd = true;
    bool isDead = false;

    int damageCooldown = 60;
    int damageTimer = 0;
};