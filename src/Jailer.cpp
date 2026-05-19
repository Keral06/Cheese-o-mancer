#include "Jailer.h"
#include "Engine.h"
#include "Log.h"
#include "Textures.h"
#include "scene.h"

Jailer::Jailer() : Enemy()
{
    name = "Jailer";
}

Jailer::~Jailer()
{
}

bool Jailer::Start()
{
    texW = 128*4;
    texH = 128 * 4;
	attackRange = 5;
    offsetAttackHitboxX = 40;
    offsetAttackHitboxY = -texH/2;
    texName = "assets/Textures/Spritesheets/Jailer/sprite_jailer_02.png";
    texName2 = "assets/Textures/Spritesheets/Jailer/sprite_jailer_attack_02.png";
    spriteSheetName = "";
    type = EnemyType::GROUND;
    
    texture = Engine::GetInstance().textures->Load(texName);
    texture2 = Engine::GetInstance().textures->Load(texName2);

    pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.0f);


    pbody->listener = this;


    pbody->ctype = ColliderType::ENEMY;


    pathfinding = std::make_shared<Pathfinding>();

    Vector2D pos = GetPosition();

    Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());

    pathfinding->ResetPath(tilePos);


    std::unordered_map<int, std::string> aliases = { {0,"idle"},{19,"walk"},{38,"transform"}};
    anims.LoadFromTSX("assets/Textures/Spritesheets/Jailer/ja_spritesheet.tsx", aliases);

    std::unordered_map<int, std::string> aliases2 = { {0,"attack"}};
    anims2.LoadFromTSX("assets/Textures/Spritesheets/Jailer/ja_attack.tsx", aliases2);

    anims.SetCurrent("idle");
    anims2.SetCurrent("attack");

    CreateAttackHitbox(GetPosition().getX(), GetPosition().getY(), 70, 200);

    spawnPosition = GetPosition();

    jailerState = JAILER_IDLE;
    lastJailerState = JAILER_IDLE;

    detectionRange = 20;
    speed = 10;

    LOG("Verdugo creado");

    return true;
}

void Jailer::Attack()
{
	isAttacking = true;
	attackTimer = attackDuration;

	LOG("Verdugo empieza ataque");
}

bool Jailer::Update(float dt)
{
    GetPhysicsValues();

    switch (jailerState)
    {
    case JAILER_IDLE:
        UpdateIdle(dt);
        break;

    case JAILER_CHASE:
        UpdateChase(dt);
        break;

    case JAILER_ATTACK:
        UpdateAttack(dt);
        break;

    case JAILER_RETURN:
        UpdateReturn(dt);
        break;
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
    if (distanceToPlayer > attackRange + 2.0f)
    {
        attackLocked = false;
    }

    if (distanceToPlayer > detectionRange)
    {
        SetJailerState(JAILER_RETURN);
        return;
    }

    if (!attackLocked && distanceToPlayer <= attackRange)
    {
        SetJailerState(JAILER_ATTACK);
        return;
    }

    PerformPathfinding();
    Move();
}
void Jailer::UpdateAttack(float dt)
{
    velocity.x = 0;
    velocity.y = 0;

    anims2.Update(dt);

   
    if (anims2.HasFinished())
    {
        attackLocked = true;
        SetJailerState(JAILER_CHASE);
        return;
    }

    attackDurationTimer -= dt;

    // =========================
    // ACTIVAR HITBOX EN VENTANA
    // =========================
    if (attackDurationTimer <= (attackDuration - hitboxStart) &&
        attackDurationTimer >= (attackDuration - hitboxEnd))
    {
        if (!hitboxActive)
        {
            hitboxActive = true;
            hasHit = false;
            LOG("Hitbox ACTIVADA");
        }
    }
    else
    {
        if (hitboxActive)
        {
            hitboxActive = false;
            LOG("Hitbox DESACTIVADA");
        }
    }

    // =========================
    // HACER DAÑO (solo una vez)
    // =========================
    if (hitboxActive && playerInHitbox && !hasHit)
    {
        LOG("GOLPE AL PLAYER");

        Player* player = dynamic_cast<Player*>(Engine::GetInstance().scene->GetPlayer());

        if (player)
            player->lives--;

        hasHit = true;
    }

    // =========================
    // FIN DEL ATAQUE
    // =========================
    if (attackDurationTimer <= 0.0f)
    {
        hitboxActive = false;
        attackTimer = attackCooldown;

        SetJailerState(JAILER_CHASE);
    }
}

void Jailer::UpdateReturn(float dt)
{
    Vector2D current = GetPosition();

    Vector2D dir = spawnPosition - current;

    float len = sqrtf(
        dir.getX() * dir.getX() +
        dir.getY() * dir.getY()
    );

    if (len < 5.0f)
    {
        velocity.x = 0;
        velocity.y = 0;

        SetJailerState(JAILER_IDLE);
        return;
    }

    dir = Vector2D(
        dir.getX() / len,
        dir.getY() / len
    );

    velocity.x = dir.getX() * speed;
    velocity.y = dir.getY() * speed;
}

void Jailer::Draw(float dt)
{
    int x, y;

    if (pbody != nullptr) {
        pbody->GetPosition(x, y);
        position.setX((float)x);
        position.setY((float)y);
    }

    SDL_Rect animFrame;
    SDL_Texture* texToDraw = texture;
    SDL_FlipMode flip = facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    // =========================
    // ATTACK
    // =========================
    if (jailerState == JAILER_ATTACK)
    {
        anims2.Update(dt);
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
        x - texW/2,
        y - texH/2,
        &animFrame,
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );

    
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

    case EnemyState::RUNNING:
        anims2.SetCurrent("attack");
        break;

    case EnemyState::DYING:
        anims.SetCurrent("transform");
        break;
    }
}

void Jailer::SetJailerState(JailerState newState)
{
    if (jailerState == newState)
        return;

    lastJailerState = jailerState;
    jailerState = newState;

    switch (jailerState)
    {
    case JAILER_IDLE:
        SetState(EnemyState::IDLE);
        break;

    case JAILER_CHASE:
        SetState(EnemyState::WALKING);
        break;

    case JAILER_ATTACK:
        SetState(EnemyState::ATTACKING);
        break;

    case JAILER_RETURN:
        SetState(EnemyState::WALKING);
        break;
    }

    if (jailerState == JAILER_ATTACK)
    {
        attackDurationTimer = attackDuration;
        hitboxActive = false;
        hasHit = false;
        playerInHitbox = false;
    }
}

void Jailer::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (!hitboxActive) return;

    if (physB->ctype == ColliderType::PLAYER)
    {
        playerInHitbox = true;
    }
}

void Jailer::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype == ColliderType::PLAYER)
    {
        playerInHitbox = false;
    }
}

