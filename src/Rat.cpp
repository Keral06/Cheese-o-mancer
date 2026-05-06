#include "Rat.h"
#include "Engine.h"
#include "Log.h"
#include "Input.h"
#include "coins.h"
#include "EntityManager.h"
#include "Scene.h"

Rat::Rat() : Enemy()
{
    name = "Rat";
}

Rat::~Rat()
{
}

bool Rat::Start()
{
    texW = 256;
    texH = 256;
	attackRange = 5;
    offsetAttackHitboxX = 40;
    offsetAttackHitboxY = -texH/2;
    type = EnemyType::MELEE;
    
    texName = "assets/Textures/Spritesheets/Rata/sprite_rat_02.png";
    spriteSheetName = "";
    Enemy::Start();
    
    
    std::unordered_map<int, std::string> aliases = { {0,"idle"},{10,"walk"},{20,"run"},{30,"fall"},{49,"spawn"},{50,"death"}, };
    anims.LoadFromTSX("assets/Textures/Spritesheets/Rata/spritesheet_ratEnemy_02.tsx", aliases);

    anims.SetCurrent("idle");   
    attackHitbox = nullptr;
    patrolStart = GetPosition();
    patrolEnd = Vector2D(patrolStart.getX() + 400, patrolStart.getY());

    detectionRange = 10;
    speed = 10;
    return true;
}

bool Rat::Update(float dt)
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

    // =====================
    // MOVERSE
    // =====================

    if (distanceToPlayer < detectionRange) {
        if (!isKnockback) {
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
            knockbackTimer--;

            
            SetState(EnemyState::FALLING);
            

            if (knockbackTimer < 0) {
                knockbackTimer = knockbackDuration;
                isKnockback = false;
            }

        }
    }
    else {
       Patrol();
    }

    ApplyPhysics();

    if (velocity.x < 0)
        facingLeft = true;
    else if (velocity.x > 0)
        facingLeft = false;

    Draw(dt);

    return true;
}

void Rat::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype == ColliderType::PLAYER) {

        // daño directo
        Player* player = dynamic_cast<Player*>(physB->listener);

        if (player && damageTimer <= 0) {
            player->lives--;
        }
    }
}

void Rat::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
    
}

//Muerte
void Rat::Die() {
    isDead = true;
    SetState(EnemyState::DYING);

    deathPosition = GetPosition();

    // eliminar físicas
    if (pbody != nullptr) {
        pbody->listener = nullptr;
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    if (attackHitbox != nullptr) {
        Engine::GetInstance().physics->DeletePhysBody(attackHitbox);
        attackHitbox = nullptr;
    }

    // spawn coin
    auto newCoin = Engine::GetInstance().entityManager->CreateEntity(EntityType::COIN);
    auto coinEntity = std::static_pointer_cast<Coins>(newCoin);

    if (coinEntity) {
        const Vector2D& pos = this->GetPosition();
        coinEntity->xInicial = (int)pos.getX();
        coinEntity->yInicial = (int)pos.getY();
        coinEntity->Start();
    }
}

void Rat::Patrol()
{
    float threshold = 5.0f;

    Vector2D target = goingToEnd ? patrolEnd : patrolStart;

    int ex, ey;
    pbody->GetPosition(ex, ey);

    if (abs(ex - target.getX()) < threshold) {
        goingToEnd = !goingToEnd;
    }

    if (ex < target.getX())
        velocity.x = speed/2;
    else
        velocity.x = -speed/2;

    SetState(EnemyState::WALKING);
}
