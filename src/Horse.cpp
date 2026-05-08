#include "Horse.h"
#include "Engine.h"
#include "Log.h"
#include "Input.h"
#include "coins.h"
#include "EntityManager.h"
#include "Scene.h"

Horse::Horse() : Enemy()
{
    name = "Horse";
}

Horse::~Horse()
{
}

bool Horse::Start()
{
    texW = 256;
    texH = 256;
    attackRange = 15;
    offsetAttackHitboxX = 60;
    offsetAttackHitboxY = -texH / 2;
    type = EnemyType::MELEE;

    texName = "assets/Textures/Spritesheets/Rata/sprite_rat_02.png";
    spriteSheetName = "";
    Enemy::Start();


    std::unordered_map<int, std::string> aliases = { {0,"idle"},{10,"walk"},{20,"run"},{30,"fall"},{49,"spawn"},{50,"death"}, };
    anims.LoadFromTSX("assets/Textures/Spritesheets/Rata/spritesheet_ratEnemy_02.tsx", aliases);
    anims.SetCurrent("idle");
    attackHitbox = nullptr;

    detectionRange = 20;
    speed = 10; 

    return true;
}

bool Horse::Update(float dt)
{
    if (isDead) {
        Draw(dt);
        return true;
    }

    if (health <= 0 && !coinDropped) {
        coinDropped = true;
        SetState(EnemyState::DYING);
        Die();
        return true;
    }

    repathTimer++;
    if (damageTimer > 0) {
        damageTimer--;
    }

    GetPhysicsValues();

    distanceToPlayer = CalculateDistance();


    if (!isKnockback) {

        Sprint(); 

        if (!isSprinting && !isBraking) {
            if (distanceToPlayer < detectionRange) {
                PerformPathfinding();
                Move();
                SetState(EnemyState::RUNNING);
                if (velocity.y < -0.1f) {
                    SetState(EnemyState::JUMPING);
                }
                else if (velocity.y > 0.1f) {
                    SetState(EnemyState::FALLING);
                }
            }
            else {
                velocity.x = 0;
                SetState(EnemyState::IDLE);
            }
        }
    }
    else {
        knockbackTimer--;
        SetState(EnemyState::FALLING);

        if (knockbackTimer < 0) {
            knockbackTimer = knockbackDuration;
            isKnockback = false;
        }
    }

    ApplyPhysics();

    if (velocity.x < 0)
        facingLeft = true;
    else if (velocity.x > 0)
        facingLeft = false;

    Draw(dt);

    return true;
}

void Horse::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype == ColliderType::PLAYER) {

        Player* player = dynamic_cast<Player*>(physB->listener);

        if (player && damageTimer <= 0) {
            player->lives--;
            damageTimer = damageCooldown; 
        }
    }
}

void Horse::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{

}

void Horse::Die() {
    isDead = true;
    SetState(EnemyState::DYING);

    deathPosition = GetPosition();

    if (pbody != nullptr) {
        pbody->listener = nullptr;
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    if (attackHitbox != nullptr) {
        Engine::GetInstance().physics->DeletePhysBody(attackHitbox);
        attackHitbox = nullptr;
    }

    
    auto newCoin = Engine::GetInstance().entityManager->CreateEntity(EntityType::COIN);
    auto coinEntity = std::static_pointer_cast<Coins>(newCoin);

    if (coinEntity) {
        const Vector2D& pos = this->GetPosition();
        coinEntity->xInicial = (int)pos.getX();
        coinEntity->yInicial = (int)pos.getY();
        coinEntity->Start();
    }
}

void Horse::Sprint()
{
    if (sprintCooldownTimer > 0) sprintCooldownTimer--;

   
    if (!isSprinting && !isBraking && distanceToPlayer <= attackRange && sprintCooldownTimer <= 0) {
        isSprinting = true;
        sprintTimer = sprintDuration;

        int ex, ey;
        pbody->GetPosition(ex, ey);

     
        if (!pathfinding->pathTiles.empty()) {
            Vector2D nextTile = pathfinding->pathTiles.front();
            Vector2D nextPosWorld = Engine::GetInstance().map->MapToWorld((int)nextTile.getX(), (int)nextTile.getY());

            if (nextPosWorld.getX() < ex) sprintDirection = -1; 
            else sprintDirection = 1; 
        }
        else {
            Vector2D pPos = Engine::GetInstance().scene->GetPlayerPosition();
            if (pPos.getX() < ex) sprintDirection = -1;
            else sprintDirection = 1;
        }

        SetState(EnemyState::ATTACKING);
    }

    if (isSprinting) {
        velocity.x = sprintSpeed * sprintDirection;
        sprintTimer--;

        if (sprintTimer <= 0) {
            isSprinting = false;
            isBraking = true;
            brakeTimer = brakeDuration;
        }
    }

    if (isBraking) {
        velocity.x = 0;
        SetState(EnemyState::IDLE);

        brakeTimer--;
        if (brakeTimer <= 0) {
            isBraking = false;
            sprintCooldownTimer = sprintCooldown;
        }
    }
}