#include "Horse.h"
#include "Engine.h"
#include "Log.h"
#include "Input.h"
#include "coins.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Textures.h"

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
    texH = 256 * 2;
    attackRange = 15;
    offsetAttackHitboxX = 60;
    offsetAttackHitboxY = -texH / 2;
    knockbackDuration = 20;
	health = 80; // 4 hits
    type = EnemyType::MELEE;

    texName = "assets/Textures/Spritesheets/Unicorn/sprite_unicorn_b_02.png";
    spriteSheetName = "";

    texture = Engine::GetInstance().textures->Load(texName);

    
    pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.0f);

    
    pbody->listener = this;

   
    pbody->ctype = ColliderType::ENEMY;

  
    pathfinding = std::make_shared<Pathfinding>();
    
    Vector2D pos = GetPosition();
    
    Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());
    
    pathfinding->ResetPath(tilePos);


    std::unordered_map<int, std::string> aliases = { {0,"idle"},{1,"walk"},{12,"charge"},{24,"stop"},{36,"death"}, };
    anims.LoadFromTSX("assets/Textures/Spritesheets/Unicorn/u_spritesheet.tsx", aliases);
    anims.SetCurrent("idle");
    attackHitbox = nullptr;

    detectionRange = 20;
    speed = 10; 

    return true;
}

bool Horse::Update(float dt)
{
    if (hasBeenPicked) return true;

    if (Engine::GetInstance().scene->GetPlayer()->isDead()) return true;

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
    repathTimer++;
    if (damageTimer > 0) damageTimer--;

    GetPhysicsValues();
    distanceToPlayer = CalculateDistance();

    if (!isKnockback) {
        Sprint();

        if (!isSprinting && !isBraking) {
            if (distanceToPlayer < detectionRange) {
                PerformPathfinding();
                Move();
                SetState(EnemyState::RUNNING);
                if (velocity.y < -0.1f || velocity.y > 0.1f) {
                    SetState(EnemyState::IDLE);
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
        SetState(EnemyState::IDLE);

        if (knockbackTimer < 0) {
            knockbackTimer = knockbackDuration;
            isKnockback = false;
        }
    }

    ApplyPhysics();

    if (velocity.x < 0) facingLeft = true;
    else if (velocity.x > 0) facingLeft = false;

    Draw(dt);
    return true;
}

void Horse::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (isDead) {
        if (physB->ctype == ColliderType::PLATFORM && pbody != nullptr) {
            Engine::GetInstance().physics->SetLinearVelocity(pbody, { 0.0f, 0.0f });
            b2Body_SetGravityScale(pbody->body, 0.0f);
            //pbody->listener = nullptr;
        }
        return;
    }

    if (physB->ctype == ColliderType::PLAYER) {
        Player* player = dynamic_cast<Player*>(physB->listener);
        if (player && damageTimer <= 0) {
            Engine::GetInstance().scene->lives--;
            damageTimer = damageCooldown;
        }
    }
}

void Horse::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{

}

void Horse::ChangeCurrentAnimation()
{
    if (state == lastState) return;

    switch (state)
    {
    case EnemyState::IDLE:
        anims.SetCurrent("idle");
        break;

    case EnemyState::WALKING:
        anims.SetCurrent("walk");
        break;

    case EnemyState::RUNNING:
        anims.SetCurrent("charge");
        break;

    case EnemyState::DYING:
        anims.SetCurrent("death");
        break;
    }
}

void Horse::Draw(float dt)
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

    Engine::GetInstance().render->DrawTexture(texture, x - texW, y - texH + 128, &animFrame, 1.0f, 0.0, INT_MAX, INT_MAX, flip);
}

void Horse::Die() {
    isDead = true;
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