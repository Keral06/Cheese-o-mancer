#pragma once
#include "Enemy.h"
#include "Physics.h"

class Horse : public Enemy
{
public:
    Horse();
    ~Horse();

    bool Start() override;
    bool Update(float dt) override;
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;
    void ChangeCurrentAnimation() override;
    void Draw(float dt) override;
    void Die();
    void Sprint();

protected:

    bool coinDropped = false;

    bool isSprinting = false;
    bool isBraking = false;

    float sprintSpeed = 25.0f;
    int sprintDuration = 30;
    int sprintTimer = 0;

    int brakeDuration = 40;
    int brakeTimer = 0;

    int sprintCooldown = 120;
    int sprintCooldownTimer = 0;
    int sprintDirection = 1;

    int damageCooldown = 60;
    int damageTimer = 0;
};