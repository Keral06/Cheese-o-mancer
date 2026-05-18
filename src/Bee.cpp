#include "Bee.h"
#include "Engine.h"
#include "Scene.h"
#include "Map.h"
#include "Log.h"
#include <cmath>
#include "Textures.h"
#include "Physics.h"
#include "scene.h"

// =====================
// CONSTRUCTOR
// =====================

Bee::Bee() : Enemy()
{
    name = "Bee";
}

Bee::~Bee() {}

// =====================
// START
// =====================

bool Bee::Start()
{
    texW = 128;
    texH = 128;

    type = EnemyType::AIR;

    texName = "assets/Textures/Spritesheets/Bee/Bee_Enemy.png";
    spriteSheetName = "";
    Enemy::Start();
    

    std::unordered_map<int, std::string> aliases = { {0,"idle"},{6,"anticipation attack"},{12,"charge"},{18,"hit wall"},{24,"stunned"},{30,"death"}, };
    anims.LoadFromTSX("assets/Textures/Spritesheets/Bee/b_spritesheet.tsx", aliases);
    anims.SetCurrent("idle");

    pbody = Engine::GetInstance().physics->CreateRectangleFriction(
        position.getX(),
        position.getY(),
        texW,
        texH,
        bodyType::DYNAMIC,
        0.0f
    );

    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;

    patrolCenter = GetPosition();

    detectionRange = 12;
    speed = 4.0f;

    // timers
    anticipationTimer = 0.0f;
    stunTimer = 0.0f;
    hitWallTimer = 0.2f;

    beeState = BEE_PATROL;

    anims.SetCurrent("idle");

    return true;
}

// =====================
// UPDATE
// =====================

bool Bee::Update(float dt)
{
    if (health <= 0 && beeState != BEE_DEATH)
    {
        beeState = BEE_DEATH;
        SetState(EnemyState::DYING);
        Die();
        return true;
    }

    GetPhysicsValues();

    float dist = CalculateDistance();

    switch (beeState)
    {
        // =====================
    case BEE_PATROL:
    {
        Patrol();

        if (dist < detectionRange)
            StartCharge();

        break;
    }

    // =====================
    case BEE_ANTICIPATION:
    {
        velocity.x = 0;
        velocity.y = 0;

        anticipationTimer -= dt;

        SetState(EnemyState::IDLE);

        if (anticipationTimer <= 0.0f)
            beeState = BEE_CHARGE;

        break;
    }

    // =====================
    case BEE_CHARGE:
    {
        velocity.x = chargeDir.getX() * chargeSpeed;
        velocity.y = chargeDir.getY() * chargeSpeed;

        SetState(EnemyState::ATTACKING);

        break;
    }

    // =====================
    case BEE_HIT_WALL:
    {
        velocity.x = 0;
        velocity.y = 0;

        hitWallTimer -= dt;

        SetState(EnemyState::HIT);

        if (hitWallTimer <= 0.0f)
        {
            beeState = BEE_STUNNED;
            stunTimer = 2.0f;
        }

        break;
    }

    // =====================
    case BEE_STUNNED:
    {
        velocity.x = 0;
        velocity.y = 0;

        stunTimer -= dt;

        SetState(EnemyState::HIT);

        if (stunTimer <= 0.0f)
            beeState = BEE_PATROL;

        break;
    }

    // =====================
    case BEE_DEATH:
        return true;
    }

    ApplyPhysics();
    Draw(dt);

    return true;
}

// =====================
// PATROL (vuelo circular)
// =====================

void Bee::Patrol()
{
    patrolAngle += patrolSpeed * 0.01f;

    Vector2D center = patrolCenter;

    float x = center.getX() + cosf(patrolAngle) * patrolRadius;
    float y = center.getY() + sinf(patrolAngle) * patrolRadius;

    int ex, ey;
    pbody->GetPosition(ex, ey);

    Vector2D target(x, y);

    float dx = target.getX() - ex;
    float dy = target.getY() - ey;

    velocity.x = dx * 0.05f;
    velocity.y = dy * 0.05f;

    facingLeft = velocity.x < 0;

    SetState(EnemyState::IDLE);
}

// =====================
// START CHARGE (telegraph)
// =====================

void Bee::StartCharge()
{
    beeState = BEE_ANTICIPATION;

    anticipationTimer = 0.4f;

    Vector2D player = Engine::GetInstance().scene->GetPlayerPosition();

    int ex, ey;
    pbody->GetPosition(ex, ey);

    chargeDir = player - Vector2D(ex, ey);

    float len = sqrtf(
        chargeDir.getX() * chargeDir.getX() +
        chargeDir.getY() * chargeDir.getY()
    );

    if (len != 0)
    {
        chargeDir = Vector2D(
            chargeDir.getX() / len,
            chargeDir.getY() / len
        );
    }

    velocity.x = 0;
    velocity.y = 0;
}

// =====================
// COLLISIONS
// =====================

void Bee::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (beeState == BEE_CHARGE)
    {
        if (physB->ctype == ColliderType::PLAYER)
        {
            Player* player = dynamic_cast<Player*>(physB->listener);

            if (player)
                player->lives--;

            beeState = BEE_PATROL;
            return;
        }

        // cualquier otra cosa = pared
        beeState = BEE_HIT_WALL;
        hitWallTimer = 0.2f;
    }
}

void Bee::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
}

void Bee::ChangeCurrentAnimation()
{
    if (state == lastState) return;

    switch (state)
    {
    case EnemyState::IDLE:
        anims.SetCurrent("idle");
        break;

    case EnemyState::WALKING:
        anims.SetCurrent("idle"); // o hover si tienes
        break;

    case EnemyState::RUNNING:
        anims.SetCurrent("charge");
        break;

    case EnemyState::ATTACKING:
        anims.SetCurrent("charge");
        break;

    case EnemyState::HIT:
        anims.SetCurrent("hit_wall");
        break;

    case EnemyState::JUMPING:
        anims.SetCurrent("anticipation_attack");
        break;

    case EnemyState::FALLING:
        anims.SetCurrent("stunned");
        break;

    case EnemyState::DYING:
        anims.SetCurrent("death");
        break;
    }
}