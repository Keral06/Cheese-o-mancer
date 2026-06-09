#include "Jailer.h"
#include "Engine.h"
#include "Log.h"
#include "Textures.h"
#include "scene.h"
#include "EntityManager.h"
#include "coins.h"

Jailer::Jailer() : Enemy()
{
    name = "Jailer";
}

Jailer::~Jailer()
{
}

bool Jailer::Start()
{
    texW = 128 * 4;
    texH = 128 * 4;
    attackRange = 5;
    offsetAttackHitboxX = 280;
    offsetAttackHitboxY = 0;
    type = EnemyType::GROUND;

    texName = "assets/Textures/Spritesheets/Jailer/sprite_jailer_02.png";
    texName2 = "assets/Textures/Spritesheets/Jailer/sprite_jailer_attack_02.png";
    spriteSheetName = "";

    Enemy::Start();

    texture2 = Engine::GetInstance().textures->Load(texName2);

    if (pbody) {
        pbody->ctype = ColliderType::ENEMY;
        pbody->listener = this;
    }

    Vector2D pos = GetPosition();
    spawnPosition = pos;

    pathfinding = std::make_shared<Pathfinding>();
    Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());
    pathfinding->ResetPath(tilePos);

    std::unordered_map<int, std::string> aliases = { {0,"idle"},{19,"walk"},{38,"transform"} };
    anims.LoadFromTSX("assets/Textures/Spritesheets/Jailer/ja_spritesheet.tsx", aliases);

    std::unordered_map<int, std::string> aliases2 = { {0,"attack"} };
    anims2.LoadFromTSX("assets/Textures/Spritesheets/Jailer/ja_attack.tsx", aliases2);

    anims.SetCurrent("idle");
    anims2.SetCurrent("attack");

    CreateAttackHitbox(GetPosition().getX(), GetPosition().getY(), 70, 200);

    if (attackHitbox) {
        attackHitbox->ctype = ColliderType::ENEMY_ATTACK;
        attackHitbox->listener = this;
    }

    jailerState = JAILER_IDLE;
    lastJailerState = JAILER_IDLE;

	health = 100; // 5 golpes de 20 de daño
    detectionRange = 10;
    speed = 10;
    damageTimer = 0;
    damageCooldown = 60;

    LOG("Verdugo creado");

    return true;
}

void Jailer::Attack()
{
    isAttacking = true;
    attackDurationTimer = attackDuration;
    attackTimer = 0;
    hitboxActive = false;
    hasHit = false;

    LOG("Verdugo empieza ataque");
}

bool Jailer::Update(float dt)
{
    if (hasBeenPicked) return true;

    if (Engine::GetInstance().scene->GetPlayer()->isDead()) return true;

    // 1. ZONA SEGURA DE MUERTE
    if (health <= 0 && !isDead) {
        Die();
    }

    // 2. DIBUJAR CADÁVER
    if (isDead) { // Equivalente a jailerState == JAILER_DEATH
        anims.Update(dt);
        Draw(dt);
        return true;
    }

    // 3. LÓGICA NORMAL
    if (attackCooldownTimer > 0.0f) {
        attackCooldownTimer -= dt;
    }

    if (isKnockback) {
        knockbackTimer--;
        if (knockbackTimer <= 0) {
            isKnockback = false;
            knockbackTimer = knockbackDuration;
            Engine::GetInstance().physics->SetLinearVelocity(pbody, 0.0f, 0.0f);
        }
    }

    if (!isKnockback) {
        GetPhysicsValues();
        distanceToPlayer = CalculateDistance();

        switch (jailerState) {
        case JAILER_IDLE:   UpdateIdle(dt);   break;
        case JAILER_CHASE:  UpdateChase(dt);  break;
        case JAILER_ATTACK: UpdateAttack(dt); break;
        case JAILER_RETURN: UpdateReturn(dt); break;
        }
    }

    ApplyPhysics();
    Draw(dt);
    return true;
}

void Jailer::UpdateIdle(float dt)
{
    velocity.x = 0;

    if (distanceToPlayer < detectionRange)
    {
        SetJailerState(JAILER_CHASE);
    }
}

void Jailer::UpdateChase(float dt)
{
    if (distanceToPlayer > detectionRange)
    {
        SetJailerState(JAILER_RETURN);
        return;
    }

    PerformPathfinding();
    Move();

    if (justExitedAttack) {
        justExitedAttack = false;
    }

    if (attackCooldownTimer <= 0.0f && distanceToPlayer <= attackRange)
    {
        SetJailerState(JAILER_ATTACK);
        Attack();
        return;
    }
}

void Jailer::UpdateAttack(float dt)
{
    velocity.x = 0; velocity.y = 0;
    anims2.Update(dt);
    attackTimer++;

    if (attackTimer >= hitboxStart && attackTimer <= hitboxEnd) {
        hitboxActive = true;
    }
    else {
        hitboxActive = false;
        hasHit = false;
    }

    if (hitboxActive && playerInHitbox && !hasHit) {
        Engine::GetInstance().scene->lives--;
        hasHit = true;
        LOG("JAILER HITBOX DAMAGE");
    }

    if (anims2.HasFinished()) {
        attackCooldownTimer = attackCooldown;
        justExitedAttack = true;
        hitboxActive = false;
        hasHit = false;
        anims2.Resets();
        SetJailerState(JAILER_CHASE);
    }
}

void Jailer::UpdateReturn(float dt)
{
    int x, y;
    pbody->GetPosition(x, y);

    Vector2D current((float)x, (float)y);

    Vector2D dir = spawnPosition - current;

    float dx = spawnPosition.getX() - current.getX();
    float len = fabsf(dx);

    if (len < 10.0f)
    {
        velocity.x = 0;
        velocity.y = 0;
        SetJailerState(JAILER_IDLE);
        return;
    }

    dir = Vector2D(dir.getX() / len, dir.getY() / len);

    velocity.x = dir.getX() * speed;
    velocity.y = 0;

    if (dir.getX() < 0) facingLeft = true;
    else if (dir.getX() > 0) facingLeft = false;
}

void Jailer::Draw(float dt)
{
    int x = (int)position.getX();
    int y = (int)position.getY();

    if (pbody != nullptr) {
        pbody->GetPosition(x, y);
        position.setX((float)x);
        position.setY((float)y);
    }

    SDL_Rect animFrame;
    SDL_Texture* texToDraw = texture;
    SDL_FlipMode flip = facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    if (jailerState == JAILER_ATTACK)
    {
        animFrame = anims2.GetCurrentFrame();
        texToDraw = texture2;
    }
    else
    {
        anims.Update(dt);
        animFrame = anims.GetCurrentFrame();
        texToDraw = texture;
    }

    Engine::GetInstance().render->DrawTexture(
        texToDraw,
        x - texW / 2,
        y - texH / 2,
        &animFrame,
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );

    int offsetX = facingLeft ? -offsetAttackHitboxX : offsetAttackHitboxX;
    if (attackHitbox) {
        attackHitbox->SetPosition(x + offsetX, y + (int)offsetAttackHitboxY);
    }
}

void Jailer::ChangeCurrentAnimation()
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

    case EnemyState::ATTACKING:
        anims2.SetCurrent("attack");
        break;

    case EnemyState::DYING:
        anims.SetCurrent("transform");
        break;
    }
}

void Jailer::SetJailerState(JailerState newState)
{
    if (jailerState == newState) return;

    LOG("STATE CHANGE: %d -> %d (frame)", jailerState, newState);

    lastJailerState = jailerState;
    jailerState = newState;

    switch (jailerState)
    {
    case JAILER_IDLE: SetState(EnemyState::IDLE); break;
    case JAILER_CHASE: SetState(EnemyState::WALKING); break;
    case JAILER_ATTACK: SetState(EnemyState::ATTACKING); break;
    case JAILER_RETURN: SetState(EnemyState::WALKING); break;
    case JAILER_DEATH: SetState(EnemyState::DYING); break;
    }

    ChangeCurrentAnimation();
}

void Jailer::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (isDead) {
        if (physB->ctype == ColliderType::PLATFORM && pbody != nullptr) {
            Engine::GetInstance().physics->SetLinearVelocity(pbody, { 0.0f, 0.0f });
            b2Body_SetGravityScale(pbody->body, 0.0f);
            pbody->listener = nullptr;
        }
        return;
    }

    if (physA == attackHitbox && physB->ctype == ColliderType::PLAYER)
    {
        playerInHitbox = true;
    }
}

void Jailer::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
    if (physA == attackHitbox && physB->ctype == ColliderType::PLAYER)
    {
        playerInHitbox = false;
    }
}

void Jailer::Die() {
    isDead = true;
    SetJailerState(JAILER_DEATH);
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