#pragma once

#include "Enemy.h"
#include "Pathfinding.h"

class Bee : public Enemy
{
public:
    Bee();
    ~Bee();

    bool Start() override;
    bool Update(float dt) override;
    void ChangeCurrentAnimation() override;
    void Draw(float dt) override;
    void HoverPlayer(float dt);

    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;

    void Die() override;

private:

    // =====================
    // ESTADOS PROPIOS
    // =====================

    enum BeeState
    {
        BEE_PATROL,
        BEE_ANTICIPATION,
        BEE_CHARGE,
        BEE_HIT_WALL,
        BEE_STUNNED,
        BEE_RETURN,
        BEE_HOVER,
        BEE_DEATH
    };

    BeeState beeState = BEE_PATROL;
    BeeState lastBeeState = BEE_PATROL;
    // =====================
    // MOVIMIENTO PATROL
    // =====================

    Vector2D patrolCenter;
    float patrolRadius = 120.0f;
    float patrolAngle = 0.0f;
    float patrolSpeed = 1.5f;

    // =====================
    // CHARGE ATTACK
    // =====================

    Vector2D chargeDir;
    float chargeSpeed = 18.0f;

    float anticipationTimer = 0.0f;

    // =====================
    // STUN / HIT WALL
    // =====================

    float stunTimer = 0.0f;
    float hitWallTimer = 100.0f;

    // =====================
    // DETECCIÓN
    // =====================

    float detectionRange = 100.0f; // world units (ajustable)

    // =====================
    // MÉTODOS INTERNOS
    // =====================

    void Patrol();
    void StartCharge();

    void SetBeeState(BeeState newState);

    Vector2D chargeStartPosition;
    bool returningToPatrol = false;

    float attackCooldown = 3000.0f;
    float attackTimer = 0.0f;

    float hoverRadius = 200.0f;
    float hoverHeight = 680.0f;
    float hoverAngle = 0.0f;
    float hoverSpeed = 1.2f;
};