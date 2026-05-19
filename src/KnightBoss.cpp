#include "KnightBoss.h"

#include "BossFightPrincessKnight.h"

#include "Engine.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Textures.h"

// =========================
// CONSTRUCTOR
// =========================

KnightBoss::KnightBoss() : Enemy()
{
    name = "KnightBoss";
}

KnightBoss::~KnightBoss()
{
}

// =========================
// START
// =========================

bool KnightBoss::Start()
{
    texW = 128 * 2;
    texH = 128 * 5;

    type = EnemyType::MELEE;

    stateTimer = 0.0f;

    attackSpeed = 12.0f;

    bounceCount = 0;

    busy = false;

    invulnerable = true;

    std::unordered_map<int, std::string> aliasesNIdle = { {0,"idle"}};
    std::unordered_map<int, std::string> aliasesNSlide = { {0,"slide"}};
    std::unordered_map<int, std::string> aliasesCIdle = { {0,"idle"}};
    std::unordered_map<int, std::string> aliasesCAttack1 = { {0,"attack1"} };
    std::unordered_map<int, std::string> aliasesCAttack2 = { {0,"attack2"} };
    std::unordered_map<int, std::string> aliasesCDefeat = { {0,"defeat"} };
    std::unordered_map<int, std::string> aliasesCTransform = { {0,"transform"}, {21, "death"}};

    animsNIdle.LoadFromTSX("assets/Textures/Spritesheets/Knight/Normal/kn_idle.tsx", aliasesNIdle);
    animsNSlide.LoadFromTSX("assets/Textures/Spritesheets/Knight/Normal/kn_slide.tsx", aliasesNSlide);
    animsCIdle.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_3x4.tsx", aliasesCIdle);
    animsCAttack1.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/kc_attack1.tsx", aliasesCAttack1);
    animsCAttack2.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_5x5.tsx", aliasesCAttack2);
    animsCDefeat.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_5x5.tsx", aliasesCDefeat);
    animsCTransform.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_5x5.tsx", aliasesCTransform);

    textureNIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Normal/KnightN_Idle.png");
    textureNSlide = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Normal/KnightN_sliding.png");
    textureCIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Idle.png");
    textureCAttack1 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Attack1.png");
    textureCAttack2 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Attack2.png");
    textureCDefeat = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Defeat.png");
    textureCTransform = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Transform_And_Walk.png");
    
    texture = textureNIdle;


    LOG("Knight Pos: %f %f", position.getX(), position.getY());
    pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.0f);


    pbody->listener = this;


    pbody->ctype = ColliderType::ENEMY;


    pathfinding = std::make_shared<Pathfinding>();

    Vector2D pos = GetPosition();

    Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());

    pathfinding->ResetPath(tilePos);

    knightState = KnightState::IDLE;
    lastKnightState = KnightState::IDLE;

    actionFinished = false;
    attackHitbox = nullptr;

    currentAnim = &animsNIdle;
    currentTexture = textureNIdle;

    return true;
}

// =========================
// UPDATE
// =========================

bool KnightBoss::Update(float dt)
{
    stateTimer += dt;

    GetPhysicsValues();

    if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_N))
    {
        StartBounceAttack();
    }

    switch (knightState)
    {
    case KnightState::IDLE:
        break;

    case KnightState::ENTRANCE_DASH:
        UpdateEntrance(dt);
        break;

    case KnightState::TRANSFORM:
    {
        if (stateTimer >= 2.0f)
        {
            busy = false;

            SetKnightState(KnightState::IDLE);
        }

        break;
    }

    case KnightState::LUNGE_ATTACK:
        UpdateLunge(dt);
        break;

    case KnightState::BOUNCE_ATTACK:
        UpdateBounce(dt);
        break;

    case KnightState::RECOVER:
        UpdateRecover(dt);
        break;

    case KnightState::DEATH:
        break;
    }

    ApplyPhysics();

    Draw(dt);

    return true;
}

// =========================
// ENTRANCE
// =========================

void KnightBoss::StartEntrance()
{
    busy = true;

    stateTimer = 0.0f;

    SetKnightState(KnightState::ENTRANCE_DASH);

    LOG("Knight Entrance");
}

void KnightBoss::UpdateEntrance(float dt)
{
    velocity.x = -12.0f;
    velocity.y = 0;

    if (stateTimer >= 1.5f)
    {
        velocity.x = 0;

        busy = false;

        SetKnightState(KnightState::IDLE);
    }
}

// =========================
// TRANSFORM
// =========================

void KnightBoss::StartTransform()
{
    busy = true;

    stateTimer = 0.0f;

    SetKnightState(KnightState::TRANSFORM);

    LOG("Knight Transform");
}

// =========================
// LUNGE
// =========================

void KnightBoss::StartLungeAttack(float speedMultiplier)
{
    if (!canAct) return;

    busy = true;

    stateTimer = 0.0f;

    attackSpeed = 15.0f * speedMultiplier;

    Vector2D player =
        Engine::GetInstance().scene->GetPlayerPosition();

    int ex, ey;
    pbody->GetPosition(ex, ey);

    attackDirection = player - Vector2D(ex, ey);

    float len =
        sqrtf(
            attackDirection.getX() * attackDirection.getX() +
            attackDirection.getY() * attackDirection.getY()
        );

    if (len > 0.0f)
    {
        attackDirection =
        {
            attackDirection.getX() / len,
            attackDirection.getY() / len
        };
    }

    SetKnightState(KnightState::LUNGE_ATTACK);

    LOG("Knight Lunge");
}

void KnightBoss::UpdateLunge(float dt)
{
    velocity.x = attackDirection.getX() * attackSpeed;
    velocity.y = attackDirection.getY() * attackSpeed;

    if (stateTimer >= 1.2f)
    {
        velocity.x = 0;
        velocity.y = 0;

        busy = false;

        SetKnightState(KnightState::RECOVER);

        if (fightController)
            FinishAction();
    }
}

// =========================
// BOUNCE
// =========================

void KnightBoss::StartBounceAttack()
{
    if (!canAct) return;

    busy = true;

    stateTimer = 0.0f;
    bounceCount = 0;
    maxBounces = 6;

    SetKnightState(KnightState::BOUNCE_ATTACK);

    Vector2D player = Engine::GetInstance().scene->GetPlayerPosition();

    int x, y;
    pbody->GetPosition(x, y);

    bounceDir = Vector2D(0.5f, -1.0f);

    float len = sqrtf(
        bounceDir.getX() * bounceDir.getX() +
        bounceDir.getY() * bounceDir.getY()
    );

    if (len > 0.0f)
    {
        bounceDir = Vector2D(
            bounceDir.getX() / len,
            bounceDir.getY() / len
        );
    }

    bounceSpeed = 30.0f;

    LOG("Knight Bounce Attack");
}

void KnightBoss::UpdateBounce(float dt)
{
    velocity.x = bounceDir.getX() * bounceSpeed;
    velocity.y = bounceDir.getY() * bounceSpeed;

    if (bounceCount >= maxBounces)
    {
        velocity.x = 0;
        velocity.y = 0;

        busy = false;

        SetKnightState(KnightState::RECOVER);

        FinishAction();
    }
}

// =========================
// RECOVER
// =========================

void KnightBoss::UpdateRecover(float dt)
{
    velocity.x = 0;
    velocity.y = 0;

    if (stateTimer >= 0.5f)
    {
        SetKnightState(KnightState::IDLE);
    }
}

// =========================
// HELPERS
// =========================

void KnightBoss::SetFightController(BossFightPrincessKnight* controller)
{
    fightController = controller;
}

bool KnightBoss::IsBusy() const
{
    return busy;
}

bool KnightBoss::IsInvulnerable() const
{
    return invulnerable;
}

// =========================
// STATE
// =========================

void KnightBoss::SetKnightState(KnightState newState)
{
    if (knightState == newState)
        return;

    lastKnightState = knightState;

    knightState = newState;

    stateTimer = 0.0f;

    ChangeCurrentAnimation();

    LOG("Knight State: %d", (int)knightState);
}

// =========================
// DRAW
// =========================

void KnightBoss::Draw(float dt)
{
    if (currentAnim == nullptr)
        return;

    currentAnim->Update(dt);

    const SDL_Rect& animFrame =
        currentAnim->GetCurrentFrame();

    int x, y;

    if (pbody != nullptr)
    {
        pbody->GetPosition(x, y);

        position.setX((float)x);
        position.setY((float)y);
    }
    else
    {
        x = (int)deathPosition.getX();
        y = (int)deathPosition.getY();
    }

    SDL_FlipMode flip =
        facingLeft ?
        SDL_FLIP_NONE :
        SDL_FLIP_HORIZONTAL;

    Engine::GetInstance().render->DrawTexture(
        currentTexture,
        x - texW / 2,
        y - texH / 2,
        &animFrame,
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );
}

void KnightBoss::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (knightState != KnightState::BOUNCE_ATTACK)
        return;

   
    bounceDir.setY(-bounceDir.getY());

    
    velocity.x = bounceDir.getX() * bounceSpeed;
    velocity.y = bounceDir.getY() * bounceSpeed;

    bounceCount++;
}

void KnightBoss::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
}

bool KnightBoss::HasFinishedAction() const
{
    return actionFinished;
}

void KnightBoss::ResetActionFinished()
{
    actionFinished = false;
}

void KnightBoss::ChangeCurrentAnimation()
{
    switch (knightState)
    {
    case KnightState::IDLE:
    {
        currentAnim = &animsNIdle;
        currentTexture = textureNIdle;
        break;
    }

    case KnightState::ENTRANCE_DASH:
    {
        currentAnim = &animsNSlide;
        currentTexture = textureNSlide;
        break;
    }

    case KnightState::TRANSFORM:
    {
        currentAnim = &animsCTransform;
        currentTexture = textureCTransform;
        break;
    }

    case KnightState::LUNGE_ATTACK:
    {
        currentAnim = &animsCAttack1;
        currentTexture = textureCAttack1;
        break;
    }

    case KnightState::BOUNCE_ATTACK:
    {
        currentAnim = &animsCAttack2;
        currentTexture = textureCAttack2;
        break;
    }

    case KnightState::RECOVER:
    {
        currentAnim = &animsCIdle;
        currentTexture = textureCIdle;
        break;
    }

    case KnightState::DEATH:
    {
        currentAnim = &animsCDefeat;
        currentTexture = textureCDefeat;
        break;
    }
    }

    if (currentAnim != nullptr)
    {
        currentAnim->Resets();
    }
}

void KnightBoss::FinishAction()
{
    actionFinished = true;
    busy = false;

    if (fightController) {
    
    }
        //fightController->OnKnightFinishedAction();
}

