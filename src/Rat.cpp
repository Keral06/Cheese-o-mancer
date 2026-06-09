#include "Rat.h"
#include "Engine.h"
#include "Log.h"
#include "Input.h"
#include "coins.h"
#include "EntityManager.h"
#include "Scene.h"
#include "HighPriestess.h"

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
	health = 60; // 3 hits
    
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
    if (hasBeenPicked) return true;

    if (Engine::GetInstance().scene->GetPlayer()->isDead()) return true;

    // Lógica de Arena
    if (!hasEntered) {
        if (pbody != nullptr && b2Body_IsEnabled(pbody->body)) {
            b2Body_Disable(pbody->body);
        }
        return true;
    }

    if (pbody != nullptr && !b2Body_IsEnabled(pbody->body)) {
        b2Body_Enable(pbody->body);
    }

    // 1. ZONA SEGURA DE MUERTE
    if (health <= 0 && !isDead) {
        Die();
    }

    // 2. DIBUJAR CADÁVER
    if (isDead) {
        Draw(dt);
        return true;
    }

    // 3. LÓGICA NORMAL
    if (damageTimer > 0) damageTimer--;

    GetPhysicsValues();
    distanceToPlayer = CalculateDistance();

    if (isKnockback) {
        knockbackTimer--;
        if (knockbackTimer <= 0) {
            isKnockback = false;
            knockbackTimer = knockbackDuration;
            Engine::GetInstance().physics->SetLinearVelocity(pbody, 0.0f, 0.0f);
            velocity = { 0.0f, 0.0f };
        }
    }
    else {
        if (distanceToPlayer < detectionRange) {
            PerformPathfinding();
            Move();
            SetState(EnemyState::RUNNING);
        }
        else {
            Patrol();
            SetState(EnemyState::WALKING);
        }
    }

    ApplyPhysics();

    if (velocity.x < -0.1f) facingLeft = true;
    else if (velocity.x > 0.1f) facingLeft = false;

    Draw(dt);
    return true;
}

void Rat::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (isDead) {
        if (physB->ctype == ColliderType::PLATFORM && pbody != nullptr) {
            Engine::GetInstance().physics->SetLinearVelocity(pbody, { 0.0f, 0.0f });

            b2Body_SetGravityScale(pbody->body, 0.0f);

            //pbody->listener = nullptr;
        }
        return;
    }

    if (physB->ctype == ColliderType::DANGER) {
        this->DecreaseHealth(100);
    }

    if (physB->ctype == ColliderType::PLAYER) {
        Player* player = dynamic_cast<Player*>(physB->listener);
        if (player && !player->godMode) {
            Engine::GetInstance().scene->lives--;
        }
        if (player && damageTimer <= 0) {
            damageTimer = 60; 
            LOG("Daño recibido!");
        }
    }
}

void Rat::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
    
}

//Muerte
void Rat::Die()
{
    isDead = true;
    isDefeated = true;
    SetState(EnemyState::DYING);
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
            texW/2,
            texH/2,
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
    if (HighPriestesss::instance != nullptr) {
        HighPriestesss::instance->NotifyEnemyDeath();
    }
    if (isArenaRat) {
        Engine::GetInstance().scene->CheckMiniBossStatus();
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

void Rat::Draw(float dt)
{
    int x = (int)position.getX();
    int y = (int)position.getY();

    if (pbody != nullptr) {
        pbody->GetPosition(x, y);
        position.setX((float)x);
        position.setY((float)y);
    }

    anims.Update(dt);
    SDL_Rect animFrame = anims.GetCurrentFrame();


    SDL_FlipMode flip = facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    int drawX = x - texW / 2;
    int drawY = y - texH / 2;


    if (isDead) {
        drawY -= 64;
    }

    Engine::GetInstance().render->DrawTexture(
        texture,
        drawX,
        drawY,
        &animFrame,
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );
}