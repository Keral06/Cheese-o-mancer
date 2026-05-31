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

    detectionRange = 10;
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
    distanceToPlayer = CalculateDistance();

    justExitedAttack = false;

    if (attackCooldownTimer > 0.0f)
        attackCooldownTimer -= dt;

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
    if (distanceToPlayer > detectionRange)
    {
        SetJailerState(JAILER_RETURN);
        return;
    }

    PerformPathfinding();
    Move();

    if (justExitedAttack)
        return;

    // SOLO atacar si cooldown ok
    if (attackCooldownTimer <= 0.0f &&
        distanceToPlayer <= attackRange)
    {
        SetJailerState(JAILER_ATTACK);
        return;
    }
}
void Jailer::UpdateAttack(float dt)
{
    LOG("ATTACK | timer=%.2f | animFinished=%d",
        attackDurationTimer,
        anims2.HasFinished());

    velocity.x = 0;
    velocity.y = 0;

    anims2.Update(dt);

    attackDurationTimer -= dt;

    if (anims2.HasFinished())
    {
        LOG("ATTACK FINISHED -> CHASE");
        attackCooldownTimer = attackCooldown;
        justExitedAttack = true;
        anims2.Resets();
        SetJailerState(JAILER_CHASE);
        return;
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
    if (jailerState == newState)
        return;

    LOG("STATE CHANGE: %d -> %d (frame)", jailerState, newState);

    lastJailerState = jailerState;
    jailerState = newState;

    switch (jailerState)
    {
    case JAILER_IDLE:
        LOG(" -> IDLE");
        SetState(EnemyState::IDLE);
        break;

    case JAILER_CHASE:
        LOG(" -> CHASE");
        SetState(EnemyState::WALKING);
        break;

    case JAILER_ATTACK:
        LOG(" -> ATTACK");
        SetState(EnemyState::ATTACKING);
        break;

    case JAILER_RETURN:
        LOG(" -> RETURN");
        SetState(EnemyState::WALKING);
        break;
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

