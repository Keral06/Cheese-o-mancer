#include "Bee.h"
#include "Engine.h"
#include "Scene.h"
#include "Map.h"
#include "Log.h"
#include <cmath>
#include "Textures.h"
#include "Physics.h"
#include "scene.h"
#include "EntityManager.h"
#include "coins.h"

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
    health = 40; // 2 hits

    texName = "assets/Textures/Spritesheets/Bee/Bee_Enemy.png";
    spriteSheetName = "";
    Enemy::Start();
    

    std::unordered_map<int, std::string> aliases = { {0,"idle"},{6,"anticipation attack"},{12,"charge"},{18,"hit wall"},{24,"stunned"},{30,"death"}, };
    anims.LoadFromTSX("assets/Textures/Spritesheets/Bee/b_spritesheet.tsx", aliases);
    anims.SetCurrent("idle");

    patrolCenter = GetPosition();

    detectionRange = 12;
    speed = 4.0f;
    attackHitbox = nullptr;
    // timers
    anticipationTimer = 0.0f;
    stunTimer = 0.0f;
    hitWallTimer = 0.2f;

    beeState = BEE_PATROL;
    lastBeeState = BEE_PATROL;

    anims.SetCurrent("idle");

    return true;
}

// =====================
// UPDATE
// =====================

bool Bee::Update(float dt)
{
    if (hasBeenPicked) return true;

    if (Engine::GetInstance().scene->GetPlayer()->isDead()) return true;

    // 1. ZONA SEGURA DE MUERTE
    if (health <= 0 && !isDead) {
        Die();
    }

    // 2. DIBUJAR CADÁVER
    if (isDead) { // Equivalente a beeState == BEE_DEATH
        Draw(dt);
        return true;
    }

    // 3. LÓGICA NORMAL
    GetPhysicsValues();
    float dist = CalculateDistance();

    switch (beeState)
    {
    case BEE_PATROL:
        Patrol();
        if (dist < detectionRange) {
            attackTimer = attackCooldown;
            SetBeeState(BeeState::BEE_HOVER);
        }
        break;

    case BEE_HOVER:
        HoverPlayer(dt);
        attackTimer -= dt;
        if (attackTimer <= 0.0f) StartCharge();
        break;

    case BEE_ANTICIPATION:
        velocity.x = 0; velocity.y = 0;
        if (anims.HasFinished()) {
            anims.Resets();
            SetBeeState(BeeState::BEE_CHARGE);
        }
        break;

    case BEE_CHARGE:
        velocity.x = chargeDir.getX() * chargeSpeed;
        velocity.y = chargeDir.getY() * chargeSpeed;
        SetBeeState(BeeState::BEE_CHARGE);
        break;

    case BEE_HIT_WALL:
        velocity.x = 0; velocity.y = 0;
        hitWallTimer -= dt;
        SetBeeState(BeeState::BEE_HIT_WALL);
        if (hitWallTimer <= 0.0f) {
            SetBeeState(BeeState::BEE_STUNNED);
            stunTimer = 3000.0f;
        }
        break;

    case BEE_STUNNED:
        velocity.x = 0; velocity.y = 0;
        stunTimer -= dt;
        SetBeeState(BeeState::BEE_STUNNED);
        if (stunTimer <= 0.0f) {
            attackTimer = attackCooldown;
            SetBeeState(BeeState::BEE_HOVER);
        }
        break;

    case BEE_RETURN:
    {
        int ex, ey;
        pbody->GetPosition(ex, ey);
        Vector2D current(ex, ey);
        Vector2D dir = chargeStartPosition - current;
        float len = sqrtf(dir.getX() * dir.getX() + dir.getY() * dir.getY());

        if (len < 5.0f) {
            velocity.x = 0; velocity.y = 0;
            SetBeeState(BeeState::BEE_PATROL);
            break;
        }

        dir = Vector2D(dir.getX() / len, dir.getY() / len);
        velocity.x = dir.getX() * speed;
        velocity.y = dir.getY() * speed;
        break;
    }
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

    SetBeeState(BeeState::BEE_PATROL);
}

// =====================
// START CHARGE (telegraph)
// =====================

void Bee::StartCharge()
{
    SetBeeState(BeeState::BEE_ANTICIPATION);

    anticipationTimer = 0.4f;

    Vector2D player = Engine::GetInstance().scene->GetPlayerPosition();

    int ex, ey;
    pbody->GetPosition(ex, ey);


    chargeStartPosition = Vector2D(ex, ey);
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

void Bee::SetBeeState(BeeState newState)
{
    if (beeState == newState) return;

    lastBeeState = beeState;
    beeState = newState;
    LOG("Bee state: %d", beeState);
    ChangeCurrentAnimation();
}

// =====================
// COLLISIONS
// =====================

void Bee::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (isDead) {
        if (physB->ctype == ColliderType::PLATFORM && pbody != nullptr) {
            Engine::GetInstance().physics->SetLinearVelocity(pbody, { 0.0f, 0.0f });
            b2Body_SetGravityScale(pbody->body, 0.0f);
            //pbody->listener = nullptr;
            hasLanded = true;
        }
        return;
    }

    if (beeState == BEE_CHARGE)
    {
        if (physB->ctype == ColliderType::PLAYER)
        {
            Player* player = dynamic_cast<Player*>(physB->listener);
            if (player)
                Engine::GetInstance().scene->lives--;

            SetBeeState(BeeState::BEE_PATROL);
            return;
        }

        SetBeeState(BeeState::BEE_HIT_WALL);
        hitWallTimer = 0.2f;
    }
}

void Bee::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
}

void Bee::ChangeCurrentAnimation()
{
    if (beeState == lastBeeState) return;

    switch (beeState)
    {
    case BeeState::BEE_PATROL:
        anims.SetCurrent("idle");
        break;

    case BeeState::BEE_CHARGE:
        anims.SetCurrent("charge");
        break;

    case BeeState::BEE_HIT_WALL:
        anims.SetCurrent("hit wall");
        break;

    case BeeState::BEE_HOVER:
        anims.SetCurrent("idle");
        break;

    case BeeState::BEE_ANTICIPATION:
        anims.SetCurrent("anticipation attack");
        break;

    case BeeState::BEE_STUNNED:
        anims.SetCurrent("stunned");
        break;

    case BeeState::BEE_DEATH:
        anims.SetCurrent("death");
        break;
    }
}

void Bee::Draw(float dt)
{
    anims.Update(dt);
    const SDL_Rect& animFrame = anims.GetCurrentFrame();


    int x, y;
    if (pbody != nullptr) {
        pbody->GetPosition(x, y);
        position.setX((float)x);
        position.setY((float)y);
    }
    else {
        x = (int)deathPosition.getX();
        y = (int)deathPosition.getY();
    }



    SDL_Rect sect = { 0,0,texW,texH };

    SDL_FlipMode flip = facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    Engine::GetInstance().render->DrawTexture(texture, x - texW, y - texH, &animFrame, 1.0f, 0.0, INT_MAX, INT_MAX, flip);
}

void Bee::HoverPlayer(float dt)
{
    hoverAngle += hoverSpeed * dt;

    Vector2D player = Engine::GetInstance().scene->GetPlayerPosition();

    float targetX =
        player.getX() + cosf(hoverAngle) * hoverRadius;

    float targetY =
        player.getY() - hoverHeight +
        sinf(hoverAngle) * 20.0f;

    int ex, ey;
    pbody->GetPosition(ex, ey);

    Vector2D current(ex, ey);
    Vector2D target(targetX, targetY);

    Vector2D dir = target - current;

    float distance =
        sqrtf(dir.getX() * dir.getX() +
            dir.getY() * dir.getY());

    // evitar jitter
    if (distance > 10.0f)
    {
        velocity.x = dir.getX() * 0.02f;
        velocity.y = dir.getY() * 0.02f;
    }
    else
    {
        velocity.x *= 0.9f;
        velocity.y *= 0.9f;
    }

    // facing estable
    float diffX = player.getX() - current.getX();

    const float facingMargin = 25.0f;

    if (diffX > facingMargin)
        facingLeft = false;
    else if (diffX < -facingMargin)
        facingLeft = true;
}

void Bee::Die()
{
    isDead = true;
    SetBeeState(BeeState::BEE_DEATH);
    deathPosition = GetPosition();

    if (attackHitbox != nullptr) {
        Engine::GetInstance().physics->DeletePhysBody(attackHitbox);
        attackHitbox = nullptr;
    }

    if (pbody != nullptr) {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = Engine::GetInstance().physics->CreateRectangleSensor(
            (int)deathPosition.getX(),
            (int)deathPosition.getY(),
            texW,
            texH,
            bodyType::DYNAMIC
        );
        pbody->ctype = ColliderType::NPC;
        pbody->listener = this;
    }

    auto newCoin = Engine::GetInstance().entityManager->CreateEntity(EntityType::COIN);
    auto coinEntity = std::static_pointer_cast<Coins>(newCoin);

    if (coinEntity) {
        coinEntity->xInicial = (int)deathPosition.getX();
        coinEntity->yInicial = (int)deathPosition.getY();
        coinEntity->Start();
    }
}