#pragma once
#include "Enemy.h"
#include "Physics.h"
class Jailer : public Enemy
{
public:
    Jailer();
    ~Jailer();

    bool Start() override;
    void Attack() override;
    bool Update(float dt) override;
    void UpdateIdle(float dt);
    void UpdateChase(float dt);
    void UpdateAttack(float dt);
    void UpdateReturn(float dt);

    void Draw(float dt) override;
    void ChangeCurrentAnimation() override;
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;
protected:

    enum JailerState
    {
        JAILER_IDLE,
        JAILER_CHASE,
        JAILER_ATTACK,
        JAILER_RETURN,
        JAILER_DEATH
    };


    void SetJailerState(JailerState newState);
    

    float hitboxStart = 5.0f;
    float hitboxEnd = 15.0f;
    bool hasHit = false;
    bool hitboxActive = false;

    bool isAttacking = false;

    bool playerInHitbox = true;
    char* texName2 = "";
    AnimationSet anims2;
    SDL_Texture* texture2 = nullptr;

    JailerState jailerState;
    JailerState lastJailerState;

    Vector2D spawnPosition;

    float attackCooldown = 2.0f;
    float attackTimer = 0.0f;

    float attackDuration = 0.6f;
    float attackDurationTimer = 0.0f;

    bool attackLocked = false;
};